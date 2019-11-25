PORT=1234
make clean
make
fuser -fk $PORT/udp
fuser -fk 20000/udp
rm test.out
./file-sender   test.in localhost 20000 1 > test.out &
./file-receiver test.out $PORT 1 > test.out

diff test.in test.out