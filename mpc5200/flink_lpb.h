#ifndef FLINK_LPB_H_
#define FLINK_LPB_H_

#include "../flink.h"

// PCI device data
struct flink_lpb_data {
	void __iomem* base_ptr;
	unsigned long base_address;
	unsigned long mem_size;
};

#endif /* FLINK_LPB_H_ */
