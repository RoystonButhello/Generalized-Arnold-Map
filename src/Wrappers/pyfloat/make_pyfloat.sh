swig -python pyfloat.i
gcc -w -c -fpic pyfloat_wrap.c pyfloat.c -I/usr/include/python3.5m
gcc -w -shared pyfloat.o pyfloat_wrap.o -o _pyfloat.so