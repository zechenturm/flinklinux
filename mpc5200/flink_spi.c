/*******************************************************************
 *   _________     _____      _____    ____  _____    ___  ____    *
 *  |_   ___  |  |_   _|     |_   _|  |_   \|_   _|  |_  ||_  _|   *
 *    | |_  \_|    | |         | |      |   \ | |      | |_/ /     *
 *    |  _|        | |   _     | |      | |\ \| |      |  __'.     *
 *   _| |_        _| |__/ |   _| |_    _| |_\   |_    _| |  \ \_   *
 *  |_____|      |________|  |_____|  |_____|\____|  |____||____|  *
 *                                                                 *
 *******************************************************************
 *                                                                 *
 *  MPC5200 spi communication module  	                           *
 *                                                                 *
 *******************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <asm/io.h>


#include "../flink.h"
#include "flink_spi.h"

#define DBG 1
#define MODULE_NAME THIS_MODULE->name

#define SPI_BASE 0xF0000F00
#define SPIREGS_SIZE 0x10

#define SPICR1_OFFSET 0x0
#define SPICR2_OFFSET 0x1
#define SPIBDRATE_OFFSET 0x4
#define SPIST_OFFSET 0x5
#define SPIDATA_OFFSET 0x9
#define SPIPORT_OFFSET 0xD
#define SPIDATADIR_OFFSET 0x10

#define GPSPCR 0xF0000B00
#define TIMEOUT_CYLES 10000000

MODULE_AUTHOR("Marco Tinner <marco.tinner@ntb.ch>");
MODULE_DESCRIPTION("fLink spi bus module for mpc5200");
MODULE_SUPPORTED_DEVICE("fLink SPI devices");
MODULE_LICENSE("Dual BSD/GPL");

// ############ Module parameters ############
static unsigned int dev_mem_length = 0x40;
module_param(dev_mem_length, uint, 0444);
MODULE_PARM_DESC(dev_mem_length, "Device memory length");

static unsigned long spi_sclk_speed = 2000000;
module_param(spi_sclk_speed, ulong, 0444);
MODULE_PARM_DESC(dev_mem_length, "SPI sclk speed in Hz only values 33Mhz/2, 33Mhz/4, ... , 33/Mhz/2048 are allowed");


// ############ Bus communication functions ############

static int transferData(struct flink_spi_data* spi_data,u32 readAddress,u32 writeAddress,u32 data){
    	u8 dataToSend[12];
	u32 result = 0;
	u32 i = 0;
	u32 timeoutCount= 0;
	dataToSend[0] = (u8) ((readAddress >> 24) & 0xff);
	dataToSend[1] = (u8) ((readAddress >> 16) & 0xff);
	dataToSend[2] = (u8) ((readAddress >> 8)  & 0xff);
	dataToSend[3] = (u8) (readAddress & 0xff);
	dataToSend[4] = (u8) ((writeAddress >> 24) & 0xff);
	dataToSend[5] = (u8) ((writeAddress >> 16) & 0xff);
	dataToSend[6] = (u8) ((writeAddress >> 8)  & 0xff);
	dataToSend[7] = (u8) (writeAddress & 0xff);
	dataToSend[8] = (u8) ((data >> 24) & 0xff);
	dataToSend[9] = (u8) ((data >> 16) & 0xff);
	dataToSend[10] = (u8) ((data >> 8)  & 0xff);
	dataToSend[11] = (u8) (data & 0xff);
    	for(i = 0; i<sizeof(dataToSend); i++){
	    	iowrite8(dataToSend[i],spi_data->base_ptr + SPIDATA_OFFSET);
				
		timeoutCount= 0;
	    	while((ioread8(spi_data->base_ptr + SPIST_OFFSET)&0x80)==0x0 && timeoutCount < TIMEOUT_CYLES){
			timeoutCount++;
		}
		udelay(2); //if this delay is removed the method blocks maybe caused by concurrent access to the SPIST register.
		if (timeoutCount >= TIMEOUT_CYLES){
			printk(KERN_ALERT "[%s] Timeout during SPI Transfer", MODULE_NAME);
			return 0;		
		}
	    	dataToSend[i] = ioread8(spi_data->base_ptr + SPIDATA_OFFSET);
    	}
	result =  ((((u32)dataToSend[8])&0xFF) << 24);
	result =  result | ((((u32)dataToSend[9])&0xFF) << 16);
	result =  result | ((((u32)dataToSend[10])&0xFF) << 8);
	result =  result | (((u32)dataToSend[11])&0xFF);
	return result;
}

static u8 spi_read8(struct flink_device* fdev, u32 addr) {
	struct flink_spi_data* spi_data = (struct flink_spi_data*)fdev->bus_data;
	if(spi_data != NULL) {
		return transferData(spi_data,addr,0,0);
	}
	return 0;
}

static u16 spi_read16(struct flink_device* fdev, u32 addr) {
	struct flink_spi_data* spi_data = (struct flink_spi_data*)fdev->bus_data;
	if(spi_data != NULL) {
		return transferData(spi_data,addr,0,0);
	}
	return 0;
}

static u32 spi_read32(struct flink_device* fdev, u32 addr){
	struct flink_spi_data* spi_data = (struct flink_spi_data*)fdev->bus_data;
	if(spi_data != NULL) {
		return transferData(spi_data,addr,0,0);
	}
	return 0;
}

int spi_write8(struct flink_device* fdev, u32 addr, u8 val) {
	struct flink_spi_data* spi_data = (struct flink_spi_data*)fdev->bus_data;
	if(spi_data != NULL) {
		transferData(spi_data,0,addr,val);
	}
	return 0;
}

static int spi_write16(struct flink_device* fdev, u32 addr, u16 val) {
	struct flink_spi_data* spi_data = (struct flink_spi_data*)fdev->bus_data;
	if(spi_data != NULL) {
		transferData(spi_data,0,addr,val);
	}
	return 0;
}

static int spi_write32(struct flink_device* fdev, u32 addr, u32 val) {
	
	struct flink_spi_data* spi_data = (struct flink_spi_data*)fdev->bus_data;
	if(spi_data != NULL) {
		transferData(spi_data,0,addr,val);
	}
	return 0;
}

static u32 spi_address_space_size(struct flink_device* fdev) {
	struct flink_spi_data* spi_data = (struct flink_spi_data*)fdev->bus_data;
	return (u32)(spi_data->mem_size);
}



//method not working for a value of 0

static u32 pseudo_log2(u32 value){
	int i = 0;
	int temp = 1;
	while(temp < value){
		i++;
		temp=temp*2;
	}
	return i;

}


//calculates the  SPI baude rate register out of the chosen speed. Allways rounds down. 
static u8 speedToRegValue(unsigned long speed){
	u8 regNumber = 0;
	u8 regValue = 0;
	//TODO get system clock from system.
	if(speed > 16500000){
		speed = 16500000;
	}
	if(33000000/speed < 256){
		regNumber = pseudo_log2(33000000/speed)-1;
	}else{
		regNumber = (pseudo_log2(33000000/speed-256)-1)|0x38;
	}
	regValue = regValue |(regNumber & 0x7); //set the last 3 bits;
	regNumber = regNumber & 0x38; //clear all bits expect of 3,4,5
	regNumber = regNumber << 1; //shift one to left to have bits 3,4,5 in the right position see reg definition to understand why this step is neccessary. 
	regValue = regValue | regNumber;
	return regValue;
}


static struct flink_bus_ops spi_bus_ops = {
	.read8              = spi_read8,
	.read16             = spi_read16,
	.read32             = spi_read32,
	.write8             = spi_write8,
	.write16            = spi_write16,
	.write32            = spi_write32,
	.address_space_size = spi_address_space_size
};

// ############ Initialization and cleanup ############
static int __init flink_spi_init(void) {
	int error = 0;
	struct flink_spi_data* spi_data = kmalloc(sizeof(struct flink_spi_data), GFP_KERNEL); 
	struct flink_device* fdev = flink_device_alloc();	
	void __iomem* reg;
	unsigned int gpspcr_val;

	#if defined(DBG)
		printk(KERN_DEBUG "[%s] Initializing module with parameters 'length=%x'", MODULE_NAME, dev_mem_length);
	#endif

	//config pin mux register
	if (check_mem_region(GPSPCR, 4)) {
		printk(KERN_ALERT "[%s] ERROR: GPSPCR register memory already in use\n", MODULE_NAME);
		goto check_mem_region_GPSPCR_fail;
	}
	if(request_mem_region(GPSPCR, 4, MODULE_NAME)== NULL){
		printk(KERN_ALERT "[%s] ERROR: I/O request memory region GPSPCR failed!", MODULE_NAME);
		goto check_mem_region_GPSPCR_fail;
	}
	reg = ioremap(GPSPCR,4);
	if(reg == NULL){
		printk(KERN_ALERT "[%s] ERROR: GPSPCR register io remap failed!", MODULE_NAME);
		goto iomap_mem_region_GPSPCR_fail;
	}
	gpspcr_val = le32_to_cpu(ioread32(reg));
	gpspcr_val = gpspcr_val & 0xCFFFF0FF; //Clean ALTs bits
	gpspcr_val = gpspcr_val | 0x00000C00;// use pins on PCS3 for SPI and UART
	#if defined(DBG)
		printk(KERN_DEBUG "[%s] Set GPSPCR to 0x%x", MODULE_NAME,gpspcr_val);
	#endif	
	iowrite32(cpu_to_le32(gpspcr_val),reg);
	iounmap(reg);
	

	//create flink device
	if(dev_mem_length <= 0){
		printk(KERN_ALERT "[%s] ERROR: device memory length has to be bigger than 0!", MODULE_NAME);
		goto iomap_mem_region_GPSPCR_fail;
	}
	spi_data->mem_size = dev_mem_length;
	
	if(fdev == NULL){
		printk(KERN_ALERT "[%s] ERROR: fLink device alloc failed!", MODULE_NAME);
		goto iomap_mem_region_GPSPCR_fail;
	}		
	if(spi_data == NULL){
		printk(KERN_ALERT "[%s] ERROR: Memory alloc for spi data failed!", MODULE_NAME);
		goto mem_alloc_fail;
	}

	//configure spi register
	if (check_mem_region(SPI_BASE, SPIREGS_SIZE)) {
		printk(KERN_ALERT "[%s] ERROR: SPI register memory already in use\n", MODULE_NAME);
		goto check_mem_region_SPI_fail;
	}
	if(request_mem_region(SPI_BASE, SPIREGS_SIZE, MODULE_NAME)== NULL){
		printk(KERN_ALERT "[%s] ERROR: I/O request memory region SPI failed!", MODULE_NAME);
		goto check_mem_region_SPI_fail;
	}
	spi_data->base_ptr = ioremap(SPI_BASE,SPIREGS_SIZE);
	if(spi_data->base_ptr == NULL){
		printk(KERN_ALERT "[%s] ERROR: SPI register io remap failed!", MODULE_NAME);
		goto iomap_mem_region_SPI_fail;
	}
	iowrite8(0x56,spi_data->base_ptr + SPICR1_OFFSET);//SPI Enable, SPI Master set, CPOL = 0, CPHA = 1, CS enable
	iowrite8(0x00,spi_data->base_ptr + SPICR2_OFFSET);
	iowrite8(speedToRegValue(spi_sclk_speed),spi_data->base_ptr + SPIBDRATE_OFFSET); 
	#if defined(DBG)
		printk(KERN_DEBUG "[%s] Baude Rate register set to: 0x%x", MODULE_NAME,speedToRegValue(spi_sclk_speed));
	#endif	


	iowrite8(0xE,spi_data->base_ptr + SPIDATADIR_OFFSET); // enable CS
	ioread8(spi_data->base_ptr + SPIST_OFFSET); // clear status register


	flink_device_init(fdev, &spi_bus_ops, THIS_MODULE);
	fdev->bus_data = spi_data;
	flink_device_add(fdev);
	

	// All done
	printk(KERN_INFO "[%s] Module sucessfully loaded", MODULE_NAME);
	
	return 0;

// ---- ERROR HANDLING ----

	iomap_mem_region_SPI_fail:		
		release_mem_region(SPI_BASE,SPIREGS_SIZE);
	check_mem_region_SPI_fail:
		kfree(spi_data);
	mem_alloc_fail:
		flink_device_delete(fdev);
	iomap_mem_region_GPSPCR_fail:
		release_mem_region(GPSPCR,4);
	check_mem_region_GPSPCR_fail:
	return error;

}


static void __exit flink_spi_exit(void) {
	
	struct flink_device* fdev;
	struct flink_device* fdev_next;
	struct flink_spi_data* spi_data;

	list_for_each_entry_safe(fdev, fdev_next, flink_get_device_list(), list) {
		if(fdev->appropriated_module == THIS_MODULE) {
			spi_data = (struct flink_spi_data*)(fdev->bus_data);
			iounmap(spi_data->base_ptr);
			kfree(spi_data);
			flink_device_remove(fdev);
			flink_device_delete(fdev);
		}
	}
	release_mem_region(SPI_BASE,SPIREGS_SIZE);
	release_mem_region(GPSPCR,4);
	printk(KERN_INFO "[%s] Module sucessfully unloaded", MODULE_NAME);
}

module_init(flink_spi_init);
module_exit(flink_spi_exit);
