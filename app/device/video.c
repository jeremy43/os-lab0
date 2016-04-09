#include "../include/common.h"
#include "../include/string.h"
#include "../include/device/video.h"
#include "../include/device/font.h"

/* 绘制屏幕的帧缓冲实现。
 * 在某些版本的qemu-kvm上，由于每次访问显存映射区域都会产生一次VM exit，
 * 更新屏幕的速度可能非常缓慢从而引起游戏跳帧。定义宏SLOW以只重绘屏幕变化
 * 的部分；定义宏TOOSLOW在只重绘屏幕变化部分的基础上，隔行更新。
 * TOOSLOW可能会引起视觉效果的损失。 */
#define SLOW

#ifdef TOOSLOW
	#define PARTIAL_UPDATE
	#define INTERLACE
#else
#ifdef SLOW
	#define PARTIAL_UPDATE
#endif
#endif

static uint8_t v_buf[320*200];
uint8_t *vmem = v_buf;
static uint8_t vbuf[SCR_SIZE];
#ifdef PARTIAL_UPDATE
static uint8_t vref[SCR_SIZE];
#endif

void
prepare_buffer(void) {
#ifdef PARTIAL_UPDATE
	memcpy(vref, vbuf, SCR_SIZE);
#endif
	vmem = vbuf;
	memset(vmem, 0, SCR_SIZE);
}

void
display_buffer(void) {
#ifdef PARTIAL_UPDATE
	int i;
#ifdef INTERLACE
	static int update_line = 0;
	update_line ^= 1;
#endif
	uint32_t *buf = (uint32_t*)vbuf;
	uint32_t *ref = (uint32_t*)vref;
	uint32_t *mem = (uint32_t*)VMEM_ADDR;
	vmem = VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i ++) {
#ifdef INTERLACE
		if ((i * 4 / SCR_WIDTH) % 2 == update_line) {
			buf[i] = ref[i];
			continue;
		}
#endif
		if (buf[i] != ref[i]) {
			mem[i] = buf[i];
		}
	}
#else
	vmem = VMEM_ADDR;
	asm volatile ("cld; rep movsl" : : "c"(SCR_SIZE / 4), "S"(vbuf), "D"(vmem));
#endif
}

static inline void
draw_character(char ch, int x, int y, int color) {
	int i, j;
//	assert((ch & 0x80) == 0);
	char *p = font8x8_basic[(int)ch];
	for (i = 0; i < 8; i ++) 
		for (j = 0; j < 8; j ++) 
			if ((p[i] >> j) & 1)
				draw_pixel(x + i, y + j, color);
}

void
draw_string(const char *str, int x, int y, int color) {
	while (*str) {
		draw_character(*str ++, x, y, color);
		if (y + 8 >= SCR_WIDTH) {
			x += 8; y = 0;
		} else {
			y += 8;
		}
	}
}
void 
draw_door(int x,int y,int color)
{
	int i,j;
	color=66;
	int color2=112;
	uint8_t b[16][16];
	for (i=0;i<16;++i)
		for (j=0;j<16;++j)
			b[i][j]=color;
	for (i=1;i<=14;++i)
	{
		b[2][i]=color2;
		b[14][i]=color2;
        }
	b[2][7]=color;b[2][8]=color;
	b[14][7]=color;b[14][8]=color;
	for (i=2;i<=14;++i)
	{
		b[i][1]=color2;
		b[i][14]=color2;
	}
	for (i=4;i<=6;++i)
	{
		b[5][i]=color2;
		b[9][i]=color2;
		b[5][i+5]=color2;
		b[9][i+5]=color2;
	}
	for (i=2;i<=5;++i)
	{
		b[i][6]=color2;
		b[i][9]=color2;
	}
	for (i=9;i<=14;++i)
	{
		b[i][6]=color2;
		b[i][9]=color2;
	}
	b[7][3]=color2;b[7][12]=color2;
	for (i=0;i<16;++i)
		for (j=0;j<16;++j)
			draw_pixel(x+i,y+j,b[i][j]);
	
	
}
void
draw_aixin(int x,int y,int color)
{
  int i,j;
  for (i=0;i<16;++i)
	  for (j=0;j<16;++j)
		  draw_pixel(x+i,y+j,65);
  for (i=0;i<=7;++i)
	  for (j=0;j<16;++j)
	  {
		  draw_pixel(x+2*i+1,y+j,6);
		  draw_pixel(x+j,y+2*i+1,6);
	  }
  

 /* for (i=0;i<16;++i)
	  for (j=0;j<16;++j)
		  for (k=0;k<7;++k)
			  for (l=0;l<7;++l) draw_pixel(60+7*i+k,185+7*j+l,16*i+j);
*/
}
/*void 
draw_rgb()
{
	int i,j,k,l;
	for (i=1;i<=16;++i)
		for (j=1;j<=16;++j)
		for (k=1;k<=8;++k)
			for (k=
}*/
void
draw_bottle(int x,int y)
{

	int color=80;
	int color2=53;
	uint8_t a[16][16];
	int i,j;
	for (i=0;i<=15;++i)
	for (j=0;j<=15;++j)
		a[i][j]=0;
	for (i=6;i<=9;++i)
		for (j=0;j<=1;++j)
			a[j][i]=color;
	for (i=7;i<=8;++i)
		for (j=2;j<=6;++j) a[j][i]=color;
	for (i=6;i<=9;++i) a[7][i]=color;
	for (i=4;i<=11;++i) a[8][i]=color;
	for (i=3;i<=12;++i) a[9][i]=color;
	for (i=10;i<=12;++i)
		for (j=2;j<=13;++j) a[i][j]=color;
	for (i=13;i<=14;++i)
		for (j=3;j<=12;++j) 
			a[i][j]=color;
	for (i=5;i<=9;++i)
		a[15][i]=color;
	a[1][7]=color2;
	a[1][8]=color2;
	for (i=4;i<=11;++i)
		for (j=11;j<=12;++j)
			a[j][i]=color2;
	for (i=5;i<=10;++i) a[13][i]=color2;
	for (i=6;i<=9;++i) a[14][i]=color2;
	for (i=0;i<=15;++i)
		for (j=0;j<=15;++j)
			draw_pixel(x+i,y+j,a[i][j]);
}
void 
draw_yaoshi(int x,int y)
{
	int color=90;
	int i,j;
	uint8_t a[16][16];
	for (i=0;i<=15;++i)
		for(j=0;j<=15;++j)
			a[i][j]=0;
	a[1][14]=1;a[1][15]=1;
	a[2][14]=1;a[2][15]=1;
	for (i=12;i<=15;++i) a[3][i]=1;
	for (i=12;i<=14;++i) a[4][i]=1;
	a[5][5]=1;
	a[5][6]=1;
	for (i=10;i<=13;++i) a[5][i]=1;
	for (i=3;i<=12;++i) a[6][i]=1;
a[6][8]=0;
	for (i=2;i<=10;++i) 
	{
		a[7][i]=1;
		a[8][i]=1;
	}
	for (i=9;i<=11;++i)
		for (j=1;j<=9;++j) a[i][j]=1;
	for (i=2;i<=8;++i)a[12][i]=1;
	for(i=3;i<=8;++i)a[13][i]=1;
	for (i=4;i<=7;++i)a[14][i]=1;
	for(i=5;i<=6;++i)a[15][i]=1;
	for (i=10;i<=11;++i)
		for (j=4;j<=6;++j)a[i][j]=0;
	a[9][5]=0;
	a[12][5]=0;

	if (a[0][0]) draw_pixel(x,y,color);
	for (i=0;i<16;++i)
		for (j=0;j<16;++j)
			if(a[i][j]) draw_pixel(x+i,y+j,color);
} 
void 
draw_up(int x,int y)
{
	int i,j;
	uint8_t a[16][16];
	for (i=0;i<=15;++i)
		for (j=0;j<=15;++j)a[i][j]=0;
	for(i=11;i<=15;++i)
		for (j=2;j<=4;++j) a[j][i]=30;
	for (i=7;i<=14;++i)
		for (j=6;j<=8;++j) a[j][i]=30;
	a[8][14]=0;
	for (i=10;i<=12;++i)
		for (j=3;j<=12;++j) a[i][j]=30;
	for (i=14;i<=15;++i)
		for (j=3;j<=11;++j)a[i][j]=30;
	for (i=12;i<=15;++i)
		for (j=12;j<=15;++j)a[i][j]=26;
	for (i=13;i<=15;++i)
		for (j=10;j<=11;++j) a[j][i]=26;
	for (i=14;i<=15;++i)
		for (j=7;j<=8;++j) a[j][i]=26;
	a[5][15]=26;
	a[6][15]=26;
	for (i=0;i<16;++i)
		for (j=0;j<16;++j)
			draw_pixel(x+i,y+j,a[i][j]);
}
void draw_down(int x,int y)
{
	uint8_t a[16][16];
	int i,j;
	int color=60;
	for (i=0;i<16;++i)
		for (j=0;j<16;++j)
			a[i][j]=0;
	for (i=9;i<=15;++i)
		for (j=0;j<=1;++j) a[i][j]=color;
	for (i=7;i<=15;++i)
		for (j=3;j<=4;++j) a[i][j]=color;
	for (i=5;i<=15;++i)
		for (j=6;j<=7;++j) a[i][j]=color;
	for (i=3;i<=15;++i)
		for (j=9;j<=10;++j)
			a[i][j]=color;
	for (i=1;i<=15;++i)
		for (j=12;j<=13;++j)
			a[i][j]=color;
	for (i=0;i<=15;++i)
		for (j=0;j<=15;++j)
			draw_pixel(x+i,y+j,a[i][j]);

}
void 
draw_people(int x,int y,int color)
{
	int i,j;
	uint8_t  a[16][16];
	for (i=0;i<16;++i) 
		for (j=0;j<16;++j) a[i][j]=0;
	for (i=6;i<=9;++i) a[1][i]=6;
	for (i=6;i<=9;++i)
		for (j=2;j<=4;++j)a[i][j]=54;
	for (i=7;i<=8;++i) a[5][i]=90;
	for (i=3;i<=12;++i)
		for (j=6;j<=9;++j) a[j][i]=54;
	for (i=5;i<=10;++i)
		for (j=10;j<=14;++j) a[j][i]=54;
	a[14][4]=0;
	a[13][7]=0;
	a[14][7]=0;
	a[13][10]=0;
	a[9][2]=90;
	a[10][2]=90;
	a[10][3]=90;
	a[9][13]=90;
	a[10][12]=90;
	a[10][13]=90;
	for (i=0;i<16;++i)
       for (j=0;j<16;++j)
	       draw_pixel(x+i,y+j,a[i][j]);
}
void 
draw_monster(int x,int y,int color)
{//color 100
   int i,j;
   color=96;
   int color2=15;
   uint8_t a[16][16];
   for (i=0;i<16;++i)
	   for (j=0;j<16;++j) a[i][j]=0;
   for (i=2;i<=13;++i)
	   for (j=4;j<=11;++j) a[j][i]=color;
   for (i=5;i<=10;++i)
   {
	   a[2][i]=color;
	   a[14][i]=color;
   }
   for (i=3;i<=12;++i)
   {
	   a[3][i]=color;
	   a[12][i]=color;

   }
   for (i=4;i<=11;++i) a[13][i]=color;
   for (i=6;i<=9;++i)
   
   {
	   a[i][1]=color;
	   a[i][14]=color;
  }
   for (i=3;i<=6;++i)
{
           a[7][i]=color2;
           a[8][i]=color2;
           a[7][5+i]=color2;
           a[8][5+i]=color2;	   
}
for (i=4;i<=5;++i)
for (j=7;j<=8;++j) 
{
	a[j][i]=0;
	a[j][i+5]=0;
}
for (i=0;i<16;++i)
for (j=0;j<16;++j) draw_pixel(x+i,y+j,a[i][j]);
  /* for (i=2;i<=5;++i)
   {
	   draw_pixel(x,y+i,color);
	   draw_pixel(x,y+8+i,color);
   }
   for (i=1;i<=6;++i)
  {
	 draw_pixel(x+1,y+i,color);
	 draw_pixel(x+1,y+8,color);
 }
  for (i=1;i<=14;++i)
  {
 	  draw_pixel(x+2,y+i,color);
	  draw_pixel(x+9,y+i,color);
	  draw_pixel(x+10,y+i,color);
 }
 for (i=3;i<=8;++i)
	for (j=0;j<=15;++j)
	       draw_pixel(x+i,y+j,color);

 for (i=11;i<=14;++i)
 for(j=i-9;j<=24-i;++j) draw_pixel(x+i,y+j,color);
 draw_pixel(x+15,y+7,color);
 draw_pixel(x+15,y+8,color);
*/
}
