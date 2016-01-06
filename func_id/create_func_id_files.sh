#!/bin/bash

# flinklib
filepath_func_id_flinklib_dir=./flinklib/lib
filepath_func_id_flinklib=$filepath_func_id_flinklib_dir/subdevtypes.c
mkdir -p $filepath_func_id_flinklib_dir

# flinklinux
filepath_func_id_flinklinux_dir=./flinklinux
filepath_func_id_flinklinux=$filepath_func_id_flinklinux_dir/flink_fmi.c
mkdir -p $filepath_func_id_flinklinux_dir

# definitions of the func ids
names[0]="Info";
hex[0]=00;

names[1]="Analog input"
hex[1]=01;

names[2]="Analog output"
hex[2]=02;

names[3]="reserved"
hex[3]=03;

names[4]="reserved"
hex[4]=04;

names[5]="Digital I/O"
hex[5]=05;

names[6]="Counter"
hex[6]=06;

names[7]="Timer"
hex[7]=07;

names[8]="Memory"
hex[8]=08;

names[9]="reserved"
hex[9]=09;

names[10]="reserved"
hex[10]=0A;

names[11]="reserved"
hex[11]=0B;

names[12]="PWM"
hex[12]=0C;

names[13]="PPWA"
hex[13]=0D;

names[14]="unknown"
hex[14]=0E;

names[15]="unknown"
hex[15]=0F;

names[16]="Watch dog"
hex[16]=10;

names[17]="Sensor"
hex[17]=11;

names[18]="unknown"
hex[18]=12;

names[19]="unknown"
hex[19]=13;

names[20]="unknown"
hex[20]=14;

names[21]="unknown"
hex[21]=15;

names[22]="unknown"
hex[22]=16;

names[23]="unknown"
hex[23]=17;

names[24]="unknown"
hex[24]=18;

names[25]="unknown"
hex[25]=19;


# DEBUG
for (( i=0; i < ${#names[@]}; i++)); do
#for (( i=0; i < 11; i++)); do
  echo ${names[i]}
  echo ${hex[i]}
done




# flinklib: subdevtypes.c
# #######################
filepath=$filepath_func_id_flinklib

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
 *  fLink userspace library, subdevice types  	                   *
 *                                                                 *
 *******************************************************************/

/** @file flinkioctl_cmd.h
 *  @brief flink userspace library, strings for subdevice functions.
 *
 *  This header file contains string definitions for subdevice function id's.
 *
 *  THIS FILE WAS CREATED AUTOMATICALLY
 *
 *  Createt with: flinklinux/shared_config/create_ioctl_files.sh
 *
 *
 *  @author Martin Züger
 *  @author Marcel Gehrig
 */
 
" >> $filepath

# func_id list
content="#include <stdint.h>\n
\n
const char* flink_subdev_function_strings[] = {\n"

for (( i=0; i < ${#names[@]}; i++)); do		# whole list
   content="$content\t\"${names[i]}\",\t\t\t// 0x${hex[i]}\",\n"
done

content="$content};\n"

content="$content
#define NOF_KNOWNSUBDEVIDS (sizeof(flink_subdev_function_strings) / sizeof(char*))\n
\n
\n
const char* flink_subdevice_id2str(uint16_t id) {\n
	if(id > NOF_KNOWNSUBDEVIDS - 1) { // unknown subdevice id\n
		return "unknown";\n
	}\n
	return flink_subdev_function_strings[id];\n
}\n"

echo -e $content >> $filepath	# write file




# flinklinux: flink_fmi.c
# #######################
filepath=$filepath_func_id_flinklinux

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
 
 #include "flink.h"
" >> $filepath

# func_id list
fmit_lkm_lut="const char* fmit_lkm_lut[] = {\n"

for (( i=0; i < ${#names[@]}; i++)); do		# whole list
   fmit_lkm_lut="$fmit_lkm_lut\t[0x${hex[i]}] = \"${names[i]}\",\n"
done

fmit_lkm_lut="$fmit_lkm_lut};\n"

echo -e $fmit_lkm_lut >> $filepath	# write file



echo "$filepath_func_id_flinklib and $filepath_func_id_flinklinux created"
exit 0