#include "game.h"
#include "string.h"
#include "device/video.h"
#define wall 1
#define door 2
#define monster 3
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
        //const char * miss;
	snake_t it;
	food_t ifood;
	prepare_buffer(); /* 准备缓冲区 */

	/* 绘制每个字符 */
/*	for (it = characters(); it != NULL; it = it->_next) {
		static char buf[2];
		buf[0] = it->text + 'A'; buf[1] = 0;
		draw_string(buf, it->x, it->y, 15);
	}
        */
	int i,j;
	for (i=0;i<12;++i)
        for (j=0;j<12;++j)
		switch(map[0][i][j])
		{
		    case wall:	draw_aixin(i*16,j*16,60); break;
		    case door:  draw_door(i*16,j*16,40);break;
                    case monster:draw_monster(i*16,j*16,30);break;
		}

	for (it=characters();it!=NULL;it=it->_next)
	{
            static char buf[1];
	    buf[0]='X';
	    draw_string(buf,it->x,it->y,15);
	}
	for (ifood=icharacters();ifood!=NULL;ifood=ifood->_next)
	{
		static char buf2[1];
		buf2[0]='A';
		draw_string(buf2,ifood->x,ifood->y,15);
	}
	/* 绘制命中数、miss数、最后一次按键扫描码和fps */
//	draw_string(itoa(last_key_code()), SCR_HEIGHT - 8, 0, 48);
//	hit = itoa(get_hit());
	//draw_string(hit, 0, SCR_WIDTH - strlen(hit) * 8, 10);
//	miss = itoa(get_miss());
//	draw_string(miss, SCR_HEIGHT - 8, SCR_WIDTH - strlen(miss) * 8, 12);
//	draw_string(itoa(get_fps()), 0, 0, 14);
//	draw_string("FPS", 0, strlen(itoa(get_fps())) * 8, 14);
        //draw_string("JEREMY",8,strlen("JEREMY"),50);
        draw_string("wayne",8,strlen("wayne")*8,50);
	draw_aixin(14,15,60);
	display_buffer(); /* 绘制缓冲区 */
}

