#!/bin/bash
device_type="3200P 2210P 5600P 3800D"
for var in $device_type;do
echo start make $var
source env.sh $var 
make -C tsip/src/drv clean
make -C tsip/src/drv

done



