#include <asm/e820.h>
#include <asm/bootparam.h>

#include <os/types.h>
#include <os/kernel.h>

struct e820map e820;

static void early_panic(char *msg) {
	early_printk(msg);
	early_printk("\npanic in e820");
    __asm__("hlt");
}

static void e820_print_map(char *who) {
	int i;

	for (i = 0; i < e820.nr_map; i++) {
		printk(KERN_INFO " %s: %016Lx - %016Lx ", who,
		       (unsigned long long) e820.map[i].addr,
		       (unsigned long long)
		       (e820.map[i].addr + e820.map[i].size));
		switch (e820.map[i].type) {
		case E820_RAM:
			printk(KERN_CONT "(usable)\n");
			break;
		case E820_RESERVED:
			printk(KERN_CONT "(reserved)\n");
			break;
		case E820_ACPI:
			printk(KERN_CONT "(ACPI data)\n");
			break;
		case E820_NVS:
			printk(KERN_CONT "(ACPI NVS)\n");
			break;
		default:
			printk(KERN_CONT "type %u\n", e820.map[i].type);
			break;
		}
	}
}

static int copy_e820_map(struct e820entry *biosmap, int nr_map) {
    if (nr_map < 2) {
        return -1;
    }

    do {
        uint64_t start = biosmap->addr;
		uint64_t size = biosmap->size;
		uint64_t end = start + size;
		uint32_t type = biosmap->type;

		/* Overflow in 64 bits? Ignore the memory map. */
		if (start > end)
			return -1;
        int x = e820.nr_map;

        if (x == E820MAX) {
            printk(KERN_ERR "Ooops! Too many entries in the memory map!\n");
            return;
        }

        e820.map[x].addr = start;
        e820.map[x].size = size;
        e820.map[x].type = type;
        e820.nr_map++;
    } while (biosmap++, --nr_map);
    return 0;
}

char *machine_specific_memory_setup() {
    char *who = "BIOS-e820";

    // e820 数组中，可能存在某些内存段重复，需要将重复的部分分离；
    // 而我们暂不处理这种情况，认为 multiboot2 中的内存段不存在重复。
    // 复制 e820 信息
    if (copy_e820_map(boot_params.e820_map, boot_params.e820_entries) < 0) {
        early_panic("Cannot find a valid memory map");
    }
    printk(KERN_INFO "BIOS-provided physical RAM map:\n");

    e820_print_map(who);
    return who;
}