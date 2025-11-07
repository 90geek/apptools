#!/bin/bash

output_cmd=$(./apptools cputemp 0;./apptools cputemp 1;./apptools cputemp 2;./apptools cputemp 3)
echo "$output_cmd"

cat /sys/class/thermal/thermal_zone0/temp
cat /sys/class/thermal/thermal_zone1/temp
cat /sys/class/thermal/thermal_zone2/temp
cat /sys/class/thermal/thermal_zone3/temp
