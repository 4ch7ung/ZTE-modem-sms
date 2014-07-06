#!/bin/bash
work_dir="$( cd "$( dirname "$0" )" && pwd )"
i=0
for device in `ls /dev/cu.ZTEUSBATPort_*`
do
	imei=`$work_dir/bin/mget_imei $device`
	if [ $? -ne 0 ]
	then
		echo "[ERROR] Error occured while getting imei numbers"
		exit 1
	fi

	sudo ln -s $device $work_dir/sock/$imei
	if [ $? -ne 0 ]
	then
		echo "[ERROR] Error occured while creating socket files"
		exit 1
	else
		#echo "[INFO] Create socket in $work_dir/sock/$imei"
		((i++))
	fi
done
echo "[DONE] $i sockets created in dir $work_dir/sock"
cd $workdir/sock
for device in `ls`
do
	$work_dir/bin/minit_session $device
	if [ $? -ne 0 ]
		echo  "[ERROR] Error occured on $device init"
		exit 1
	fi
done
