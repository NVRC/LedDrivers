import nLevelLinearGradient as lg
import gradientHelpers as gh
import threading
from appJar import gui

class linearGradientWrapper:

    def __init__(self):
        self.initColorList = []
        self.nextColorList = []
        self.brightness = ['255']
        self.TIMER_CONSTANT = 1

    @staticmethod
    def addRGB(r,g,b):
        hex = gh.rgb_to_hex(r,g,b)
        self.initColorList.append(hex[1:])
        pass

    @staticmethod
    def addHEX(hex):
        self.initColorList.append(hex)
        pass

    @staticmethod
    def setBrightness(brightness):
        self.brightness = brightness
        pass

    def display():
        list = initColorList
        list.append(brightness)
        lg.display(getArgs())
        pass

    def getArgs():
        list = initColorList
        return list.append(brightness)

    def cycle():
        nextColorList = lg.getColorList(getArgs())
        threading.Timer(self.TIMER_CONSTANT,cycleLoop).start
        pass

    def cycleLoop():
        lg.led_output(nextColorList)
        nextColorList = gh.getShiftedColors(nextColorList)
        cycleLoop()
        pass
