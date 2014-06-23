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
 *  SPI bus communication module                                   *
 *                                                                 *
 *******************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/device.h>

#include "flink.h"

#define DBG 1
#define MODULE_NAME THIS_MODULE->name

MODULE_AUTHOR("Martin Zueger <martin@zueger.eu>");
MODULE_DESCRIPTION("fLink SPI module");
MODULE_SUPPORTED_DEVICE("fLink SPI devices");
MODULE_LICENSE("Dual BSD/GPL");

// ############ Bus communication functions ############
u8 spi_read8(struct flink_device* fdev, u32 addr) {
	// TODO
	return 0;
}

u16 spi_read16(struct flink_device* fdev, u32 addr) {
	// TODO
	return 0;
}

u32 spi_read32(struct flink_device* fdev, u32 addr) {
	// TODO
	return 0;
}

int spi_write8(struct flink_device* fdev, u32 addr, u8 val) {
	// TODO
	return -1;
}

int spi_write16(struct flink_device* fdev, u32 addr, u16 val) {
	// TODO
	return -1;
}

int spi_write32(struct flink_device* fdev, u32 addr, u32 val) {
	// TODO
	return -1;
}

u32 spi_address_space_size(struct flink_device* fdev) {
	// TODO
	return 0;
}

struct flink_bus_ops spi_bus_ops = {
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
	
	// All done
	printk(KERN_INFO "[%s] Module sucessfully loaded", MODULE_NAME);
	
	return 0;

	// ERROR HANDLING

	
	return error;
}

static void __exit flink_spi_exit(void) {
	printk(KERN_INFO "[%s] Module sucessfully unloaded", MODULE_NAME);
}

module_init(flink_spi_init);
module_exit(flink_spi_exit);
