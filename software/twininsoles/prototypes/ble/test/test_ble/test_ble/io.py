from typing import Dict, Any, IO
import os
import random
import string
import csv


class DataPointWriter:
    def __init__(self, csv_path: str):
        csv_path: str = csv_path
        os.makedirs(os.path.dirname(csv_path), exist_ok=True)
        csv_path = self.if_exists_create_new(csv_path)
        self.csv_file: IO[Any] = open(csv_path, "a+", newline="")

        fieldnames = [
            "timestamp",
            "gait",
            "Heel",
            "Lateral",
            "4thmetatarsal",
            "1thmetatarsal"
        ]
        self.writer: cvs.DictWriter = csv.DictWriter(
            self.csv_file, fieldnames=fieldnames
        )

    def if_exists_create_new(self, csv_path: str):
        if os.path.exists(csv_path):

            suffix: str = "".join(
                random.choices(string.ascii_uppercase + string.digits, k=4)
            )

            new_path = os.path.splitext(csv_path)[0] + "_" + suffix + ".csv"
            open(new_path, "w+").close()

            return new_path
        else:
            return csv_path

    def write_header(self):
        self.writer.writeheader()
        self.csv_file.flush()

    def write_row(self, json_dict: Dict[str, Any]):
        self.writer.writerow(
            {
                "timestamp": json_dict["timestamp"],
                "gait": json_dict["gait"],
                "Heel": json_dict["FSR"]["Heel"],
                "Lateral": json_dict["FSR"]["Lateral"],
                "4thmetatarsal": json_dict["FSR"]["4thmetatarsal"],
                "1thmetatarsal": json_dict["FSR"]["1thmetatarsal"],
            }
        )
        self.csv_file.flush()

    def close():
        self.csv_file.close()
