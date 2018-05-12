from collections import deque
import nLevelLinearGradient as nllg

def hex_to_rgb(value):
    """Return (red, green, blue) for the color given as #rrggbb."""
    value = value.lstrip('#')
    lv = len(value)
    return tuple(int(value[i:i + lv // 3], 16) for i in range(0, lv, lv // 3))

def rgb_to_hex(red, green, blue):
    """Return color as #rrggbb for the given color values."""
    return '#%02x%02x%02x' % (red, green, blue)

def rgb_to_hex_list(colors):
    hex_list = []
    for color in colors:
        r = color[0]
        g = color[1]
        b = color[2]
        hex_list.append(rgb_to_hex(r,g,b))

    return hex_list

def hex_to_rgb_list(colors):
    rgb_list = []
    for color in colors:
        rgb_list.append(hex_to_rgb(color))

    return rgb_list

def getShiftedColors(colors):
    return rotate(colors, 1)

def getCrossColors(colors, period):
    temp = nllg.linear_gradient(colors[59],colors[0],60)
    columns = period*60
    colorMatrix = [columns][60]
    for i in range(0,60):
        temp1 = colors[i]
        temp2 = temp[i]
        cross = nllg.linear_gradient(temp1, temp2,columns)
        for x in range(0,columns):
            colorMatrix[x][i] = cross[x]
    return colorMatrix




def rotate(l, n):
    return l[n:] + l[:n]
