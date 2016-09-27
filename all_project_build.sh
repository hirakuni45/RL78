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
      echo "\033[1;32mClean project: " ${file} "\033[m"
      make clean >& /dev/null
      if [ $? -ne 0 ]; then
          echo "\033[1;31mError: " ${file} "\033[m"
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
	echo "\033[1;35mStart project: " ${file} "\033[m"
    cd "${file}"
    make > /dev/null
    if [ $? -ne 0 ]; then
        echo "\033[1;31mCompile error: " ${file} "\033[m"
		echo ""
        break;
    fi
    cd ..
	echo "\033[1;36mBuild project: " ${file} "\033[m"
  fi
done
