gcc -c hello_from_static_lib.c
ar rc libstatic.a hello_from_static_lib.o 
gcc hello.c -L. -l static -o hello

