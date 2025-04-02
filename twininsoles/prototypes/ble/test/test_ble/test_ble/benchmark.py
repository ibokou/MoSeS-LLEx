from test_ble.msg import create_cmd_msg_dict
from test_ble.msg import CMD_MODE
from test_ble.io import DataPointWriter

import time

from bleak import BleakClient
import json


async def start_benchmark1(
    client: BleakClient, read_chr_uuid: str, write_chr_uuid: str
) -> None:
    json_string: str = json.dumps(create_cmd_msg_dict(CMD_MODE.REQ_BENCHMARK1))
    writer = DataPointWriter("data/benchmark1.csv")
    writer.write_header()
    await client.write_gatt_char(write_chr_uuid, bytes(json_string, "utf-8") + b"\x00")
    while True:
        byte_msg = await client.read_gatt_char(read_chr_uuid)
        json_str = byte_msg.decode("utf-8")
        data_pt = json.loads(json_str)
        writer.write_row(data_pt)
        print(data_pt)
        time.sleep(0.01)


async def start_benchmark2(
    client: BleakClient, read_chr_uuid: str, write_chr_uuid: str
) -> None:

    while True:
        pass


async def start_benchmark3(
    client: BleakClient, read_chr_uuid: str, write_chr_uuid: str
) -> None:

    while True:
        pass
