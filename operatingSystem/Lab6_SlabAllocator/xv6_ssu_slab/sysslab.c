#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"
#include "slab.h"
#include "x86.h"

#define TEST_ALLOC_MIN_SIZE 8
#define TEST_ALLOC_MAX_SIZE 32
#define NTEST 1000

int slabtest(void)
{
	//cprintf("__slabtest__\n");

	int x = TEST_ALLOC_MIN_SIZE;
	char *t[NTEST];
	int i;

	//cprintf("Initial state ...\n");
	slabdump();

	while (x <= TEST_ALLOC_MAX_SIZE)
	{
		//cprintf("Allocate %dB memory ...\n", x-1);
		for (i = 0; i < NTEST; i++)
		{
			t[i] = (char *)kmalloc(x - 1);
		}
		slabdump();

		//cprintf("Free %dB memory ...\n", x-1);
		for (i = 0; i < NTEST; i++)
		{
			kmfree((char *)t[i], x - 1);
		}
		slabdump();
		x <<= 1;
	}
	return 0;
}

int sys_slabtest(void)
{
	slabtest();
	return 0;
}
