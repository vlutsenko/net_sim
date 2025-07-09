import sysrepo
import libyang
import random



def hardware_components(xpath, private_data, **kwargs):
    print(xpath)
    return {
        "hardware": {
            "component": [
                {
                    "name": "card",
                    "class": "bbf-hardware-types:board",
                    "description": "Board",
                    "firmware-rev": "simulated-fc",
                    "software-rev": "R0.00.100-sim",
                    "serial-num": "00000000",
                    "mfg-name": "Argonauts",
                    "model-name": "simV-FC-AEHG4-AEXG4"
                }
