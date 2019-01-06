flags='-g -Wall'
include='-Isocklib/Include/'
libs='-L./socklib/lib'

cd socklib
sh compile_lib.sh
cd ..

gcc ${flags} ${include} ${libs} client.c -o client -lsocklib
gcc ${flags} ${include} ${libs} server.c -o server -lsocklib
gcc ${flags} ${include} ${libs} producer.c -o producer -lsocklib
gcc ${flags} ${include} ${libs} consumer.c -o consumer -lsocklib
