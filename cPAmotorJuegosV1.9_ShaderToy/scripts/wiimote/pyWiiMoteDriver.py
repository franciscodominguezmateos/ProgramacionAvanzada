'''
Created on 15 Jul 2019

@author: Francisco Dominguez
'''

import cwiid 
import time 
import socket
import argparse

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument('--ip', default="localhost")
    parser.add_argument('--id', type=int, default=0)
    parser.add_argument('--port', type=int, default=8881)
    args = parser.parse_args()

    #connecting to the Wiimote. This allows several attempts 
    # as first few often fail. 
    #print dir(cwiid.Wiimote)
    print 'Press 1+2 on your Wiimote now...' 
    wm = None 
    i=2 
    while not wm: 
      try: 
        wm=cwiid.Wiimote() 
      except RuntimeError: 
        if (i>10): 
          quit() 
          break 
        print "Error opening wiimote connection" 
        print "attempt " + str(i) 
        i +=1 
    
    #set Wiimote to report button presses and accelerometer state 
    wm.rpt_mode = cwiid.RPT_BTN | cwiid.RPT_ACC | cwiid.RPT_IR | cwiid.RPT_EXT | cwiid.RPT_NUNCHUK
     
    #turn on led to show connected id
    wm.led = 2**args.id
    
    addr = (args.ip, args.port)
    while True:
        #print(wm.state['acc'],wm.state["buttons"])
        #print(wm.state)
        try:
            sock = socket.socket()
            sock.connect(addr)
            x=wm.state['acc'][0]
            y=wm.state['acc'][1]
            z=wm.state['acc'][2]
            s="device=WiiMote&id=%i&x=%i&y=%i&z=%i&buttons=%s" % (args.id,x,y,z,wm.state["buttons"])
            #print(s)
            sock.send(s.encode())
            sock.recv(1000)
            #print(self.sock.recv(1000).decode())
            sock.close()
        except socket.error as serr: 
            print("Connection refused: WiiMote->PAGameEngine")
            print wm.state
            time.sleep(5.1)
        time.sleep(0.03)
