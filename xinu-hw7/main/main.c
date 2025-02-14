#include <xinu.h>

extern void testcases(void);

process main(void)
{
	kprintf("Hello Xinu World!\r\n");
	testcases();
    	while (1)
		;
    	return 0;
}

