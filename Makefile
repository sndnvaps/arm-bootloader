CC=arm-none-eabi-gcc 
OBJDUMP=arm-none-eabi-objdump
LD=arm-none-eabi-ld
DEBUG=arm-none-eabi-insight
OBJCOPY=arm-none-eabi-objcopy -O binary
MAKE=make
OS_BIN=linux.bin
OS_PATH=linux/
LIBC=$(shell $(CC) -print-libgcc-file-name)

CFLAGS=-Wall -g -fomit-frame-pointer -Os

TARGET=bootloader
SRC=$(wildcard *.c)

LDFILE=ldfile
CRT0=crt0
OBJS=$(CRT0).o $(SRC:.c=.o)

.PHONY : all clean tidy lst bin linux rclean

all : $(TARGET) lst bin linux

lst : $(TARGET).lst

bin : $(TARGET).bin

linux :
	@$(MAKE) -C $(OS_PATH) all
	@$(MAKE) -C $(OS_PATH) bin
	@cp $(OS_PATH)$(OS_BIN) .

debug : $(TARGET)
	$(DEBUG) $(TARGET)

$(TARGET).bin : $(TARGET)
	$(OBJCOPY) $^ $@

$(TARGET).lst : $(TARGET)
	$(OBJDUMP) -d $(TARGET) > $@

$(TARGET) : $(OBJS)
	$(CC) -nostartfiles  -Wl,-T$(LDFILE) -Wl,--print-map -Wl,--cref $^ -o $@ > $@.map

%.o : %.c
	$(CC) $(CFLAGS) -c $<
	
%.o : %.S
	$(CC) $(CFLAGS) -c $<

rclean: clean
	@$(MAKE) -C $(OS_PATH) clean

clean : tidy
	@rm -f *.o $(TARGET) $(TARGET).map $(TARGET).lst $(TARGET).bin $(OS_BIN)

tidy:
	@rm -f *~
