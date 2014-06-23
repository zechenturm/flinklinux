#ifndef FLINK_H_
#define FLINK_H_

#include <linux/types.h>
#include <linux/spinlock_types.h>
#include <linux/list.h>
#include <linux/fs.h>

// ############ Flink error numbers ############
#define UNKOWN_ERROR -1

// FPGA module interface types
extern const char* fmit_lkm_lut[];

// ############ Forward declarations ############
struct flink_device;

// ############ Flink private data ############
struct flink_private_data {
	struct flink_device*    fdev;
	struct flink_subdevice* current_subdevice;
};

// ############ Flink bus operations ############
struct flink_bus_ops {
	u8  (*read8)(struct flink_device*, u32 addr);
	u16 (*read16)(struct flink_device*, u32 addr);
	u32 (*read32)(struct flink_device*, u32 addr);
	int (*write8)(struct flink_device*, u32 addr, u8 val);
	int (*write16)(struct flink_device*, u32 addr, u16 val);
	int (*write32)(struct flink_device*, u32 addr, u32 val);
	u32 (*address_space_size)(struct flink_device*);
};

// ############ Flink subdevice ############
#define MAX_NOF_SUBDEVICES 256

struct flink_subdevice {
	struct list_head     list;
	struct flink_device* parent;
	u8                   id;
	u16                  type_id;
	u8                   sub_type_id;
	u8                   if_version;
	u32                  base_addr;
	u32                  mem_size;
	u32                  nof_channels;
};

// ############ Flink device ############
struct flink_device {
	struct list_head      list;
	u8                    id;
	u8                    nof_subdevices;
	struct list_head      subdevices;
	struct flink_bus_ops* bus_ops;
	struct module*        appropriated_module;
	void*                 bus_data;
	struct cdev*          char_device;
	struct device*        sysfs_device;
};

// ############ Public functions ############
extern struct flink_device*    flink_device_alloc(void);
extern void                    flink_device_init(struct flink_device* fdev, struct flink_bus_ops* bus_ops, struct module* mod);
extern int                     flink_device_add(struct flink_device* fdev);
extern int                     flink_device_remove(struct flink_device* fdev);
extern int                     flink_device_delete(struct flink_device* fdev);
extern struct flink_device*    flink_get_device_by_id(u8 flink_device_id);
extern struct flink_device*    flink_get_device_by_cdev(struct cdev* char_device);
extern struct list_head*       flink_get_device_list(void);

extern struct flink_subdevice* flink_subdevice_alloc(void);
extern void                    flink_subdevice_init(struct flink_subdevice* fsubdev);
extern int                     flink_subdevice_add(struct flink_device* fdev, struct flink_subdevice* fsubdev);
extern int                     flink_subdevice_remove(struct flink_subdevice* fsubdev);
extern int                     flink_subdevice_delete(struct flink_subdevice* fsubdev);
extern struct flink_subdevice* flink_get_subdevice_by_id(struct flink_device* fdev, u8 flink_device_id);

extern struct class*           flink_get_sysfs_class(void);

extern int                     flink_select_subdevice(struct file* f, u8 subdevice, bool exclusive);

// ############ Constants ############

// Memory addresses and offsets
#define MAIN_HEADER_SIZE			16		// byte
#define SUB_HEADER_SIZE				16		// byte
#define SUBDEV_TYPE_OFFSET			0x0000	//byte
#define SUBDEV_SIZE_OFFSET			0x0004	//byte
#define SUBDEV_NOFCHANNELS_OFFSET	0x0008	//byte
#define SUBDEV_STATUS_OFFSET		0x0010	//byte
#define SUBDEV_CONFIG_OFFSET		0x0014	//byte

// Types
#define UNKNOWN_TYPE_ID				0x00

// ############ I/O Controls ############

// IOCTL Commands
#define SELECT_SUBDEVICE			0x10
#define SELECT_SUBDEVICE_EXCL		0x11
#define SELECT_SUBDEVICE 			0x10
#define SELECT_SUBDEVICE_EXCL		0x11
#define READ_NOF_SUBDEVICES			0x20
#define READ_SUBDEVICE_INFO			0x21
#define READ_SINGLE_BIT				0x30
#define WRITE_SINGLE_BIT			0x31

// Userland types and sizes
struct ioctl_bit_container_t {
	uint32_t offset;
	uint8_t  bit;
	uint8_t  value;
};

#define FLINKLIB_SUBDEVICE_SIZE		17		// byte

#endif /* FLINK_H_ */