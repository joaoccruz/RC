PORT=1234
make clean
make
fuser -fk $PORT/udp
./file-sender   test.in localhost $PORT 1 > test.out &
./file-receiver test.out $PORT 1 > test.out