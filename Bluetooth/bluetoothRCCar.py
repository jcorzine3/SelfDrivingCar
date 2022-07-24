# https://create.arduino.cc/projecthub/ansh2919/serial-communication-between-python-and-arduino-e7cce0
# For the connection between python and arduino
import bluetooth
import serial
from pprint import pprint
import time

#Jesse's port '/devices/cu.usbmodem14201'
arduino = serial.Serial(port='COM3', baudrate=115200)
bluetooth_rc_car = serial.Serial('COM7', baudrate=9600)

def write_read():
    data = arduino.readline()
    return data

while True:
    value = write_read()
    value = value.decode("utf-8")
    value = value.strip()
    value = str(int(float(value)))
    bluetooth_rc_car.write(value.encode("utf-8"))
    print(value)

