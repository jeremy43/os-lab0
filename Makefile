CC = gcc
LD = ld
CFLAGS = -m32 -c -static  -MD -std=gnu89 -ggdb \
		 -fno-builtin -fno-stack-protector -fno-omit-frame-pointer \
		 -Wall -Werror  
		 
ASFLAGS = -m32 -MD
LDFLAGS = -melf_i386
QEMU = qemu-system-i386
GDB = gdb
KERNEL=k.dat
GDB_OPTIONS = -ex "target remote 127.0.0.1:1234"
GDB_OPTIONS += -ex "symbol $(KERNEL)"

CFILES = $(shell find kernel/ app/ -name "*.c")
SFILES = $(shell find kernel/ app/ -name "*.S")
OBJS = $(CFILES:.c=.o) $(SFILES:.S=.o)

GAME_CFILES = $(shell find app/ -name "*.c")
GAME_SFILES = $(shell find app/ -name "*.S")
GAME_OBJS = $(GAME_CFILES:.c=.o) $(GAME_SFILES:.S=.o)

KERNEL_CFILES = $(shell find kernel/ -name "*.c")
KERNEL_SFILES = $(shell find kernel/ -name "*.S")
K_OBJS = $(KERNEL_CFILES:.c=.o) $(KERNEL_SFILES:.S=.o)
game.img: game k
	@cd boot; make
	cat boot/bootblock  k.dat game.dat > game.img
game/%.o:game/%.[Sc]
	$(CC) $(CFLAGS) -I game/include/ $< -o $@
kernel/%.o:kernel/%.[Sc]
	$(CC) $(CFLAGS) -c -I kernel/include/ $< -o $@


game: $(GAME_OBJS)
	$(LD) $(LDFLAGS) -e main_loop -Ttext 0x02000000 -o game.dat $(GAME_OBJS)

k: $(K_OBJS)
	$(LD) $(LDFLAGS) -e kernel_init -Ttext 0x00100000 -o k.dat $(K_OBJS)
#	$(LD) $(LDFLAGS) -T kernel/kernel.ld  -o k.dat $(K_OBJS)
	@perl ./gen.pl  k.dat
#@perl ./jbtx.pl k.dat
#-include $(patsubst %.o, %.d, $(OBJS))

.PHONY: play clean debug gdb

play: game.img
	$(QEMU)  -serial  stdio game.img #-d int
debug: game.img
	$(QEMU) -d int -serial stdio -s -S game.img
gdb:
	$(GDB) $(GDB_OPTIONS)

clean:
	@cd boot; make clean
	rm -f k.dat game.dat game.img $(OBJS) $(OBJS:.o=.d)
