# -*- coding:UTF-8 -*-

#--------------Driver Library-----------------#
import RPi.GPIO as GPIO
import .OLED_Driver as OLED

from .PrintDetects import print_detects



#----------------------MAIN-------------------------#
try:

    def main():
    
        #-------------OLED Init------------#
        OLED.Device_Init()

        #-------------Draw Pictures------------#
        items = []
        items.append((0, "トールくん(赤)", [10, 0, 200, 150]))
        items.append((1, "トールくん(黄)", [50, 20, 180, 100]))
        items.append((8, "赤べこ", [11, 4, 240, 154]))

        print_detects(items)
        OLED.Delay(2000)

    if __name__ == '__main__':
        main()

except:
    print("except")
    OLED.Clear_Screen()
    GPIO.cleanup()


