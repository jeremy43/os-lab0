#include "../include/game.h"
#include "../include/string.h"
#include "../include/device/video.h"
#define wall 1
#define door 2
#define monster 3
#define yaoshi 4
#define up 5
#define down 6
#define bottle 7
/* 绘制屏幕上的内容。
 * 注意程序在绘图之前调用了prepare_buffer，结束前调用了display_buffer。
 * prepare_buffer会准备一个空白的绘图缓冲区，display_buffer则会将缓冲区绘制到屏幕上，
 * draw_pixel或draw_string绘制的内容将保存在缓冲区内(暂时不会显示在屏幕上)，调用
 * display_buffer后才会显示。
*/
void
redraw_screen() {
     //	fly_t it;
	//const char *hit, *miss;
       // const char * miss;
	snake_t it;
	//food_t ifood;
	prepare_buffer(); /* 准备缓冲区 */

	/* 绘制每个字符 */
/*	for (it = characters(); it != NULL; it = it->_next) {
		static char buf[2];
		buf[0] = it->text + 'A'; buf[1] = 0;
		draw_string(buf, it->x, it->y, 15);
	}
        */
	int i,j;
	int FLOOR=get_floor();
//	printk("FLOOR %d\n",FLOOR);
	for (i=0;i<12;++i)
        for (j=0;j<12;++j)
	{
		switch(map[FLOOR][i][j])
		{
 		    case wall:	draw_aixin(i*16,j*16,60); break;
		    case door:  draw_door(i*16,j*16,40);break;
                    case monster:draw_monster(i*16,j*16,30);break;
		    case yaoshi: draw_yaoshi(i*16,j*16);break;
		    case up: draw_up(i*16,j*16);break;
	            case down: draw_down(i*16,j*16);break;
	            case bottle:draw_bottle(i*16,j*16);break;
		}
        }
	for (it=characters();it!=NULL;it=it->_next)
	{
                
	//	printk("$$ x=%d y=%d\n",it->x,it->y);
		draw_people((it->x)*16,(it->y)*16,50);
	}
	
	/* 绘制命中数、miss数、最后一次按键扫描码和fps */
//	draw_string(itoa(last_key_code()), SCR_HEIGHT - 8, 0, 48);
//	hit = itoa(get_hit());
	//draw_string(hit, 0, SCR_WIDTH - strlen(hit) * 8, 10);
//	miss = itoa(get_miss());
//	draw_string(miss, SCR_HEIGHT - 8, SCR_WIDTH - strlen(miss) * 8, 12);
//	draw_string(itoa(get_fps()), 0, 0, 14);
//	draw_string("FPS", 0, strlen(itoa(get_fps())) * 8, 14);
       draw_string("KEY",40,230+strlen("KEY"),80);
	draw_string(itoa(key_number),40,270,84);
	draw_string("BLOOD",60,230+strlen("key"),80);
	draw_string(itoa(blood),60,280,84);
	draw_string("FLOOR",80,230+strlen("KEY"),80);
        // draw_string(blood,110,250,60);
        draw_string(itoa(get_floor()),80,280,84);
//	draw_aixin(14,15,60);
	
	//draw_rgb();
	display_buffer(); /* 绘制缓冲区 */
}

