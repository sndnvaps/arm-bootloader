arm-none-eabi-objdump -S a.out >toto.s
arm-none-eabi-objdump -S a.out
arm-none-eabi-ld -Tldfile serial.o
arm-none-eabi-gcc -c serial.c
vi ldfile +3
ascii -x

openocd -f /comelec/softs/opt/openocd/enst/FS44B0XII.cfg
telnet localhost 4444
halt ; flash write_image erase /stud/users/promo10/moussu/Desktop/serial/arm 0 elf ; reg pc 0 ; resume ;
halt ; reg pc 0 ; resume ;
reset init
armv4_5 reg
flash erase_sector 0 0 1
mdw 0 1000
minicom
reg pc 0x0C000000
reg pc 0
resume
arm-none-eabi-objcopy -O binary arm arm.bin
