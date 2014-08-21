#!/bin/bash
work_dir="$( cd "$( dirname "$0" )" && pwd )"
if [ -z $2 ]
then
	echo "Usage: storm_sms.sh <phone_number> <message>"
	exit 1
fi
cd $work_dir/sock
for device in `ls`
do
	$work_dir/bin/msend_sms $device "$1" "$2"&
done
