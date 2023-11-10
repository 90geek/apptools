#!/bin/bash
function get_make_command()
{
	echo command make
}

function make()
{
		local start_time=$(date +"%s")
		$(get_make_command) "$@"
		local ret=$?
		local end_time=$(date +"%s")
		local tdiff=$(($end_time-$start_time))
		local hours=$(($tdiff / 3600 ))
		local mins=$((($tdiff % 3600) / 60))
		local secs=$(($tdiff % 60))
		echo
		if [ $ret -eq 0 ] ; then
				echo -n -e "\033[1;32m #### make \""$@"\" completed successfully!"
		else
				echo -n -e "\033[1;31m #### make target \""$@"\" failed !!!"
		fi
		if [ $hours -gt 0 ] ; then
				printf "(%02g:%02g:%02g (hh:mm:ss))" $hours $mins $secs
		elif [ $mins -gt 0 ] ; then
				printf "(%02g:%02g (mm:ss))" $mins $secs
		elif [ $secs -gt 0 ] ; then
				printf "(%s seconds)" $secs
		fi
		echo -e " ####\033[1;0m"
		echo
		return $ret
}

################################################
# host mips64 loongarch64 
################################################
SUPPORT_PLATFORM=(host mips64 loongarch64)
unset PLATFORM

if [ $1 ];then 
	if [[ "${SUPPORT_PLATFORM[@]}" =~ $1 ]];then
		export PLATFORM=$1
	else
		echo "Support Platform As Belows:"
		for var in ${SUPPORT_PLATFORM[@]};	
		do	
					echo $var  
		done	
	fi
else
		echo "Support Platform As Belows:"
		for var in ${SUPPORT_PLATFORM[@]};	
		do	
					echo $var  
		done	
fi
################################################
#
################################################
unset SDK_DIR
export SDK_DIR=$(pwd)

################################################
# modules
################################################
if [ $PLATFORM ] ;then
	if [ $PLATFORM = "host" ] ; then	
		unset CAPSULE_SUPPORT
		# export CAPSULE_SUPPORT=1

		unset HARDINFO_SUPPORT
		export HARDINFO_SUPPORT=1

		unset DMIDECODE_SUPPORT
		export DMIDECODE_SUPPORT=1

		unset INFOUI_SUPPORT
		# export INFOUI_SUPPORT=1
		
		unset DEVMEM_SUPPORT
		export DEVMEM_SUPPORT=1

		unset CAPSULE_SUPPORT
		export CAPSULE_SUPPORT=1
		
		unset UPFLASH_SUPPORT
		export UPFLASH_SUPPORT=1
	elif [ $PLATFORM = "mips64" ] ; then	 
		unset CAPSULE_SUPPORT
		# export CAPSULE_SUPPORT=1

		unset HARDINFO_SUPPORT
		export HARDINFO_SUPPORT=1

		unset DMIDECODE_SUPPORT
		export DMIDECODE_SUPPORT=1
		
		unset INFOUI_SUPPORT
		# export INFOUI_SUPPORT=1
		
		unset DEVMEM_SUPPORT
		export DEVMEM_SUPPORT=1

		unset RUNTIME_SUPPORT
		export RUNTIME_SUPPORT=1

		unset UPFLASH_SUPPORT
		export UPFLASH_SUPPORT=1
	elif [ $PLATFORM = "loongarch64" ] ; then 
		unset CAPSULE_SUPPORT
		# export CAPSULE_SUPPORT=1

		unset HARDINFO_SUPPORT
		export HARDINFO_SUPPORT=1

		unset DMIDECODE_SUPPORT
		export DMIDECODE_SUPPORT=1
		
		unset INFOUI_SUPPORT
		# export INFOUI_SUPPORT=1
		
		unset DEVMEM_SUPPORT
		export DEVMEM_SUPPORT=1
		
		unset RUNTIME_SUPPORT
		export RUNTIME_SUPPORT=1
		
		unset UPFLASH_SUPPORT
		export UPFLASH_SUPPORT=1
	else 
		echo "PLATFORM not define"	
fi
	################################################
	#display make infomation
	################################################
 
	make info
fi
