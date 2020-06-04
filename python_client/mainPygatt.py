import pygatt
from pygatt.backends.backend import BLEAddressType
from binascii import hexlify

address = "E8:17:29:04:2E:CF"
main_srv = "daeb885a-8e55-4865-bfd1-86f53f134280"
srv = "daeb1501-8e55-4865-bfd1-86f53f134280"

def handle_data(handle, value):
    """
    handle -- integer, characteristic read handle the data was received on
    value -- bytearray, the data returned in the notification
    """
    print("Received data: %s" % hexlify(value))


adapter = pygatt.GATTToolBackend()

try:
    adapter.start()
    device = adapter.connect(address,address_type=BLEAddressType.random)
    device.subscribe(srv, callback=handle_data)
finally:
    adapter.stop()

while True:
    pass

adapter.stop()
