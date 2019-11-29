PORT=1240
WS=5
make clean
make
fuser -fk $PORT/udp
fuser -fk 20000/udp
rm test.out
./file-sender   test.in localhost 1234 $WS > test.out &
./file-receiver test.out 1234 $WS > test.out 

echo "diff: "
diff test.in test.out -q