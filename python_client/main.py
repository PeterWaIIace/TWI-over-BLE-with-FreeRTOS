# bluetooth low energy scan
from bluetooth.ble import DiscoveryService
# from bluetooth.ble import BeaconService

find_address = "E8:17:29:04:2E:CF"
service = DiscoveryService()
# service = BeaconService()

finish = True
while finish:
    try:
        devices = service.discover(2)
        # devices = service.scan(2)
        finish = False
        print("scanned")
    except:
        pass
# print("after",list(devices.items()))

for address, name in devices.items():
# for address, data in list(devices.items()):
    print("name: {}, address: {}".format(name, address))
    if(find_address==address):
        print("^ Found")

###### 
## Data reading 

# from gattlib import GATTRequester

# req = GATTRequester("00:11:22:33:44:55")
# "daeb885a-8e55-4865-bfd1-86f53f134280" - main service
# "daebXXXX-8e55-4865-bfd1-86f53f134280" - this part is exchangable
# services from 1501 - 1506
# name = req.read_by_uuid("00002a00-0000-1000-8000-00805f9b34fb")[0]
# steps = req.read_by_handle(0x15)[0]
