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
 *  fLink Linux, subdevice types 	 	                   *
 *                                                                 *
 *******************************************************************/

/** @file flink_fmi.c
 *  @brief flink linux, strings for subdevice functions.
 *
 *  This header file contains string definitions for subdevice function id's.
 
 *  THIS FILE WAS CREATED AUTOMATICALLY
 *
 *  Createt with: flinklinux/shared_config/func_id/create_func_id_files.sh
 *
 *  @author Martin Züger
 *  @author Marcel Gehrig
 */
 
 #include flink.h

const char* fmit_lkm_lut[] = {
	[0x00] = "Info",
	[0x01] = "Analog input",
	[0x02] = "Analog output",
	[0x03] = "reserved",
	[0x04] = "reserved",
	[0x05] = "Digital I/O",
	[0x06] = "Counter",
	[0x07] = "Timer",
	[0x08] = "Memory",
	[0x09] = "reserved",
	[0x0A] = "reserved",
	[0x0B] = "reserved",
	[0x0C] = "PWM",
	[0x0D] = "PPWA",
	[0x0E] = "unknown",
	[0x0F] = "unknown",
	[0x10] = "Watch dog",
	[0x11] = "Sensor",
	[0x12] = "unknown",
	[0x13] = "unknown",
	[0x14] = "unknown",
	[0x15] = "unknown",
	[0x16] = "unknown",
	[0x17] = "unknown",
	[0x18] = "unknown",
	[0x19] = "unknown",
};

