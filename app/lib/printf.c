#include "../include/common.h"

static int vfprintf(void (*printer)(char), const char *ctl, void **args) {
	static char buf[32] = {0};
	char *data = NULL;
	char *print_ptr = NULL;
	unsigned int uinteger = 0;
	int count = 0;

	while (*ctl) {
		if (*ctl != '%') {
			printer(*ctl);
			count ++;
			ctl ++;
			continue;
		}
		switch (*(++ctl)) {
		case 'c':
			printer(*(char *)args);
			count ++;
			args ++;
			break;
		case 's':
			data = *(char **)args;
			while (*data) {
				printer(*data ++);
				count ++;
			}
			args ++;
			break;
		case 'd':
			uinteger = *(unsigned int *)args;
			if ((int)uinteger < 0) {
				printer('-');
				count ++;
				uinteger = (~uinteger) + 1;
			}
			print_ptr = buf + sizeof(buf) - 1;
			do {
				*--print_ptr = '0' + uinteger % 10;
			} while (uinteger /= 10);
			while (*print_ptr) {
				printer(*print_ptr ++);
				count ++;
			}
			args ++;
			break;
		case 'x':
			uinteger = *(uint32_t*)args;
			print_ptr = buf + sizeof(buf) - 1;
			do {
				*--print_ptr = (uinteger % 16 < 10) ?
							'0' + uinteger % 16 : 'a' + uinteger % 16 - 10;
			} while (uinteger >>= 4);
			while (*print_ptr) {
				printer(*print_ptr ++);
				count ++;
			}
			args ++;
			break;
		default:
			printer(*ctl);
			count ++;
		}
		ctl ++;
	}
	return count;
}

char buf[256];
int len;

void sprint(char ch) {
	buf[len ++] = ch;
}

void write(char *, int);

void __attribute__((__noinline__)) 
printf(const char *ctl, ...) {
	len = 0;
	void **args = (void **)&ctl + 1;
	vfprintf(sprint, ctl, args);
	write(buf, len);
}
