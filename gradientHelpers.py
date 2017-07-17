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
