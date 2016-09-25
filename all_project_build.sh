#!/bin/sh
CMDNAME=`basename $0`
if [[ $1 = "help" ]]; then
  echo "Usage: $CMDNAME [clean]"
  echo ""
  exit
fi

# make clean
if [[ $1 = "clean" ]]; then
  for file in `ls -d *`
  do
    if [ -e "${file}/Makefile" ]; then
      cd "${file}"
      echo -e "\e[1;32mClean project: " ${file} "\e[m"
      make clean >& /dev/null
      if [ $? -ne 0 ]; then
          echo -e "\e[1;31mError: " ${file} "\e[m"
          echo ""
          break;
      fi
	  cd ..
    fi
  done
fi

# make
for file in `ls -d *`
do
  if [ -e "${file}/Makefile" ]; then
	echo -e "\e[1;35mStart project: " ${file} "\e[m"
    cd "${file}"
    make > /dev/null
    if [ $? -ne 0 ]; then
        echo -e "\e[1;31mCompile error: " ${file} "\e[m"
		echo ""
        break;
    fi
    cd ..
	echo -e "\e[1;36mBuild project: " ${file} "\e[m"
  fi
done
