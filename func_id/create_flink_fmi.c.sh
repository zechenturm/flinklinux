#!/bin/bash

SCRIPT=$(readlink -f $0)
SCRIPT_DIR=$(dirname $SCRIPT)

# flinklinux
filepath_func_id_flinklinux_dir=.
filepath_func_id_flinklinux=$filepath_func_id_flinklinux_dir/flink_fmi.c
mkdir -p $filepath_func_id_flinklinux_dir

# definitions of the func ids
source $SCRIPT_DIR/func_id_definitions.sh



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
 *  @brief flinklinux, strings for subdevice functions.
 *
 *  This header file contains string definitions for subdevice function id's.
 
 *  THIS FILE WAS CREATED AUTOMATICALLY - do not change
 *
 *  Created with: flinkinterface/func_id/create_func_id_files.sh
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