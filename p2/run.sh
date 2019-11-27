PORT=1234
make clean
make
fuser -fk $PORT/udp
fuser -fk 20000/udp
rm test.out
./file-receiver test.out $PORT 1 > test.out &
./file-sender test.in localhost $PORT 1 > test.out 

echo "diff: "
diff test.in test.out