# Project Name
PROJECT = flight_controller

# Microcontroller
MCU = atmega328p

# Compiler and flags
CC = avr-gcc
CFLAGS = -Os -DF_CPU=16000000UL -mmcu=$(MCU)
LDFLAGS = -mmcu=$(MCU)
OBJ2HEX = avr-objcopy

# Source file
SRC = src/$(PROJECT).c
OBJ = $(PROJECT).o
ELF = $(PROJECT).elf
HEX = $(PROJECT).hex

# Programmer and port (adjust as necessary)
PROGRAMMER = arduino
PORT = /dev/ttyACM0
BAUD = 115200

# Targets
all: $(HEX)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(ELF): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

$(HEX): $(ELF)
	$(OBJ2HEX) -O ihex -R .eeprom $< $@

flash: $(HEX)
	sudo avrdude -F -V -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$<:i

clean:
	rm -f $(OBJ) $(ELF) $(HEX)

.PHONY: all clean flash
