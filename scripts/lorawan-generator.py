#!/usr/bin/env python3

import os

def generate_deveui():
    # Generate a random 8-byte (64-bit) DevEUI as a hex string
    return ''.join(f'{b:02X}' for b in os.urandom(8))

def generate_appkey():
    # Generate a random 16-byte (128-bit) AppKey as a hex string
    return ''.join(f'{b:02x}' for b in os.urandom(16))

for i in range(2):
    deveui = generate_deveui()
    appkey = generate_appkey()
    print(f"Board:  {i+1}")
    print(f"DevEUI: {deveui}")
    print(f"AppKey: {appkey}")
    print("*" * 32)
