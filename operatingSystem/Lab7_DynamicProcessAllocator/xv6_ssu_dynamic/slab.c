#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"
#include "slab.h"

struct
{
    struct spinlock lock;
    struct slab slab[NSLAB];
} stable;

unsigned int nextPowerOf2Idx(unsigned int n)
{
    unsigned count = 0;

    // First n in the below condition
    // is for the case where n is 0
    if (n < 8)
        return 3;

    n = n - 1;

    while (n != 0)
    {
        n >>= 1;
        count += 1;
    }

    return count;
}

void slabinit()
{
    /* fill in the blank */
    initlock(&stable.lock, "stable");

    acquire(&stable.lock);
    struct slab *slab_ptr;

    for (int i = 0; i < NSLAB; i++)
    {
        slab_ptr = &stable.slab[i];

        // set slab size
        slab_ptr->size = 0x8 << i;

        // first slab page allocation and initialization
        slab_ptr->page[0] = kalloc();
        memset(slab_ptr->page[0], 1, PGSIZE);
        slab_ptr->num_pages = 1;
        slab_ptr->num_objects_per_page = (int)PGSIZE / slab_ptr->size;
        slab_ptr->num_free_objects = slab_ptr->num_objects_per_page;
        slab_ptr->num_used_objects = 0;

        // bitmap allocation and initialization
        slab_ptr->bitmap = kalloc();
        memset(slab_ptr->bitmap, 0, PGSIZE);
    }
    release(&stable.lock);
}

char *kmalloc(int size)
{
    /* fill in the blank */
    // calculate which slab to put
    int where_to_put = 0;
    where_to_put = nextPowerOf2Idx((unsigned int)size) - 3;
    struct slab *slab_ptr;
    slab_ptr = &stable.slab[where_to_put];

    acquire(&stable.lock);
    // check if slab available
    if (slab_ptr->num_free_objects == 0) // check free object exists
    {
        // no free objects
        if (slab_ptr->num_pages <= MAX_PAGES_PER_SLAB) // check if new page allocation available
        {
            // available
            slab_ptr->page[slab_ptr->num_pages] = kalloc();
            slab_ptr->num_pages++;
            slab_ptr->num_free_objects += slab_ptr->num_objects_per_page;
        }
        else
        {
            //not available
            cprintf("page allocation limit\n");

            return 0;
        }
    }

    // search bitmap for free space
    int free_slab_idx = 0;               // free slab index from bitmap
    char *bitmap_ptr = slab_ptr->bitmap; // get bitmap start address
    char *free_space_addr;               // return value;
    unsigned char temp = 0;              // temporary space for bitmap calculation

    for (int i = 0; i * 8 < slab_ptr->num_pages * slab_ptr->num_objects_per_page; i++, bitmap_ptr++)
    {
        if (~((unsigned char)*bitmap_ptr) & 0xFF) // if not FF, there's empty space
        {
            temp = ((unsigned char)*bitmap_ptr);
            while ((temp && 0x01) != 0) // search from front
            {
                temp >>= 1;
                free_slab_idx++;
            }

            // check bitmap to 1 and return allocated space
            *bitmap_ptr = ((unsigned char)*bitmap_ptr) | (0x01 << free_slab_idx);
            free_space_addr = slab_ptr->page[(i * 8 + free_slab_idx) / slab_ptr->num_objects_per_page];
            free_space_addr += (((i * 8 + free_slab_idx) % (slab_ptr->num_objects_per_page) * slab_ptr->size));
            slab_ptr->num_free_objects--;
            slab_ptr->num_used_objects++;
            release(&stable.lock);

            return free_space_addr;
        }
    }

    release(&stable.lock);

    // something wrong
    cprintf("something wrong\n");

    return 0;
}

void kmfree(char *addr, int size)
{
    /* fill in the blank */
    // calculate which slab to free
    int where_to_free = 0;
    where_to_free = nextPowerOf2Idx((unsigned int)size) - 3;
    struct slab *slab_ptr;
    slab_ptr = &stable.slab[where_to_free];

    acquire(&stable.lock);
    // calculate index from addr
    char *ptr;
    char *bitmap_ptr = slab_ptr->bitmap; // get bitmap start address

    for (int i = 0; i * 8 < slab_ptr->num_pages * slab_ptr->num_objects_per_page; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            ptr = slab_ptr->page[(i * 8 + j) / slab_ptr->num_objects_per_page];
            ptr += (((i * 8 + j) % (slab_ptr->num_objects_per_page) * slab_ptr->size));

            if (ptr == addr)
            {
                bitmap_ptr += i;
                *bitmap_ptr = (((unsigned char)*bitmap_ptr) & ~(1 << j));
                slab_ptr->num_free_objects++;
                slab_ptr->num_used_objects--;
                memset(addr, 1, slab_ptr->size);
                release(&stable.lock);

                return;
            }
        }
    }

    release(&stable.lock);

    // something wrong
    cprintf("something wrong\n");
    cprintf("input addr : %p\n", addr);
}

void bitmapdump()
{
    struct slab *s;
    for (s = stable.slab; s < &stable.slab[NSLAB]; s++)
    {
        char *ptr = s->bitmap;
        cprintf("size : %d\n", s->size);

        for (int i = 0; i < PGSIZE; i++)
        {
            if (((unsigned char)*ptr) != 0)
                cprintf("%x ", (unsigned char)*ptr);
            else
                cprintf("00 ");
            ptr++;
        }
        cprintf("\n");
    }
}

void slabdump()
{
    cprintf("__slabdump__\n");

    struct slab *s;

    cprintf("size\tnum_pages\tused_objects\tfree_objects\n");

    for (s = stable.slab; s < &stable.slab[NSLAB]; s++)
    {
        cprintf("%d\t%d\t\t%d\t\t%d\n",
                s->size, s->num_pages, s->num_used_objects, s->num_free_objects);
    }

    //bitmapdump();
}
