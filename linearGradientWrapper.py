import nLevelLinearGradient as lg
import gradientHelpers as gh
import threading
from appJar import gui

_list_global_var = []
_color_global_var = []
_brightness_global_var = ['255']

TIMER_CONSTANT = 1

def addRGB(r,g,b):
    hex = gh.rgb_to_hex(r,g,b)
    _list_global_var.append(hex[1:])
    pass

def addHEX(hex):
    _list_global_var.append(hex)
    pass

def setBrightness(brightness):
    _brightness_global_var = brightness
    pass

def display():
    list = _list_global_var
    list.append(_brightness_global_var)
    lg.display(self.getArgs())
    pass

def getArgs():
    list = _list_global_var
    return list.append(_brightness_global_var)


def cycle():
    _color_global_var = lg.getColorList(getArgs)
    threading.Timer(TIMER_CONSTANT,cycleLoop).start
    pass

def cycleLoop():
    lg.led_output(_color_global_var)
    _color_global_var = gh.getShiftedColors(_color_global_var)
    self.cycleLoop()
    pass
