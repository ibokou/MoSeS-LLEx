#ifndef H_BLE_CTRL_
#define H_BLE_CTRL_

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "host/ble_hs.h"
#include "host/util/util.h"
#include "nimble/nimble_npl.h"
#include "nimble/nimble_port.h"

/*Low level NimBLE control functions*/
void ble_hci_sock_ack_handler(void *param);
void ble_hci_sock_init(void);
void ble_hci_sock_set_device(int dev);
void ble_store_ram_init(void);

/*Application level control functions*/
void nimble_host_task(void *param);

#endif