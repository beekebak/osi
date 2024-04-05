gcc dynamic.c -shared -fPIC -o libdynamic.so
gcc hello.c -L. -ldynamic -o hello

