#include "ata.h"
#include "lib.h"
#include "kstring.h"
#include "printk.h"
#include "types.h"
void read_sectors_ATA_PIO(uint32_t target_address, uint32_t LBA, uint8_t sector_count);
void write_sectors_ATA_PIO(uint32_t LBA, uint8_t sector_count, uint32_t* bytes);

void ata_init() {
    // io_out8(PORT_DISK1_ALT_STA_CTL, 0);

	// while(io_in8(PORT_DISK1_STATUS_CMD) & DISK_STATUS_BUSY);
	// kerr_printf("Read One Sector Starting:%02x\n",io_in8(PORT_DISK1_STATUS_CMD));

    // // io_out8(PORT_DISK1_DEVICE, 0x0);
    // io_out8(PORT_DISK1_ERR_FEATURE, 0);
    // io_out8(PORT_DISK1_SECTION_CNT, 1);
    // io_out8(PORT_DISK1_SECTION_LOW, 0);
    // io_out8(PORT_DISK1_SECTION_MID, 0);
    // io_out8(PORT_DISK1_SECTION_HIGH, 0);

    // io_out8(PORT_DISK1_STATUS_CMD, 0x30);

    // while(!(io_in8(PORT_DISK1_STATUS_CMD) & DISK_STATUS_REQ));
	// kerr_printf("Send CMD:%02x\n",io_in8(PORT_DISK1_STATUS_CMD));

    // unsigned char buf[512];
    // memset(buf, 0xAA, 512);
    // port_outsw(PORT_DISK1_DATA, buf, 256);

    uint32_t bwrite[128];
    memset(bwrite, 0xABCDEF12, 128);
    write_sectors_ATA_PIO(0x0, 1, bwrite);

    uint32_t buf[128];
    read_sectors_ATA_PIO(buf, 0x0, 1);
    for (int i = 0; i < 10; i++) {
        kerr_printf("%x ", buf[i]&0xff);
        kerr_printf("%x ", (buf[i]>>8)&0xff);
    }
    kerr_printf("\n");
}

void ata_handler_read(unsigned long nr, unsigned long r) {
    // struct Disk_Identify_Info info;
    // // kprintf("Origin Serial Number: ");
    // // for (int i = 0; i < 10; i++) {
    // //     kerr_printf("%x%x", ((info.Serial_Number[i]>>8)&0xff), (info.Serial_Number[i] & 0xff));
    // // }
    // // kprintf("\n");
    // port_insw(PORT_DISK1_DATA, &info, 256);
    
    // kprintf("Serial Number: ");
    // for (int i = 0; i < 10; i++) {
    //     kerr_printf("%x%x", ((info.Serial_Number[i]>>8)&0xff), (info.Serial_Number[i] & 0xff));
    // }
    // kprintf("\n");

    // int i = 0;
	// unsigned char a[512];
	// port_insw(PORT_DISK1_DATA, &a, 256);
	// kerr_printf("Read One Sector Finished:%02x\n",io_in8(PORT_DISK1_STATUS_CMD));
	// for(i = 0; i<10; i++)
	// 	kerr_printf("%02x ",a[i]);
}

void ata_handler_write(unsigned long nr, unsigned long r) {
    kerr_printf("write success!!!!!\n");
}


#define STATUS_BSY 0x80
#define STATUS_RDY 0x40
#define STATUS_DRQ 0x08
#define STATUS_DF 0x20
#define STATUS_ERR 0x01

//This is really specific to out OS now, assuming ATA bus 0 master 
//Source - OsDev wiki
static void ATA_wait_BSY();
static void ATA_wait_DRQ();
void read_sectors_ATA_PIO(uint32_t target_address, uint32_t LBA, uint8_t sector_count)
{

	ATA_wait_BSY();
	io_out8(0x1F6,0xE0 | ((LBA >>24) & 0xF));
	io_out8(0x1F2,sector_count);
	io_out8(0x1F3, (uint8_t) LBA);
	io_out8(0x1F4, (uint8_t)(LBA >> 8));
	io_out8(0x1F5, (uint8_t)(LBA >> 16)); 
	io_out8(0x1F7,0x20); //Send the read command

	uint16_t *target = (uint16_t*) target_address;

	for (int j =0;j<sector_count;j++)
	{
		ATA_wait_BSY();
		ATA_wait_DRQ();
		for(int i=0;i<256;i++)
			target[i] = io_in16(0x1F0);
		target+=256;
	}
}


void write_sectors_ATA_PIO(uint32_t LBA, uint8_t sector_count, uint32_t* bytes)
{
	ATA_wait_BSY();
	io_out8(0x1F6,0xE0 | ((LBA >>24) & 0xF));
	io_out8(0x1F2,sector_count);
	io_out8(0x1F3, (uint8_t) LBA);
	io_out8(0x1F4, (uint8_t)(LBA >> 8));
	io_out8(0x1F5, (uint8_t)(LBA >> 16)); 
	io_out8(0x1F7,0x30); //Send the write command

	for (int j =0;j<sector_count;j++)
	{
		ATA_wait_BSY();
		ATA_wait_DRQ();
		for(int i=0;i<256;i++)
		{
			io_out32(0x1F0, bytes[i]);
		}
	}
}

static void ATA_wait_BSY()   //Wait for bsy to be 0
{
	while(io_in8(0x1F7)&STATUS_BSY);
}
static void ATA_wait_DRQ()  //Wait fot drq to be 1
{
	while(!(io_in8(0x1F7)&STATUS_RDY));
}

