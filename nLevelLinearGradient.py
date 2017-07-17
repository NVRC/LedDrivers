#!/usr/bin/python
"""
Requires Python 2.7
"""
import sys
import gradientHelpers as gh
from dotstar import Adafruit_DotStar


CONST_NUMLEDS = 60
#Init default brightness
BRIGHTNESS_CONSTANT = 255

def dotstarlib_correction(color):
    """
    The Adafruit_DotStar library
    """
    def swap(s, i, j):
        lst = list(s);
        lst[i], lst[j] = lst[j], lst[i]
        return ''.join(lst)

    corrected_color = swap(color,1,3)
    return swap(corrected_color,2,4)




def led_output(color_list):
    for i in range(0,CONST_NUMLEDS):
        color = color_list[i][1:7]
        strip.setPixelColor(i,int(color,16))
    strip.show()


def linear_gradient(start_hex, finish_hex, number_of_leds):
    """
    Builds
    """
    start = gh.hex_to_rgb(start_hex)
    finish = gh.hex_to_rgb(finish_hex)

    hex_list = []
    hex_list.append(dotstarlib_correction(start_hex))

    for index in range(1, number_of_leds):
        corrected_vector = [
            int(start[j] + (float(index)/(number_of_leds-1))*
            (finish[j]-start[j]))
            for j in range(3)
        ]

        hex_color = gh.rgb_to_hex(corrected_vector[0],
            corrected_vector[1], corrected_vector[2])
        hex_color_corrected = dotstarlib_correction(hex_color)

        hex_list.append(hex_color_corrected)

    return hex_list

def polylinear_gradient(colors, number_of_leds):

    try:
        n_out = int(float(number_of_leds)/(len(colors)-1))

    except ZeroDivisionError:

        return linear_gradient(colors[0],colors[0],CONST_NUMLEDS)

    gradient_dict = linear_gradient(colors[0],colors[1],n_out)

    if len(colors) > 1:
        for index in range(1,len(colors)-1):
            gradient_dict += linear_gradient(colors[index],
                colors[index+1],n_out)

    return gradient_dict



strip = Adafruit_DotStar(CONST_NUMLEDS)
strip.begin()

list_len = len(sys.argv)

colors = []
for index in range(list_len-2):
    colors.append("%s%s" % ('#',sys.argv[index+1]))

BRIGHTNESS_CONSTANT = int(sys.argv[list_len-1])

strip.setBrightness(BRIGHTNESS_CONSTANT)

hex_color_list = polylinear_gradient(colors,CONST_NUMLEDS)

led_output(hex_color_list)
