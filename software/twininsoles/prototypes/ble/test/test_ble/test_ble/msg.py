from enum import Enum


class CMD_MODE(int, Enum):
    REQ_BENCHMARK1: int = 21
    REQ_BENCHMARK2: int = 22
    REQ_BENCHMARK3: int = 23
    REQ_STOP_BENCHMARK: int = 24


def create_cmd_msg_dict(mode: CMD_MODE):
    return {"type": "command", "mode": mode}
