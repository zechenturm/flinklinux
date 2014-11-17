#Overview
All the basic funtionality can be found in the file `flink_core.c` together with all necessary types and prototypes in `flink.h`. 

##Data Structures
The information for a specific flink device will be stored in a structure `flink_device`.
```
// ############ flink device ############
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
```

<img src="../doc/images/ExampleDataStructures.png" width="500px" />

