#!/bin/bash

# flinklib
filepath_ioctl_flinklib=./flinklib/include/flinkioctl_cmd.h

# flinklinux
filepath_ioctl_flinklinux=./flinklinux/flinkioctl.h

# definitions of the shared ioctl commands
i=0;
names[i]="SELECT_SUBDEVICE";
hex[i]=10;
i=$((i+1));

names[i]="SELECT_SUBDEVICE_EXCL"
hex[i]=11;
i=$((i+1));

names[i]="READ_NOF_SUBDEVICES"
hex[i]=20;
i=$((i+1));

names[i]="READ_SUBDEVICE_INFO"
hex[i]=21;
i=$((i+1));

names[i]="READ_SINGLE_BIT"
hex[i]=30;
i=$((i+1));

names[i]="WRITE_SINGLE_BIT"
hex[i]=31;
i=$((i+1));

names[i]="SELECT_AND_READ_BIT"
hex[i]=40;
i=$((i+1));

names[i]="SELECT_AND_WRITE_BIT"
hex[i]=41;
i=$((i+1));

names[i]="SELECT_AND_READ"
hex[i]=42;
i=$((i+1));

names[i]="SELECT_AND_WRITE"
hex[i]=43;
i=$((i+1));


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
 *  Createt with: flinklinux/shared_config/create_ioctl_files.sh
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



# flinklinux: flinkioctl.h
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

/** @file flinkioctl.h
 *  @brief flinklinux, ioctl comand definitions.
 *
 *  THIS FILE WAS CREATED AUTOMATICALLY
 *
 *  Createt with: flinklinux/shared_config/create_ioctl_files.sh
 *
 *  This header file contains definitions for ioctl calls.
 *
 *  @author Marcel Gehrig
 */
 
" >> $filepath

# ioctl list
typdef_enum_ioctl="// ############ I/O Controls ############\n
\n
// IOCTL Commands\n"

for (( i=0; i < ${#names[@]}; i++)); do		# whole list
   typdef_enum_ioctl="$typdef_enum_ioctl#define ${names[i]}\t\t\t= 0x${hex[i]},\n"
done

echo -e $typdef_enum_ioctl >> $filepath	# write file



echo "$filepath_ioctl_flinklib and $filepath_ioctl_flinklinux created"
exit 0