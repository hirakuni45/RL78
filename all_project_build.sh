for file in `ls -d *_sample`; do
    cd "${file}"
	make clean
	make
	cd ..
done
