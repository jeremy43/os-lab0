#include "include/x86/x86.h"
#include "include/game.h"
#include "include/string.h"
#include "include/assert.h"
#include "include/device/timer.h"

#define FPS 5
#define CHARACTER_PER_SECOND 5
#define UPDATE_PER_SECOND 10

extern uint8_t *vmem;
void realdraw(uint8_t *);
int time(void);
/*volatile int tick = 0;

void
timer_event(void) {
	tick ++;
}
int get_time(void)
{
	return tick;
}
*/
static int real_fps;
void
set_fps(int value) {
	real_fps = value;
}
int
get_fps() {
	return real_fps;
}
/* 游戏主循环。
 * 在初始化工作结束后，main函数就跳转到主循环执行。
 * 在主循环执行期间随时会插入异步的中断。时钟中断最终调用timer_event，
 * 键盘中断最终调用keyboard_event。中断处理完成后将返回主循环原位置继续执行。
 *
 * tick是时钟中断中维护的信号，数值含义是“系统到当前时刻已经发生过的时钟中断数”
 * HZ是时钟控制器硬件每秒产生的中断数，在include/device/timer.h中定义
 * now是主循环已经正确处理的时钟中断数，即游戏已经处理到的物理时间点
 *
 * 由于qemu-kvm在访问内存映射IO区域时每次都会产生陷入，在30FPS时，
 * 对显存区域每秒会产生30*320*200/4次陷入，从而消耗过多时间导致跳帧的产生(实际FPS<30)。
 * 在CFLAGS中增加-DSLOW可以在此情况下提升FPS。如果FPS仍太小，可以尝试
 * -DTOOSLOW，此时将会采用隔行扫描的方式更新屏幕(可能会降低显示效果)。
 * 这些机制的实现在device/video.c中。
 * */
void
main_loop(void) {
	printf("Jery!");

	printf("))\n");
	int now = 0, target;
	int num_draw = 0;
	bool redraw;
	int tick;
	//food_t ifood;
	init_map();
        creat_new_snake();
//	 creat_new_food();
	enable_interrupt();
	while (TRUE) {
		wait_for_interrupt();
		disable_interrupt();
		tick=time();
		if (now == tick) {
			enable_interrupt();
			continue;
		}
//		assert(now < tick);
		target = tick; /* now总是小于tick，因此我们需要“追赶”当前的时间 */
		enable_interrupt();

		redraw = FALSE;
	
		update_keypress();
		printf("RR\n");
//		ifood=icharacters();
  //              if(!ifood->exist) creat_new_food();
		/* 依次模拟已经错过的时钟中断。一次主循环如果执行时间长，期间可能到来多次时钟中断，
		 * 从而主循环中维护的时钟可能与实际时钟相差较多。为了维持游戏的正常运行，必须补上
		 * 期间错过的每一帧游戏逻辑。 */
		while (now < target) { 
			/* 每隔一定时间产生一个新的字符 */
		/*	if (now % (HZ / CHARACTER_PER_SECOND) == 0) {
				create_new_letter();
			}*/
		       
			/* 每隔一定时间更新屏幕上字符的位置 */
			if (now % (HZ / UPDATE_PER_SECOND) == 0) {
				update_letter_pos();
			} 
	
			/* 每隔一定时间需要刷新屏幕。注意到这里实现了“跳帧”的机制：假设
			 *   HZ = 1000, FPS = 100, now = 10, target = 1000
			 * 即我们要模拟990个时钟中断之间发生的事件，其中包含了9次屏幕更新，
			 * 但redraw flag只被置一次。 */
			if (now % (HZ / FPS) == 0) {
				redraw = TRUE;
			}
			/* 更新fps统计信息 */
			if (now % (HZ / 2) == 0) {
				int now_fps = num_draw * 2 + 1;
				if (now_fps > FPS) now_fps = FPS;
				set_fps(now_fps);
				num_draw = 0;
			}
			now ++;
		}
	//	printk("now= %d  target= %d \n",now,target);
     		if (redraw) { /* 当需要重新绘图时重绘 */
			num_draw ++;
			redraw_screen();
      //           printk("@@\n");
		}
		realdraw(vmem);
	}
}
