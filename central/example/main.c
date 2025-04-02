/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional "INFO %s"rmation
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "ble_ctrl.h"

static struct ble_npl_task s_task_host;
static struct ble_npl_task s_task_hci;

#define TASK_DEFAULT_PRIORITY 1
#define TASK_DEFAULT_STACK NULL
#define TASK_DEFAULT_STACK_SIZE 400

static const char *device_name = "Apache Mynewt";

static void advertise(void);

static void set_ble_addr(void) {
  int rc;
  ble_addr_t addr;

  /* generate new non-resolvable private address */
  rc = ble_hs_id_gen_rnd(1, &addr);
  assert(rc == 0);

  /* set generated address */
  rc = ble_hs_id_set_rnd(addr.val);
  assert(rc == 0);
}

static int adv_event(struct ble_gap_event *event, void *arg) {
  switch (event->type) {
    case BLE_GAP_EVENT_ADV_COMPLETE:
      printf("INFO %s", "Advertising completed, termination code: %d\n",
             event->adv_complete.reason);
      advertise();
      return 0;
    default:
      printf("ERROR %s", "Advertising event not handled\n");
      return 0;
  }
}

static void advertise(void) {
  int rc;
  struct ble_gap_adv_params adv_params;
  struct ble_hs_adv_fields fields;

  /* set adv parameters */
  memset(&adv_params, 0, sizeof(adv_params));
  adv_params.conn_mode = BLE_GAP_CONN_MODE_NON;
  adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

  memset(&fields, 0, sizeof(fields));

  /* Fill the fields with advertising data - flags, tx power level, name */
  fields.flags = BLE_HS_ADV_F_DISC_GEN;
  fields.tx_pwr_lvl_is_present = 1;
  fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;
  fields.name = (uint8_t *)device_name;
  fields.name_len = strlen(device_name);
  fields.name_is_complete = 1;

  rc = ble_gap_adv_set_fields(&fields);
  assert(rc == 0);

  printf("INFO %s", "Starting advertising...\n");

  /* As own address type we use hard-coded value, because we generate
     NRPA and by definition it's random */
  rc = ble_gap_adv_start(BLE_OWN_ADDR_RANDOM, NULL, 10000, &adv_params,
                         adv_event, NULL);
  assert(rc == 0);
}

static void on_sync(void) {
  set_ble_addr();

  /* begin advertising */
  advertise();
}

static void on_reset(int reason) {
  printf("Resetting state; reason=%d\n", reason);
}

void *ble_hci_sock_task(void *param) {
  ble_hci_sock_ack_handler(param);
  return NULL;
}

void *ble_host_task(void *param) {
  nimble_host_task(param);
  return NULL;
}

int main(int argc, char *argv[]) {
  int ret = 0;

  /* allow to specify custom hci */
  if (argc > 1) {
    ble_hci_sock_set_device(1);
  }

  ble_hs_cfg.sync_cb = on_sync;
  ble_hs_cfg.reset_cb = on_reset;

  nimble_port_init();
  ble_hci_sock_init();

  /* XXX Need to have template for store */
  ble_store_ram_init();

  int rc = ble_svc_gap_device_name_set(device_name);
  assert(rc == 0);

  ble_npl_task_init(&s_task_hci, "hci_sock", ble_hci_sock_task, NULL,
                    TASK_DEFAULT_PRIORITY, BLE_NPL_TIME_FOREVER,
                    TASK_DEFAULT_STACK, TASK_DEFAULT_STACK_SIZE);

  /* Create task which handles default event queue for host stack. */
  ble_npl_task_init(&s_task_host, "ble_host", ble_host_task, NULL,
                    TASK_DEFAULT_PRIORITY, BLE_NPL_TIME_FOREVER,
                    TASK_DEFAULT_STACK, TASK_DEFAULT_STACK_SIZE);

  pthread_exit(&ret);
}
