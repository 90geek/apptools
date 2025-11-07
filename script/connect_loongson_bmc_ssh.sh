#!/bin/bash
if [ $# != 1 ];then
    echo "Usage : $0 bmcip"
    exit 0
fi
bmc=$1
password_file=password.enc
ipmitool -I lanplus -H $bmc -U root -P Loongson@2024 raw 0x30 0x54 0x0
if [ "$?" != 0 ];then
    echo "bmc未准备完成，请稍候再试或检查参数是否正确"
    exit 0
fi
echo "获取密钥..."
curl -k -s -u "root:Loongson@2024" -H "Content-Type: application/json"  https://${bmc}/loongson/v1/Authentication/DynamicPassword --output $password_file
echo "获取密钥完成，解密..."
curl -s -F "file_input=@password.enc" http://10.40.22.13:8000/decrypt_file --output password.ext
str="$(cat password.ext)"
str="$(echo ${str} | sed -n 's/.*"content": "\(.*\)".*/\1/p')"
if [ ${#str} -ne 16 ];then
	echo "解密失败 : $str"
	exit 1
fi
echo "解密完成，开启ssh..."
ipmitool -I lanplus -H $bmc -U root -P Loongson@2024 raw 0x30 0x5a 0x01 0x${str:0:2} 0x${str:2:2} 0x${str:4:2} 0x${str:6:2} 0x${str:8:2} 0x${str:10:2} 0x${str:12:2} 0x${str:14:2}
echo "开启完成，允许root登录..."
ipmitool -I lanplus -H $bmc -U root -P Loongson@2024 raw 0x30 0x58 0x01 0x${str:0:2} 0x${str:2:2} 0x${str:4:2} 0x${str:6:2} 0x${str:8:2} 0x${str:10:2} 0x${str:12:2} 0x${str:14:2}
#ipmitool -I lanplus -H $bmc -U root -P Loongson@2024 raw 0x30 0x59 22 0x0 # 更改端口为22时，该行和下面三行都需要放开，且ssh端口号需要修改
sleep 10
ipmitool -I lanplus -H $bmc -U root -P Loongson@2024 raw 0x30 0x58 0x01 0x${str:0:2} 0x${str:2:2} 0x${str:4:2} 0x${str:6:2} 0x${str:8:2} 0x${str:10:2} 0x${str:12:2} 0x${str:14:2}
sleep 10
echo "允许root登录设置完成，连接ssh"
ssh -p 60804 root@$bmc
exit 0
