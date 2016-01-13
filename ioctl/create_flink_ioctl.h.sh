#!/bin/bash

SCRIPT=$(readlink -f $0)
SCRIPT_DIR=$(dirname $SCRIPT)

# flinklib
filepath_ioctl_flinklinux_dir=.
filepath_ioctl_flinklinux=$filepath_ioctl_flinklinux_dir/flink_ioctl.h
mkdir -p $filepath_ioctl_flinklinux_dir

# definitions of the shared ioctl commands
source $SCRIPT_DIR/ioctl_definitions.sh



# flinklinux: flink_ioctl.h
# ########################
filepath=$filepath_ioctl_flinklinux

# Empty the content of the file
cp /dev/null $filepath

# Header
echo "/*******************************************************************
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

/** @file flink_ioctl.h
 *  @brief flinklinux, ioctl comand definitions.
 *
 *  This header file contains definitions for ioctl calls.
 *
 *  THIS FILE WAS CREATED AUTOMATICALLY - do not change
 *
 *  Created with: flinkinterface/ioctl/create_flink_ioctl_h.sh
 *
 *  @author Marcel Gehrig
 */
 
" >> $filepath

# ioctl list
typdef_enum_ioctl="// ############ I/O Controls ############\n
\n
// IOCTL Commands\n"

for (( i=0; i < ${#names[@]}; i++)); do		# whole list
   typdef_enum_ioctl="$typdef_enum_ioctl#define ${names[i]}\t\t\t 0x${hex[i]}\n"
done

echo -e $typdef_enum_ioctl >> $filepath	# write file



echo "$filepath_ioctl_flinklinux created"
exit 0