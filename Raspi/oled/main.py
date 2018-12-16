# -*- coding:UTF-8 -*-

#--------------Driver Library-----------------#
import RPi.GPIO as GPIO
import OLED_Driver as OLED
#--------------Image Library---------------#
import Image
import ImageDraw
import ImageFont
#-------------Test Display Functions---------------#

import os

font_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'ipaexg.ttf')

line_pos = [
    (0, 2),
    (0, 14),
    (0, 26),
    (0, 38),
]
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
    font = ImageFont.truetype(font_path, 24)
    for i, (class_index, class_label, score) in enumerate(items):
        pos = line_pos[i]
        color = class_colors[class_index]
        draw.text(pos, '{}:{} ({})'.format(class_index, class_label, score), fill = color, font = font)
    OLED.Display_Image(image)



#----------------------MAIN-------------------------#
try:

    def main():
    
        #-------------OLED Init------------#
        OLED.Device_Init()

        #-------------Draw Pictures------------#
        items = []
        items.append((0, "トールくん(赤)", 0.90))
        items.append((1, "トールくん(黄)", 0.89))
        items.append((8, "赤べこ", 0.60))

        print_detects(items)
        OLED.Delay(2000)


    if __name__ == '__main__':
        main()

except:
    print("except")
    OLED.Clear_Screen()
    GPIO.cleanup()


