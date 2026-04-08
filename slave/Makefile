.PHONY: all clean flash size

# ===== Project =====
TARGET = firmware
MCU    = atmega328p
F_CPU  = 16000000UL

# ===== Tools =====
CC      = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
SIZE    = avr-size

# ===== Serial / Upload =====
PORT       = /dev/cu.usbmodem143101  # ls /dev/cu.*  (update to your UNO port)
BAUD       = 115200
PROGRAMMER = arduino

# ===== Flags =====
CFLAGS = -Os -Wall -Wextra -std=c17 \
         -DF_CPU=$(F_CPU) \
         -mmcu=$(MCU) \
         -g

LDFLAGS = -mmcu=$(MCU)

# ===== Files =====
CFILES = $(wildcard *.c)
HFILES = $(wildcard *.h)
OBJS   = $(CFILES:.c=.o)

# ===== Targets =====
all: $(TARGET).hex size

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c $(HFILES)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

size: $(TARGET).elf
	@echo "=== Memory Usage ==="
	@$(SIZE) --format=avr --mcu=$(MCU) $(TARGET).elf 2>/dev/null || $(SIZE) $(TARGET).elf

flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) \
	           -P $(PORT) -b $(BAUD) \
	           -D \
	           -U flash:w:$<:i

clean:
	rm -f *.o *.elf *.hex
