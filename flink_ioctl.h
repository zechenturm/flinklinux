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
 *  			ioctl definitions  	                   *
 *                                                                 *
 *******************************************************************/

/** @file flinkioctl.h
 *  @brief flinklinux, ioctl comand definitions.
 *
 *  THIS FILE WAS CREATED AUTOMATICALLY
 *
 *  Createt with: flinklinux/shared_config/ioctl/create_ioctl_files.sh
 *
 *  This header file contains definitions for ioctl calls.
 *
 *  @author Marcel Gehrig
 */
 

// ############ I/O Controls ############
 
 // IOCTL Commands
#define SELECT_SUBDEVICE			= 0x10,
#define SELECT_SUBDEVICE_EXCL			= 0x11,
#define READ_NOF_SUBDEVICES			= 0x20,
#define READ_SUBDEVICE_INFO			= 0x21,
#define READ_SINGLE_BIT			= 0x30,
#define WRITE_SINGLE_BIT			= 0x31,
#define SELECT_AND_READ_BIT			= 0x40,
#define SELECT_AND_WRITE_BIT			= 0x41,
#define SELECT_AND_READ			= 0x42,
#define SELECT_AND_WRITE			= 0x43,

