# https://create.arduino.cc/projecthub/ansh2919/serial-communication-between-python-and-arduino-e7cce0
# For the connection between python and arduino
import bluetooth
import serial
from pprint import pprint
import time

#Jesse's port '/devices/cu.usbmodem14201'
arduino = serial.Serial(port='COM3', baudrate=115200)
bluetooth_rc_car = serial.Serial('COM7')

# def scan():

#     print("Scanning for bluetooth devices:")
#     devices = bluetooth.discover_devices(lookup_names = True, lookup_class = True)
#     number_of_devices = len(devices)
#     names = []
#     info_tuples = []
#     print(number_of_devices,"devices found")
#     for addr, name, device_class in devices:
#         print("\n")
#         print("Device:")
#         print("Device Name: %s" % (name))
#         names.append(name)
#         info_tuples.append((addr, device_class))
#         print("Device MAC Address: %s" % (addr))
#         print("Device Class: %s" % (device_class))
#         print("\n")
#     if 'HC-02' in names:
#         car_info_index = names.index('HC-02')
#         print("Found RC Car")
#         car_addr, car_class = info_tuples[car_info_index]
#         service = bluetooth.find_service(address=car_addr)
#         pprint(service)

# scan()

def write_read():
    data = arduino.readline()
    return data

while True:
    value = write_read()
    value = value.decode("utf-8")
    value = value.strip()
    value = int(float(value))
    bluetooth_rc_car.write(bin(value))
    print(value)

