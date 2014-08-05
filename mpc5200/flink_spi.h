#ifndef FLINK_LPB_H_
#define FLINK_LPB_H_

#include "../flink.h"


//SPI device data
struct flink_spi_data {
	void __iomem* base_ptr;
	unsigned long mem_size;
};

#endif /* FLINK_LPB_H_ */
