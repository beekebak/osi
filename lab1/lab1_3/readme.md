export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
gcc dynamic.c -shared -fPIC -o libdynamic.so
gcc hello.c -L. -ldynamic -o hello -Wl,-rpath,/osi/lab1/lab1_3
