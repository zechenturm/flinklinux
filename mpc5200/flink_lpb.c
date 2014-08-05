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
 *  MPC5200 local plus bus communication module                    *
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
#include "flink_lpb.h"

#define DBG 1
#define MODULE_NAME THIS_MODULE->name

#define CS3CR_ADDRESS 0xf000030c
#define CS3START_REG_ADDRESS 0xf000001c
#define CS3CR_VALUE 0x00ff0500 
#define CS3_MEMORY_LENGTH 0x02000000

//TODO insert to chose between CS3, CS4 and both to expand memory range

MODULE_AUTHOR("Marco Tinner <marco.tinner@ntb.ch>");
MODULE_DESCRIPTION("fLink local plus bus module for mpc5200");
MODULE_SUPPORTED_DEVICE("fLink LPB devices");
MODULE_LICENSE("Dual BSD/GPL");

// ############ Module parameters ############
static unsigned int dev_mem_length = 0x40;

module_param(dev_mem_length, uint, 0444);
MODULE_PARM_DESC(dev_mem_length, "Device memory length");


// ############ Bus communication functions ############
u8 lpb_read8(struct flink_device* fdev, u32 addr) {
	struct flink_lpb_data* lpb_data = (struct flink_lpb_data*)fdev->bus_data;
	if(lpb_data != NULL) {
		return ioread8(lpb_data->base_ptr + addr);
	}
	return 0;
}

u16 lpb_read16(struct flink_device* fdev, u32 addr) {
	struct flink_lpb_data* lpb_data = (struct flink_lpb_data*)fdev->bus_data;
	if(lpb_data != NULL) {
		return le16_to_cpu(ioread16(lpb_data->base_ptr + addr));
	}
	return 0;
}

u32 lpb_read32(struct flink_device* fdev, u32 addr){
	struct flink_lpb_data* lpb_data = (struct flink_lpb_data*)fdev->bus_data;
	if(lpb_data != NULL) {
		return le32_to_cpu(ioread32(lpb_data->base_ptr + addr));
	}
	return 0;
}

int lpb_write8(struct flink_device* fdev, u32 addr, u8 val) {
	struct flink_lpb_data* lpb_data = (struct flink_lpb_data*)fdev->bus_data;
	if(lpb_data != NULL) {
		iowrite8(val,lpb_data->base_ptr+ addr);
	}
	return 0;
}

int lpb_write16(struct flink_device* fdev, u32 addr, u16 val) {
	struct flink_lpb_data* lpb_data = (struct flink_lpb_data*)fdev->bus_data;
	if(lpb_data != NULL) {
		iowrite16(cpu_to_le16(val),lpb_data->base_ptr+ addr);
	}
	return 0;
}

int lpb_write32(struct flink_device* fdev, u32 addr, u32 val) {
	
	struct flink_lpb_data* lpb_data = (struct flink_lpb_data*)fdev->bus_data;
	if(lpb_data != NULL) {
		iowrite32(cpu_to_le32(val),lpb_data->base_ptr+ addr);
	}
	return 0;
}

u32 lpb_address_space_size(struct flink_device* fdev) {
	struct flink_lpb_data* lpb_data = (struct flink_lpb_data*)fdev->bus_data;
	return (u32)(lpb_data->mem_size);
}

struct flink_bus_ops lpb_bus_ops = {
	.read8              = lpb_read8,
	.read16             = lpb_read16,
	.read32             = lpb_read32,
	.write8             = lpb_write8,
	.write16            = lpb_write16,
	.write32            = lpb_write32,
	.address_space_size = lpb_address_space_size
};

// ############ Initialization and cleanup ############
static int __init flink_lpb_init(void) {
	int error = 0;
	struct flink_lpb_data* lpb_data = kmalloc(sizeof(struct flink_lpb_data), GFP_KERNEL); 
	struct flink_device* fdev = flink_device_alloc();	
	void __iomem* reg;
	unsigned int base_address;

	#if defined(DBG)
		printk(KERN_DEBUG "[%s] Initializing module with parameters 'length=%x'", MODULE_NAME, dev_mem_length);
	#endif

	//get CS3 data and configure it
	//config CS3
	if (check_mem_region(CS3CR_ADDRESS, 4)) {
		printk(KERN_ALERT "[%s] ERROR: CS3 configuration register memory already in use\n", MODULE_NAME);
		goto check_mem_region_CS3CR_fail;
	}
	if(request_mem_region(CS3CR_ADDRESS, 4, MODULE_NAME)== NULL){
		printk(KERN_ALERT "[%s] ERROR: I/O request memory region CS3 configuration register failed!", MODULE_NAME);
		goto check_mem_region_CS3CR_fail;
	}
	reg = ioremap(CS3CR_ADDRESS,4);
	if(reg == NULL){
		printk(KERN_ALERT "[%s] ERROR: CS3 configuration register io remap failed!", MODULE_NAME);
		goto iomap_mem_region_CS3CR_fail;
	}
	iowrite32(CS3CR_VALUE,reg);
	iounmap(reg);
	//get CS3 start address
	if (check_mem_region(CS3START_REG_ADDRESS, 4)) {
		printk(KERN_ALERT "[%s] ERROR: CS3 start address register memory already in use\n", MODULE_NAME);
		goto check_mem_region_CS3ST_fail;
	}
	if(request_mem_region(CS3START_REG_ADDRESS, 4, MODULE_NAME)== NULL){
		printk(KERN_ALERT "[%s] ERROR: I/O request memory region CS3 start address register failed!", MODULE_NAME);
		goto check_mem_region_CS3ST_fail;
	}
	reg = ioremap(CS3START_REG_ADDRESS,4);
	if(reg == NULL){
		printk(KERN_ALERT "[%s] ERROR: CS3 start address io remap failed!", MODULE_NAME);
		goto iomap_mem_region_CS3ST_fail;
	}
	base_address = le32_to_cpu(ioread32(reg))<<16;
	#if defined(DBG)
		printk(KERN_DEBUG "[%s] CS3 start address: 0x%x", MODULE_NAME,base_address);
	#endif
	iounmap(reg);

	//create flink device
	if(dev_mem_length <= 0){
		printk(KERN_ALERT "[%s] ERROR: device memory length has to be bigger than 0!", MODULE_NAME);
		goto flink_device_alloc_fail;
	}
	if(dev_mem_length > CS3_MEMORY_LENGTH){
		printk(KERN_ALERT "[%s] ERROR: device memory length has to be smaller than 0x%x!", MODULE_NAME,CS3_MEMORY_LENGTH);
		goto flink_device_alloc_fail;
	}

	if(fdev == NULL){
		printk(KERN_ALERT "[%s] ERROR: fLink device alloc failed!", MODULE_NAME);
		goto flink_device_alloc_fail;
	}		
	if(lpb_data == NULL){
		printk(KERN_ALERT "[%s] ERROR: Memory alloc for lpb data failed!", MODULE_NAME);
		goto mem_alloc_fail;
	}
	lpb_data->mem_size = dev_mem_length;

	//request CS3 local plus bus memory space
	if (check_mem_region(base_address, dev_mem_length)) {
		printk(KERN_ALERT "[%s] ERROR: memory already in use \n", MODULE_NAME);
		goto mem_reg_fail;
	}


	if(request_mem_region(base_address, dev_mem_length, MODULE_NAME) == NULL){
		printk(KERN_ALERT "[%s] ERROR: I/O request memory region failed!", MODULE_NAME);
		goto mem_reg_fail;
	}
	lpb_data->base_address = base_address;
	lpb_data->base_ptr = ioremap(base_address, dev_mem_length);
	if(lpb_data->base_ptr == NULL) {
		printk(KERN_ALERT "[%s] ERROR: io remap failed!", MODULE_NAME);
		goto mem_remap_fail;
	}
	
	

	flink_device_init(fdev, &lpb_bus_ops, THIS_MODULE);
	fdev->bus_data = lpb_data;
	flink_device_add(fdev);
	
	// All done
	printk(KERN_INFO "[%s] Module sucessfully loaded", MODULE_NAME);
	
	return 0;

// ---- ERROR HANDLING ----

	mem_remap_fail:
		release_mem_region(base_address, dev_mem_length);
	mem_reg_fail:
		kfree(lpb_data);
	mem_alloc_fail:
		flink_device_delete(fdev);
	flink_device_alloc_fail:
	iomap_mem_region_CS3ST_fail:
		release_mem_region(CS3START_REG_ADDRESS,4);
	check_mem_region_CS3ST_fail:
	iomap_mem_region_CS3CR_fail:
		release_mem_region(CS3CR_ADDRESS,4);
	check_mem_region_CS3CR_fail:
	return error;

}


static void __exit flink_lpb_exit(void) {
	
	struct flink_device* fdev;
	struct flink_device* fdev_next;
	struct flink_lpb_data* lpb_data;

	list_for_each_entry_safe(fdev, fdev_next, flink_get_device_list(), list) {
		if(fdev->appropriated_module == THIS_MODULE) {
			lpb_data = (struct flink_lpb_data*)(fdev->bus_data);
			iounmap(lpb_data->base_ptr);
			release_mem_region(lpb_data->base_address,lpb_data->mem_size);
			kfree(lpb_data);
			flink_device_remove(fdev);
			flink_device_delete(fdev);
		}
	}
	release_mem_region(CS3CR_ADDRESS,4);
	release_mem_region(CS3START_REG_ADDRESS,4);
	printk(KERN_INFO "[%s] Module sucessfully unloaded", MODULE_NAME);
}

module_init(flink_lpb_init);
module_exit(flink_lpb_exit);
