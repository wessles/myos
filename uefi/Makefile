ARCH = x86_64
TARGET = BOOTx64.EFI
SRCS = $(wildcard *.c)
CFLAGS = -pedantic -Wall -Wextra -O2
USE_GCC = 1

include uefi/Makefile

clean:
	rm -f *.EFI