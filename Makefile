MCU = atmega16
OBJECTS =main.out serial/serial.out i2c/i2c.out rtc/rtc.out timelibrary/timelibrary.out display/display.out interrupthandlers/interrupthandlers.out
all: upload.hex

upload.hex: upload.out
	avr-objcopy -j .text -j .data -O ihex upload.out upload.hex

%.out: %.c
	avr-gcc -mmcu=atmega16 -c -Os $< -o $@

upload.out: $(OBJECTS)
	avr-gcc -mmcu=atmega16 -Os -o upload.out $(OBJECTS)

flash: upload.hex
	sudo avrdude -c usbasp -p m16 -P USBasp -U flash:w:upload.hex

clean:
	rm -f **/*.out *.out *.hex
