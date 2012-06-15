CC=arm-none-eabi-gcc 
OBJDUMP=arm-none-eabi-objdump
LD=arm-none-eabi-ld
DEBUG=arm-none-eabi-insight
OBJCOPY=arm-none-eabi-objcopy -O binary
LIBC=$(shell $(CC) -print-libgcc-file-name)

CFLAGS=-Wall -g -fomit-frame-pointer -Os

TARGET=linux
SRC=$(wildcard *.c)

LDFILE=ldfile
CRT0=crt0
OBJS=$(CRT0).o $(SRC:.c=.o)

.PHONY : all clean tidy lst bin

all : $(TARGET) lst

lst : $(TARGET).lst

bin : $(TARGET).bin

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


clean : tidy
	@rm -f *.o $(TARGET) $(TARGET).map $(TARGET).lst $(TARGET).bin

tidy:
	@rm -f *~
