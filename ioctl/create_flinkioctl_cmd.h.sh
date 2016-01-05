#!/bin/bash

SCRIPT=$(readlink -f $0)
SCRIPT_DIR=$(dirname $SCRIPT)

# flinklib
filepath_ioctl_flinklib_dir=.
filepath_ioctl_flinklib=$filepath_ioctl_flinklib_dir/flinkioctl_cmd.h
mkdir -p $filepath_ioctl_flinklib_dir

# definitions of the shared ioctl commands
source $SCRIPT_DIR/ioctl_definitions.sh


# DEBUG
: '
for (( i=0; i < ${#names[@]}; i++)); do
  echo ${names[i]}
  echo ${hex[i]}
done
'


# flinklib: flinkioctl_cmd.h
# ##########################
filepath=$filepath_ioctl_flinklib

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

/** @file flinkioctl_cmd.h
 *  @brief flink userspace library, ioctl comand definitions.
 *
 *  THIS FILE WAS CREATED AUTOMATICALLY
 *
 *  Createt with: flinklinux/shared_config/ioctl/create_ioctl_files.sh
 *
 *  This header file contains definitions for ioctl calls.
 *
 *  @author Marcel Gehrig
 */
 
" >> $filepath

# ioctl list
typdef_enum_ioctl="typedef enum {\n"
for (( i=0; i < ${#names[@]}; i++)); do		# whole list
   typdef_enum_ioctl="$typdef_enum_ioctl\t${names[i]}\t\t\t= 0x${hex[i]},\n"
done
typdef_enum_ioctl=${typdef_enum_ioctl::-3}	# delete last ','
typdef_enum_ioctl="$typdef_enum_ioctl\n} ioctl_cmd_t;"

echo -e $typdef_enum_ioctl >> $filepath	# write file

echo "$filepath_ioctl_flinklib created"
exit 0