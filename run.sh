make clean
cd src
python3 genIDT.py
cd ..
make && qemu-system-x86_64 -kernel kernel -serial stdio
