# -*- coding:UTF-8 -*-

#--------------Driver Library-----------------#
import RPi.GPIO as GPIO
import .OLED_Driver as OLED
#--------------Image Library---------------#
from PIL import Image, ImageFont, ImageDraw
#-------------Test Display Functions---------------#

import os

font_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'ipaexg.ttf')
font_size = 24
line_pos = []
for i in range(2, OLED.SSD1351_HEIGHT - font_size, font_size + 2):
    line_pos.append((0, i))
class_colors = [
    "RED",
    "YELLOW",
    "BLUE",
    "BLUE",
    "BLUE",
    "BLUE",
    "WHITE",
    "CYAN",
    "RED",
]

def print_detects(items: list):
    image = Image.new("RGB", (OLED.SSD1351_WIDTH, OLED.SSD1351_HEIGHT), "BLACK")
    draw = ImageDraw.Draw(image)
    font = ImageFont.truetype(font_path, font_size)
    for i, (class_index, class_label, [left, top, width, height]) in enumerate(items):
        if i >= len(line_pos): break
        pos = line_pos[i]
        color = class_colors[class_index]
        value = '{}:{}   ({}, {}) -> ({}, {})'.format(class_index, class_label, left, top, width, height)
        draw.text(pos, value, fill = color, font = font)
    OLED.Display_Image(image)