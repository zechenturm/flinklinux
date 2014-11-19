/** @file flink_lpb.h
 *  @brief Data structures for local plus bus communication module.
 *
 *  @author Marco Tinner
 *  @author Urs Graf
 */

#ifndef FLINK_LPB_H_
#define FLINK_LPB_H_

#include "../flink.h"

/// @brief Local Plus Bus device data
struct flink_lpb_data {
	void __iomem* base_ptr;
	unsigned long base_address;
	unsigned long mem_size;
};

#endif /* FLINK_LPB_H_ */
