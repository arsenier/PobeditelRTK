simavr_debug: simavr_build
	simavr -m atmega2560 -f 16000000L ./.pio/build/megaatmega2560/firmware.elf

simavr_build:
	pio debug

objdump: build
	avr-objdump -g -d .pio/build/megaatmega2560/firmware.elf > objdump.s

build:
	pio run

clean:
	rm -rf .pio/build