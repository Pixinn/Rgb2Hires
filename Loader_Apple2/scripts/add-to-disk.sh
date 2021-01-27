#!/bin/bash
# Adds the required files to the provided disk.dsk
# usage: add_to_disk PATH_TO_APPLECOMMANDER.jar PATH_TO_BINARY.a2 PATH_TO_PICTURE PATH_TO_DISK

set -e

if (( $# != 4 )); then
    echo "Bad number of arguments"
    echo "usage: add_to_disk.sh PATH_TO_APPLECOMMANDER.jar PATH_TO_BINARY.a2 PATH_TO_PICTURE PATH_TO_DISK"
    exit
fi

echo " . removing previous instance of DISPLAY form the disk"
java -jar ${1} -d ${4} DISPLAY

echo " .. adding DISPLAY to the disk"
java -jar ${1} -cc65 ${4} DISPLAY BIN < ${2}

echo " . removing previous instance of PICTURE form the disk"
java -jar ${1} -d ${4} PICTURE

echo " .. adding PICTURE to the disk"
java -jar ${1} -p ${4} PICTURE BIN < ${3}

echo "DONE."
