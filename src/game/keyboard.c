#include "common.h"
#include "string.h"

/* a-z对应的键盘扫描码 */
/*static int letter_code[] = {
	30, 48, 46, 32, 18, 33, 34, 35, 23, 36,
	37, 38, 50, 49, 24, 25, 16, 19, 31, 20,
	22, 47, 17, 45, 21, 44
};*/
static int snake_code[]=
{ 
	1,14,3,2
};
/* 对应键按下的标志位 */
//static bool letter_pressed[26];
static bool snake_pressed[4];
/*void
press_key(int scan_code) {
	int i;
	for (i = 0; i < 26; i ++) {
		if (letter_code[i] == scan_code) {
			letter_pressed[i] = TRUE;
		}
	}
}*/
void press_snake_key(int scan_code)
{
	int i;
//	printk("scan_code= %d\n",scan_code);
	for (i=0;i<4;++i)
	{
		if(snake_code[i]==scan_code)
		{

			snake_pressed[i]=TRUE;
			printk("**\n");
		}
	}
}
/*void
release_key(int index) {
	assert(0 <= index && index < 26);
	letter_pressed[index] = FALSE;
}*/
void 
release_snake(int index)
{
	assert(0<=index&&index<4);
	snake_pressed[index]=FALSE;//有问题
}
/*bool
query_key(int index) {
	assert(0 <= index && index < 26);
	return letter_pressed[index];
}*/
bool query_snake_key(int index)
{
	assert(0<=index&& index<4);
	return snake_pressed[index];
}
/* key_code保存了上一次键盘事件中的扫描码 */
static volatile int key_code = 0;

int last_key_code(void) {
	return key_code;
}

void
keyboard_event(int code) {
	key_code = code;
       press_snake_key(code);
}

