#!/bin/bash

SCRIPT=$(readlink -f $0)
SCRIPT_DIR=$(dirname $SCRIPT)

# flinklib
filepath_func_id_flinklib_dir=./lib
filepath_func_id_flinklib=$filepath_func_id_flinklib_dir/subdevtypes.c
mkdir -p $filepath_func_id_flinklib_dir

# definitions of the func ids
source $SCRIPT_DIR/func_id_definitions.sh



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

/** @file subdevtypes.c
 *  @brief flink userspace library, strings for subdevice functions.
 *
 *  This header file contains string definitions for subdevice function id's.
 *
 *  THIS FILE WAS CREATED AUTOMATICALLY - do not change
 *
 *  Created with: flinkinterface/func_id/create_subdevtypes.cl.sh
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
   content="$content\t\"${names[i]}\",\t\t\t// 0x${hex[i]}\"\n"
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



echo "$filepath created"
exit 0