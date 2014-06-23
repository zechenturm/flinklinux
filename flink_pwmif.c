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
 *  PWM interface module                                           *
 *                                                                 *
 *******************************************************************/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include "flink.h"

#define DBG 1
#define MODULE_NAME THIS_MODULE->name
#define MAX_DEV_NAME_LENGTH 15

MODULE_AUTHOR("Martin Zueger <martin@zueger.eu>");
MODULE_DESCRIPTION("fLink PWM interface module");
MODULE_SUPPORTED_DEVICE("fLink devices with PWM interface");
MODULE_LICENSE("Dual BSD/GPL");

struct flink_pwmif_data {
	struct list_head list;
	struct flink_subdevice* subdev;
	struct cdev *char_device;
	u32 nof_channels;
};

static u8 pwm_dev_counter = 0;
static LIST_HEAD(pwm_devices);

// ############ File operations ############

int pwm_open(struct inode *i, struct file *f) {
	
	return 0;
}

int pwm_relase(struct inode *i, struct file *f) {
	
	return 0;
}

ssize_t pwm_read(struct file *f, char __user *data, size_t size, loff_t *offset) {
	return 0;
}

ssize_t pwm_write(struct file *f, const char __user *data, size_t size, loff_t *offset) {
	u64 channel_data;
	u32 high_time;
	u32 period;
	
	if(size != 8) return -EINVAL;
	if(get_user(channel_data, data)) return -EFAULT;
	period = (u32)channel_data;
	high_time = (u32)(channel_data >> 32);
	
	return 1;
}

struct file_operations pwm_fops = {
	.owner = THIS_MODULE,
	.open = pwm_open,
	.release = pwm_relase,
	.read = pwm_read,
	.write = pwm_write
};

// ############ Create new PWM device ############

int create_pwm_dev(struct flink_subdevice* pwm_subdev) {
	if(pwm_subdev != NULL) {
		char dev_name[MAX_DEV_NAME_LENGTH + 1];
		struct flink_pwmif_data* pwm_data;
		dev_t dev;
		
		// get sysfs class from core module
		struct class* sysfs_class = flink_get_sysfs_class();
		
		// define device name
		sprintf(dev_name, "pwm%u", pwm_dev_counter);
		
		// Create and initialize pwmif_device struct
		pwm_data = kmalloc(sizeof(*pwm_data), GFP_KERNEL);
		INIT_LIST_HEAD(&(pwm_data->list));
		
		// Allocate, register and initialize char device
		alloc_chrdev_region(&dev, pwm_dev_counter, 1, MODULE_NAME);
		pwm_data->char_device = cdev_alloc();
		cdev_init(pwm_data->char_device, &pwm_fops);
		pwm_data->char_device->owner = THIS_MODULE;
//		pwm_dev->char_device->count = pwm_dev_counter;
		cdev_add(pwm_data->char_device, dev, 1);
		
		// create device node
		device_create(sysfs_class, NULL, dev, NULL, dev_name);
		#if defined(DEBUG)
			printk(KERN_DEBUG "[%s] Device node created: %s", dev_name);
		#endif
		
		// Add pwm device to list and subdevice structure
		list_add(&(pwm_data->list), &pwm_devices);
		pwm_subdev->if_data = pwm_data;
			
		// increment pwm device counter
		pwm_dev_counter++;
		
		return 0;
	}
	return -1;
}

void search_for_pwm_subdevices(struct flink_device* fdev) { // TODO first_fdev, nof_devs ?
	struct flink_subdevice* subdev;
	list_for_each_entry(subdev, &(fdev->subdevices), list) {
		if(subdev->type_id == PWM_TYPE_ID) {
			create_pwm_dev(subdev); // TODO check for error
		}
	}
}

// ############ Initialization ############
static int __init flink_init(void) {
	int error = 0;
	struct flink_if_module* pwm_mod = flink_if_module_alloc();
	
	flink_if_module_init(pwm_mod, PWM_TYPE_ID, THIS_MODULE);
	pwm_mod->device_added_hook = search_for_pwm_subdevices;
	flink_if_module_add(pwm_mod);
	
	// ---- All done ----
	printk(KERN_INFO "[%s] Module sucessfully loaded", MODULE_NAME);
		
	return 0;

	// ---- ERROR HANDLING ----
	
	return error;
}
module_init(flink_init);

// ############ Cleanup ############
static void __exit flink_exit(void) {
	struct flink_pwmif_data* pwm_data;
	list_for_each_entry(pwm_data, &pwm_devices, list) {
		device_destroy(flink_get_sysfs_class(), pwm_data->char_device->dev);
		cdev_del(pwm_data->char_device);
		unregister_chrdev_region(pwm_data->char_device->dev, 1);
	}
	
	printk(KERN_INFO "[%s] Module sucessfully unloaded", MODULE_NAME);
}
module_exit(flink_exit);
