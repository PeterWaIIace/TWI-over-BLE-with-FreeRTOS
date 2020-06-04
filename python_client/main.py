# bluetooth low energy scan

from __future__ import print_function

import sys
from threading import Event
from bluetooth.ble import DiscoveryService
from bluetooth.ble import GATTRequester, GATTResponse
import time
# from bluetooth.ble import BeaconService

#####################################################################################3
# test device reading 

# "daeb885a-8e55-4865-bfd1-86f53f134280" - main service
# "daebXXXX-8e55-4865-bfd1-86f53f134280" - this part is exchangable
# services from 1501 - 1506


address = "E8:17:29:04:2E:CF"
main_service = "daeb885a-8e55-4865-bfd1-86f53f134280"
pattern_service = "daebXXXX-8e55-4865-bfd1-86f53f134280"



class Requester(GATTRequester):
    def __init__(self, wakeup, *args):
        GATTRequester.__init__(self, *args)
        self.wakeup = wakeup

    def on_notification(self, handle, data):
        print("- notification on handle: {}\n".format(handle))
        self.wakeup.set()


class ReceiveNotification(object):
    def __init__(self, address):
        self.received = Event()
        self.requester = GATTRequester(address, False)

        self.connect()
        self.wait_notification()

    def connect(self):
        print("Connecting...", end=' ')
        sys.stdout.flush()
        connected = False
        while not connected:
            try:  
                self.requester.connect(True, channel_type='random')
                print("connected")
            except Exception as e:
                self.requester.disconnect()
                print("Exception caugth:", e)
            connected = self.requester.is_connected()
        print("OK!")

    def wait_notification(self):
        print("\nThis is a bit tricky. You need to make your device to send\n"
              "some notification. I'll wait...")
        self.received.wait()

if __name__ == "__main__":
    # response = NotifyYourName()
    req = ReceiveNotification(address)
    
    while True:
        pass
        # print("")



        