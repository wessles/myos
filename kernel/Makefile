
ARCH = x86_64
TARGET = kernel.elf
SRCS = kernel.c

CC = $(ARCH)-elf-gcc
LD = $(ARCH)-elf-ld

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) -ffreestanding -fno-stack-protector -fno-stack-check -D__$(ARCH)__ -I. -I./uefi -c $< -o kernel.o
	$(LD) -nostdlib -z max-page-size=0x1000 -Ttext=0x01000000 kernel.o -o $(TARGET)

clean:
	rm -f *.o $(TARGET)