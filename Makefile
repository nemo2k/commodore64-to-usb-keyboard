MMCU=atmega328p
ARDUINOVERSION=1.8.1
FUSES=-U lfuse:w:0xe2:m -U hfuse:w:0xd1:m -U efuse:w:0xff:m

default:
	avr-gcc -g -Wextra -Llib -Iinclude -Wall -std=gnu++11 -fno-threadsafe-statics -DARDUINO=10608 -DF_CPU=16000000UL -Os -Wl,--gc-sections -ffunction-sections  -fdata-sections -fno-exceptions -mmcu=$(MMCU) -o main.elf main.cpp ProTrinketKeyboardC.c ProTrinketKeyboard.cpp usbdrv_includer.c usbdrvasm_includer.S -larduino-$(ARDUINOVERSION)
	avr-objcopy -O ihex -R .eeprom main.elf firmware.hex

u: upload

upload:
	avrdude -v -c usbtiny -p $(MMCU) -b 57600 -U flash:w:firmware.hex

#f: fuses

#fuses:
#	avrdude -v -c usbtiny -p $(MMCU) -b 57600 $(FUSES)

m: monitor

monitor:
	screen /dev/ttyUSB0 9600

r: restart

restart:
	avrdude -v -c usbtiny -p $(MMCU) -b 57600 -n
s: size

size:
	avr-size firmware.hex
	avr-size main.elf

sd: sizedetail

sizedetail:
	avr-nm --size-sort -Ctd main.elf

all: default
