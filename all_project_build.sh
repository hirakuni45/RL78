for file in `ls -d *_sample`; do
    cd "${file}"
	make clean
	make
#	echo $?
	if [ $? -ne 0 ]; then
		echo "Compile error: " ${file}
		break;
	fi
	cd ..
done
