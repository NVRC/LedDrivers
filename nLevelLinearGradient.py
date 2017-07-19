#!/usr/bin/python
"""
Requires Python 2.7 due to LED strip C library dependancies

Can be executed via :

    1) Terminal
        python nLevelLinearGradient hex_color_one hex_color_two brightness
        python nLevelLinearGradient 000000 ffffff 255

    2) linearGradientWrapper
"""
import gradientHelpers as gh
from dotstar import Adafruit_DotStar


CONST_NUMLEDS = 60
#Init default brightness
_brightness_global_var = 255

strip = Adafruit_DotStar(CONST_NUMLEDS)
strip.begin()

def dotstarlib_correction(color):
    """
    The Adafruit_DotStar library for some , I'm sure, mundanely disinteresting
    reason has swapped the R and G values when invoking "strip.setPixelColor()".

    This simply swaps the appropriate segments. #RRGGBB -> #GGRRBB
    """
    def swap(s, i, j):
        lst = list(s);
        lst[i], lst[j] = lst[j], lst[i]
        return ''.join(lst)

    corrected_color = swap(color,1,3)
    return swap(corrected_color,2,4)

def led_output(color_list):
    """ Sets every LED in the strip to it's list mapped color.
    """
    for i in range(0,CONST_NUMLEDS):
        color = color_list[i][1:7]
        strip.setPixelColor(i,int(color,16))
    strip.show()


def linear_gradient(start_hex, finish_hex, number_of_leds):
    """ Compiles a list of the linear plot between the starting RGB value
            and the final RGB value.

        Keyword arguments:
        start_hex       -- The first plotted point
        finish_hex      -- The last plotted point
        number_of_leds  -- Dictates the number of points mapped along the plot.
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
    """ Pairs each color into a linear gradient and concatenates each segment.
    """
    try:
        n_out = int(float(number_of_leds)/(len(colors)-1))

    except ZeroDivisionError:
        #If there is only one color, the entire strip is set that color
        return linear_gradient(colors[0],colors[0],CONST_NUMLEDS)

    #Establish the first color segment
    gradient_dict = linear_gradient(colors[0],colors[1],n_out)

    if len(colors) > 1:
        for index in range(1,len(colors)-1):
            gradient_dict += linear_gradient(colors[index],
                colors[index+1],n_out)

    return gradient_dict

def display(arg):
    led_output(getColorList(arg))

def getColorList(arg):
    #Parse arguments
    list_len = len(arg)
    colors = []
    for index in range(list_len-2):
        colors.append("%s%s" % ('#',arg[index+1]))

    _brightness_global_var = int(arg[list_len-1])
    strip.setBrightness(_brightness_global_var)

    return polylinear_gradient(colors,CONST_NUMLEDS)


if __name__ == '__main__':
    import sys
    display(sys.argv[1:])
