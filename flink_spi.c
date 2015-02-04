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

#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/mpc52xx.h>

#include "flink.h"

//#define DBG
#define BUFSIZE 32
#define MODULE_NAME THIS_MODULE->name

// ############ Module Parameters ############
static unsigned int dev_mem_length = 0x40;
module_param(dev_mem_length, uint, 0444);
MODULE_PARM_DESC(dev_mem_length, "device memory length");

MODULE_AUTHOR("Urs Graf");
MODULE_DESCRIPTION("fLink SPI module for mpc5200");
MODULE_SUPPORTED_DEVICE("fLink SPI devices");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("spi:flink_spi");


/// @brief SPI bus data
struct spi_data {
	dev_t				devt;
	spinlock_t			spi_lock;
	struct spi_device*	spi;
	struct list_head	device_entry;

	/* TODO buffer is NULL unless this device is open (users > 0) */
	struct mutex		buf_lock;
//	unsigned			users;
	u32*				txBuf;	// byte ordering in memory is platform specific
	u32*				rxBuf;
	unsigned long 		mem_size; // memory size of flink device including all subdevices
};

struct spi_transfer	t1 = {
	.len = 4,
};
struct spi_transfer	t2 = {
	.len = 4,
};
struct spi_transfer	t3 = {
	.len = 4,
};
struct spi_transfer	t4 = {
	.len = 4,
};
struct spi_transfer	t5 = {
	.len = 4,
};
struct spi_transfer	t6 = {
	.len = 4,
};
struct spi_transfer	r1 = {
	.len		= 4,
};
struct spi_message msg;

static LIST_HEAD(device_list);
//static DEFINE_MUTEX(device_list_lock);

// ############ Prototypes ############


// ############ Bus communication functions ############
u8 spi_read8(struct flink_device* fdev, u32 addr) {
	return 0;
}

u16 spi_read16(struct flink_device* fdev, u32 addr) {
	return 0;
}

u32 spi_read32(struct flink_device* fdev, u32 addr) {
	ssize_t	status = 0;
	struct spi_data* data = (struct spi_data*)fdev->bus_data;
	u32 val;
	spi_message_init(&msg);
	t1.tx_buf = data->txBuf;
	*data->txBuf = addr;
	r1.rx_buf = data->rxBuf;
	spi_message_add_tail(&t1, &msg);
	spi_message_add_tail(&r1, &msg);
	status = spi_sync(data->spi, &msg);
	val = *data->rxBuf;
//	printk(KERN_DEBUG "[%s] read from addr: 0x%x\n", MODULE_NAME, (u32)*data->txBuf);
//	printk(KERN_DEBUG "[%s] read: 0x%x\n", MODULE_NAME, val);
	return val;
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
//	static u32 count = 0;
	ssize_t	status = 0;
	struct spi_data* data = (struct spi_data*)fdev->bus_data;
	spi_message_init(&msg);
//	if(count < 3) {
	t1.tx_buf = data->txBuf;
	*data->txBuf = addr | 0x80000000;	// set write bit
	t2.tx_buf = data->txBuf + 1;
	*(data->txBuf + 1) = val;
//	t3.tx_buf = data->txBuf;
//	t4.tx_buf = data->txBuf;
//	t5.tx_buf = data->txBuf;
//	t6.tx_buf = data->txBuf;
	spi_message_add_tail(&t1, &msg);
	spi_message_add_tail(&t2, &msg);
	status = spi_sync(data->spi, &msg);
//	printk(KERN_DEBUG "[%s] write to addr: 0x%x\n", MODULE_NAME, *data->txBuf);
//	printk(KERN_DEBUG "[%s] write: 0x%x\n", MODULE_NAME, *(data->txBuf+1));
	return 0;
}

u32 spi_address_space_size(struct flink_device* fdev) {
	struct spi_data* data = (struct spi_data*)fdev->bus_data;
	return (u32)(data->mem_size);
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

// ########### Helper functions ##########


// ############ Driver probe and release functions ############
static int __devinit flink_spi_probe(struct spi_device *spi) {
	struct flink_device* fdev;
	struct spi_data *spiData;
	int	status = 0;

	#if defined(DBG)
		printk(KERN_DEBUG "[%s] Run probe\n", MODULE_NAME);
	#endif

	spiData = kzalloc(sizeof(*spiData), GFP_KERNEL);	// Allocate driver data
	if (!spiData) return -ENOMEM;
	// Initialize the driver data
	spiData->spi = spi;
	spin_lock_init(&spiData->spi_lock);
	mutex_init(&spiData->buf_lock);

	INIT_LIST_HEAD(&spiData->device_entry);
	if (status == 0)
		spi_set_drvdata(spi, spiData);
	else {
		kfree(spiData);
		return -1;
	}

	if(dev_mem_length <= 0){
		printk(KERN_ALERT "[%s] ERROR: device memory length has to be bigger than 0!", MODULE_NAME);
		return -1;
	}
	spiData->mem_size = dev_mem_length;
	spiData->txBuf = kmalloc(BUFSIZE, GFP_KERNEL);
	spiData->rxBuf = kmalloc(BUFSIZE, GFP_KERNEL);
	if (!spiData->txBuf || !spiData->rxBuf) return -ENOMEM;
	spi_message_init(&msg);

	fdev = flink_device_alloc();

	flink_device_init(fdev, &spi_bus_ops, THIS_MODULE);
	fdev->bus_data = spiData;
	flink_device_add(fdev);	// creates device nodes
	return 0;
}

static struct class *spidev_class;


static int __devexit flink_spi_remove(struct spi_device *spi) {
	struct spi_data* spiData = spi_get_drvdata(spi);
	struct flink_device* fdev;
	struct flink_device* fdev_next;

	#if defined(DBG)
		printk(KERN_DEBUG "[%s] Run remove\n", MODULE_NAME);
	#endif

	list_for_each_entry_safe(fdev, fdev_next, flink_get_device_list(), list) {
		if(fdev->appropriated_module == THIS_MODULE) {
			flink_device_remove(fdev);
			flink_device_delete(fdev);
		}
	}

	/* make sure ops on existing fds can abort cleanly */
	spin_lock_irq(&spiData->spi_lock);
	spiData->spi = NULL;
	spi_set_drvdata(spi, NULL);
	spin_unlock_irq(&spiData->spi_lock);

	/* prevent new opens */
//	mutex_lock(&device_list_lock);
	list_del(&spiData->device_entry);
	device_destroy(spidev_class, spiData->devt);
//	clear_bit(MINOR(spidev->devt), minors);
//	if (spidev->users == 0)
//		kfree(spidev);
//	mutex_unlock(&device_list_lock);

	kfree(spiData);

	return 0;
}

// ############ Data structures for spi driver ############
static struct spi_driver flink_spi_driver = {
	.driver = {
		.name =	"flink_spi",
		.owner = THIS_MODULE,
	},
	.probe = flink_spi_probe,
	.remove = __devexit_p(flink_spi_remove),
};

static struct spi_board_info flinkInfo = {
		.modalias = "flink_spi",
		.max_speed_hz = 2000000,
		.mode = SPI_MODE_1
};

// ############ Initialization and cleanup ############
static int __init mod_init(void) {
	int status;

	// use these declarations for platform specific initialization
	static const struct of_device_id mpc52xx_gpio_simple[] = {
		{ .compatible = "fsl,mpc5200-gpio", },
		{}
	};
	struct spi_master *master;
	struct spi_device *dev;
	struct mpc52xx_gpio __iomem *gpio;
	struct device_node *np;
	u32 portConfig;
	u8 pscNum = 2;	// set to 1,2,3 or 6

	#if defined(DBG)
		printk(KERN_DEBUG "[%s] Registering flink driver\n", MODULE_NAME);
	#endif
	status = spi_register_driver(&flink_spi_driver);
	if (status < 0) {
		printk(KERN_ERR "[%s] Cannot register driver\n", MODULE_NAME);
		goto exit;
	}

	// use in case a PSC serves as SPI
	// set port configuration register of mpc5200
	np = of_find_matching_node(NULL, mpc52xx_gpio_simple);
	gpio = of_iomap(np, 0);
	of_node_put(np);
	if (!gpio) {
		printk(KERN_ERR "%s() failed, expect abnormal behavior\n", __func__);
		return -1;
	}
	portConfig = in_be32(&gpio->port_config);
	#if defined(DBG)
		printk(KERN_DEBUG "[%s] Current port config = 0x%x\n", MODULE_NAME, portConfig);
	#endif
	if (pscNum == 6) {
		portConfig |=  0x00700060;	// PSC6 is Codec/SPI
	} else if (pscNum >= 1 && pscNum <= 3) {
		portConfig &= ~(7 << ((pscNum-1)*4));
		portConfig |=  6 << ((pscNum-1)*4);
	} else {
		printk(KERN_ERR "[%s] Wrong PSC number\n", MODULE_NAME);
		return -1;
	}
	out_be32(&gpio->port_config, portConfig);
	#if defined(DBG)
		printk(KERN_DEBUG "[%s] New port config = 0x%x\n", MODULE_NAME, portConfig);
	#endif
	iounmap(gpio);

	// if the flink device is not loaded yet through the device tree, do it manually
	//master = spi_busnum_to_master(32766);
	master = spi_busnum_to_master(pscNum);	// use bus number of master
	if (!master) {
		printk(KERN_ERR "[%s] No master for this bus number\n", MODULE_NAME);
		goto exit;
	}
	dev = spi_new_device(master, &flinkInfo);
	if (dev) { // dev is null when the same driver is unloaded and reloaded
		dev->bits_per_word = 32;
		spi_setup(dev);
	}

	// All done
	printk(KERN_INFO "[%s] Module sucessfully loaded\n", MODULE_NAME);

exit:
	return status;
}

static void __exit mod_exit(void) {
	#if defined(DBG)
		printk(KERN_DEBUG "[%s] Unregistering flink driver\n", MODULE_NAME);
	#endif
	spi_unregister_driver(&flink_spi_driver);	// macht das remove läuft

	printk(KERN_INFO "[%s] Module sucessfully unloaded\n", MODULE_NAME);
}

module_init(mod_init);
module_exit(mod_exit);
