for file in `ls -d *_sample`; do
    cd "${file}"
	make clean
	make
#	echo $?
	if [ $? -ne 0 ]; then
		break;
	fi
	cd ..
done
