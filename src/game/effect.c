#include "game.h"
#include "common.h"
#include "string.h"
#include "adt/linklist.h"
#include "device/video.h"
#include "x86/x86.h"
# define wall 1 
#define door 2
#define monster 3
//LINKLIST_IMPL(fly, 10000)
LINKLIST_IMPL(snake,1000)
//static fly_t head = NULL;
LINKLIST_IMPL(food,100)
static snake_t shead=NULL;//snake
static food_t fhead=NULL;
static int hit = 0, miss = 0;

int
get_hit(void) {
	return hit;
}

int
get_miss(void) {
	return miss;
}

snake_t
characters(void) {
	return shead;
}
food_t
icharacters(void){
	return fhead;
}
/* 在屏幕上创建一个新的字母 */
/*void
create_new_letter(void) {
	if (head == NULL) {
		head = fly_new(); // 当前没有任何字母，创建新链表
	} else {
		fly_t now = fly_new();
		fly_insert(NULL, head, now); // 插入到链表的头部 
		head = now;
	}
	// 字母、初始位置、掉落速度均为随机设定 
	head->x = 0;
	head->y = rand() % (SCR_WIDTH / 8 - 2) * 8 + 8;
	head->v = (rand() % 1000 / 1000.0 + 1) / 2.0;
	head->text = rand() % 26;
	release_key(head->text); // 清除过往的按键 
}*/
void init_map(void)
{
  int i;
 for (i=0;i<12;++i)
 {
	 map[0][i][0]=1;
	 map[0][i][11]=1;
	 map[0][0][i]=1;
         map[0][11][i]=1;
 }
 for (i=0;i<10;++i) map[0][2][i]=1;
 for (i=2;i<=11;++i) map[0][i][4]=1;
 for (i=1;i<=3;++i)
 {
	 map[0][5][i]=1;
	 map[0][8][i]=1;
	 map[0][6][5+i]=1;
 }
 for(i=3;i<=6;++i)
{
	map[0][i][6]=1;
	map[0][i][9]=1;
}
map[0][9][7]=1;
map[0][10][7]=1;
for (i=5;i<=11;++i)
        map[0][8][i]=1;
	map[0][3][4]=door;
	map[0][5][2]=door;
	map[0][8][2]=door;
	map[0][8][5]=door;
	map[0][8][9]=door;
	map[0][1][2]=monster;
	map[0][1][3]=monster;
	map[0][1][4]=monster;
	map[0][4][2]=monster;
	map[0][5][7]=monster;
	map[0][5][8]=monster;
	
}
void creat_new_snake(void)
{
	if(shead==NULL)
	{
		shead=snake_new();
	}
	shead->x=0;
//	shead->y=10;
	shead->y= rand() % (SCR_WIDTH / 8 - 2) * 10 + 8;
        shead->direction=rand()%4;
        release_snake(shead->direction);	

}
void creat_new_food(void)
{
	if(fhead==NULL)
	{
		fhead=food_new();
	}
	fhead->x=rand()%(SCR_HEIGHT/8)*4+3;
	fhead->y=rand()%(SCR_WIDTH/7+6)*3+9;
	fhead->exist=1;
}
/* 逻辑时钟前进1单位 */
void
update_letter_pos(void) {
/*	fly_t it;
	for (it = head; it != NULL; ) {
		fly_t next = it->_next;
		it->x += it->v; // 根据速度更新位置 
		if (it->x < 0 || it->x + 7.9 > SCR_HEIGHT) {
			if (it->x < 0) hit ++; // 从上部飞出屏幕 
			else miss ++; // 从下部飞出屏幕 
			fly_remove(it);
			fly_free(it);
			if (it == head) head = next; // 更新链表 
		}
		it = next;
	}*/
	snake_t i;
	food_t ifood=fhead;
	i=shead;
	//printk("i->x= %d  i->y= %d i->direction%d\n",i->x,i->y,i->direction);
	for (i=shead;i!=NULL;)
	{
                if(i->x < SCR_HEIGHT-1 && i->y < SCR_WIDTH-1)
		
		{
			if(i->direction==0)
			{
		           	i->y-=1;
			
			}
			else if(i->direction==1)
                             i-> x-=1;
			else if(i->direction==2)
				i->y+=1;
			else i->x +=1;
		}
		else
		{
			i->x=0;
			i->y=0;
		}
		if(i->x==ifood->x&&i->y==ifood->y)
		{

			ifood->exist=0;

		}
		
		i=i->_next;
	}
}

// 更新按键 

void  update_keypress(void) {
//	fly_t it, target = NULL;

        snake_t i=NULL;//snake
	disable_interrupt();
	// 寻找相应键已被按下、最底部且未被击中的字符 
	/*for (it = head; it != NULL; it = it->_next) {
		assert(it->text >= 0 && it->text < 26);
		if (it->v > 0 && it->x > min && query_key(it->text)) {
			min = it->x;
			target = it;
		}
	}
       */

       i=shead;
       int j;
       for (j=0;j<=3;++j)
	       if (query_snake_key(j))
	       {
		       i->direction=j;
		       printk("方向在这看我看我%d\n",i->direction);
		        enable_interrupt();
			release_snake(i->direction);
		       return;
		     
	       }
//	 如果找到则更新相应数据 
/*	if (target != NULL) {
		release_key(target->text);
		target->v = -3; // 速度改为向上 
		return TRUE;
	}*/
  	enable_interrupt();

		return;
}

