#ifndef BLUEZ_CONSTANTS_H
#define BLUEZ_CONSTANTS_H

static constexpr const char *BLUEZ_INTERFACE_NAME =
    "org.bluez.LEAdvertisement1";
static constexpr const char *DBUS_PROP_INTERFACE_NAME =
    "org.freedesktop.DBus.Properties";
static constexpr const char *DBUS_OBJ_MG_INTERFACE_NAME =
    "org.freedesktop.DBus.ObjectManager";

static constexpr const char *GATT_SVC_INTERFACE_NAME = "org.bluez.GattService1";
static constexpr const char *GATT_CHR_INTERFACE_NAME =
    "org.bluez.GattCharacteristic1";

static constexpr const char *HCI_BT_ADAPTER_OBJ_PATH = "/org/bluez/hci0";
static constexpr const char *GATT_ADV_OBJ_PATH =
    "/org/bluez/ldsg/advertisement0";
static constexpr const char *GATT_APP_OBJ_PATH = "/org/bluez/ldsg/twincentral0";
static constexpr const char *GATT_SVC_OBJ_PATH =
    "/org/bluez/ldsg/twincentral0/service0";
static constexpr const char *GATT_CHR_OBJ_PATH =
    "/org/bluez/ldsg/twincentral0/service0/char0";
static constexpr const char *GATT_SVC_UUID =
    "e95d6100-251d-470a-a062-fa1922dfa9a8";
static constexpr const char *GATT_CHR_UUID =
    "e95d9250-251d-470a-a062-fa1922dfa9a8";

#endif