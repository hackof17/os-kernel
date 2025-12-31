CROSS = aarch64-linux-gnu
CC    = $(CROSS)-gcc
LD    = $(CROSS)-ld

CFLAGS = -ffreestanding -fno-pic -fno-stack-protector \
         -nostdlib -O2 -Wall

OBJS = boot.o kernel.o uart.o vector.o gic.o

all: myos.elf

myos.elf: $(OBJS)
	$(LD) -T linker.ld -o $@ $(OBJS)

boot.o: boot.S
	$(CC) $(CFLAGS) -c $< -o $@

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

uart.o: uart.c
	$(CC) $(CFLAGS) -c $< -o $@
vector.o: vector.S
	$(CC) $(CFLAGS) -c $< -o $@
gic.o: gic.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o myos.elf
