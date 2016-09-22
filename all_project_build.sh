#!/bin/sh
CMDNAME=`basename $0`
if [[ $1 = "help" ]]; then
  echo "Usage: $CMDNAME [clean]"
  echo ""
  exit
fi

# 本体
for file in `ls -d *`
do
  if [ -e "${file}/Makefile" ]; then
	echo -e "\e[1;35mStart project: " ${file} "\e[m"
    cd "${file}"
    if [[ $1 = "clean" ]]; then
      make clean
      echo -e "\e[1;32mClean project: " ${file} "\e[m"
    fi
    make
#   echo $?                                                                                     
    if [ $? -ne 0 ]; then
        echo -e "\e[1;31mCompile error: " ${file} "\e[m"
		echo ""
        break;
    fi
    cd ..
	echo -e "\e[1;36mEnd project: " ${file} "\e[m"
    echo ""
  fi
done
