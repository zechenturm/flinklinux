/** @file flink_pci.h
 *  @brief Data structures for pci bus communication module. 
 *
 *  @author Martin Züger
 *  @author Urs Graf
 */

#ifndef FLINK_PCI_H_
#define FLINK_PCI_H_

#include "flink.h"

#define PCI_CONFIG_BASE 0x0000
#define PCI_CONFIG_SIZE 0x4000
#define BASE_OFFSET (PCI_CONFIG_BASE + PCI_CONFIG_SIZE)

/// @brief PCI device data
struct flink_pci_data {
	struct pci_dev* pci_device;
	void __iomem* base_addr;
	unsigned long mem_size;
};

#endif /* FLINK_PCI_H_ */
