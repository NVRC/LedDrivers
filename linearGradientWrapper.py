from __future__ import division
import nLevelLinearGradient as lg
import gradientHelpers as gh
import threading
import time
from appJar import gui

class linearGradientWrapper:

    def __init__(self):
        self.initColorList = []
        self.segmentColorList = []
        self.brightness = 255
        self.TIMER_CONSTANT = 0.1
        self.LED_CONSTANT = 60

    def addRGB(self, r, g, b):
        hex = gh.rgb_to_hex(r,g,b)
        self.initColorList.append(hex[1:])
        pass


    def addHEX(self, hex):
        self.initColorList.append(hex)
        pass

    def addArray(self, array):
        self.initColorList = array;
        pass


    def setBrightness(self, brightness):
        self.brightness = brightness
        pass

    def display(self):
        lg.display(getArgs(self))
        pass

    def getArgs(self):
        list = self.initColorList[:]
        list.append(self.brightness)
        return list

    def cycle(self):
        cycle(self.TIMER_CONSTANT)

    def cycle(self,totalcycletime):
        print("Linear Gradient Wrapper: executing a cyclical gradient")
        #Append the first color so that the cyclical gradient is linear.
        self.initColorList.append(self.initColorList[0])
        init = self.getArgs()
        next = lg.getColorList(init)

        while(True):
            next = gh.getShiftedColors(next)
            lg.led_output(next)
            time.sleep(totalcycletime/self.LED_CONSTANT)
        pass


    def printColorList(self):
        print("Current Color List: ",self.initColorList)
        pass
