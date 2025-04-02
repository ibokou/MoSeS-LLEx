import asyncio
from bleak import BleakClient
from bleak import BleakScanner
from test_ble.util.bluetoothctl import Bluetoothctl
from test_ble.benchmark import start_benchmark1
import time


async def main(
    ble_name: str, svc_uuid: str, read_chr_uuid: str, write_chr_uuid: str
) -> None:

    #hctl = Bluetoothctl()
    #hctl.start_ble_scan()

    device = await BleakScanner.find_device_by_filter(
        lambda d, ad: d.name and d.name.lower() == ble_name.lower()
    )

    async with BleakClient(device) as client:
        # system_id = await client.read_gatt_char(read_chr_uuid)
        await start_benchmark1(client, read_chr_uuid, write_chr_uuid)
        time.sleep(100)


def start():
    device_name: str = "BLE-TwinInsole"
    uuid_template: str = "0000{0:x}-0000-1000-8000-00805f9b34fb"

    SVC_UUID = uuid_template.format(0x180)
    READ_VAL_UUID = uuid_template.format(0xFEF4)
    READ_WRITE_CMD_UUID = uuid_template.format(0xDFE1)

    asyncio.run(main(device_name, SVC_UUID, READ_VAL_UUID, READ_WRITE_CMD_UUID))
