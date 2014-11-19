#Bus Communication Modules
Every such module handles the actual bus transfer over a given hardware interface. For this purpose it must implement the bus operation functions given in 

```
// ############ flink bus operations ############
struct flink_bus_ops {
	u8  (*read8)(struct flink_device*, u32 addr);
	u16 (*read16)(struct flink_device*, u32 addr);
	u32 (*read32)(struct flink_device*, u32 addr);
	int (*write8)(struct flink_device*, u32 addr, u8 val);
	int (*write16)(struct flink_device*, u32 addr, u16 val);
	int (*write32)(struct flink_device*, u32 addr, u32 val);
	u32 (*address_space_size)(struct flink_device*);
};
```

Currently we support transfer over PCI (`pci.c`).
For the //Phytec phyCORE-MPC5200B-I/O// board there is a driver using the local plus bus on the mpc5200 (`flink_lpb.c`).
We plan to support SPI in the near future.
