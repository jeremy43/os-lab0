# GNU make手册：http://www.gnu.org/software/make/manual/make.html
# ************ 遇到不明白的地方请google以及阅读手册 *************

# 编译器设定和编译选项
CC = gcc
LD = ld
CFLAGS = -m32 -static -MD -std=gnu89 -ggdb \
		 -fno-builtin -fno-stack-protector -fno-omit-frame-pointer \
		 -Wall -Werror -O0 
ASFLAGS = -m32 -MD
LDFLAGS = -melf_i386 -nostdlib
QEMU = qemu-system-i386

# 编译目标：src目录下的所有.c和.S文件
CFILES = $(shell find kernel/ lib/ game/ -name "*.c")
SFILES = $(shell find kernel/ lib/ game/ -name "*.S")
OBJS = $(CFILES:.c=.o) $(SFILES:.S=.o)


GAME_CFILES = $(shell find game/ -name "*.c")
GAME_SFILES = $(shell find game/ -name "*.S")
GAME_OBJS = $(GAME_CFILES:.c=.o) $(GAME_SFILES:.S=.o)

KERN_CFILES = $(shell find kernel/ -name "*.c")
KERN_SFILES = $(shell find kernel/ -name "*.S")
KERN_OBJS = $(KERN_CFILES:.c=.o) $(KERN_SFILES:.S=.o)

game.img: game kern
	@cd boot; make
	cat boot/bootblock kern.bin game.bin > game.img

game/%.o : game/%.[cS]
	$(CC) $(CFLAGS) -I game/include $< -o $@
	
#lib/%.o : lib/%.[cS]
#	$(CC) $(CFLAGS) -I game/include $< -o $@

kernel/%.o : kernel/%.[cS]
	$(CC) $(CFLAGS) -I kernel/include $< -o $@

game.bin: $(GAME_OBJS)
	$(LD) $(LDFLAGS) -e main_loop -Ttext 0x00100000 -o $@ $^

kern: $(KERN_OBJS)
	$(LD) $(LDFLAGS) -e kern_init -Ttext 0x00100000 -o kern.bin $(kern_OBJS)

#-include $(patsubst %.o, %.d, $(OBJS))

# 定义的一些伪目标
.PHONY: play clean debug

# 如果make play运行失败，请检查qemu的安装
# 也可以修改这里的脚本，用其他类型的模拟器启动game.img
play: game.img
	$(QEMU) -serial stdio game.img
debug: game.img
	$(QEMU) -serial stdio -s -S game.img

# make clean可以清除已生成的文件
clean:
	@cd boot; make clean
	rm -f game game.img $(OBJS) $(OBJS:.o=.d)
