StoreAnalog: read values from analog pin and store the value into the SD card.

ReadfromSD: read digital values from SD card and use SPI interface to transfer this data from Arduino to DAC chip. The chip then output the signal wave to the speaker to play back.

adc_read: read from "adc.txt" and save reversed results in "output.txt". Make sure that the text files are under the same folder with the code.

Master files pull up synchronization Pin so that all microcontrollers start receive or send signals at the same time. 
Teensy 4.1 are physically connected by wire. Share the same GND.
