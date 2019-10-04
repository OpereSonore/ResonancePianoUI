#!/bin/bash

firmware_folder="/home/pi/pianoLight/firmware"
log_folder="/home/pi/pianoLight/log"
update_folder="/home/pi/pianoLight/update"

usb_update_file="/media/usb/pianoLightRecovery.tar.gz"
local_update_file="/home/pi/pianoLight/pianoLight.tar.gz"

if [ -d $firmware_folder ]; then
	echo "[OK] $firmware_folder"
else
	mkdir $firmware_folder
	echo "[CREATED] $firmware_folder"
fi

if [ -d $log_folder ]; then
	echo "[OK] $log_folder"
else
	mkdir $log_folder
	echo "[CREATED] $log_folder"
fi

if [ -d $update_folder ]; then
	echo "[OK] $update_folder"
else
	mkdir $update_folder
	echo "[CREATED] $update_folder"
fi

if [ -f $usb_update_file ]; then
	echo "[FOUND] $usb_update_file"
	cp -f $usb_update_file $local_update_file
fi

export QT_QPA_EGLFS_PHYSICAL_WIDTH=154
export QT_QPA_EGLFS_PHYSICAL_HEIGHT=85
export QT_QPA_EGLFS_WIDTH=800
export QT_QPA_EGLFS_HEIGHT=480
export QT_QPA_EGLFS_CURSOR=0

if [ -f $local_update_file ]; then
	su pi -c '/home/pi/pianoLight/bootloader'
else
	su pi -c '/home/pi/pianoLight/pianoLight'
fi
