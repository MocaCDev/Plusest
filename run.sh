cargo build
gcc main.c -o main -lmacrolang -L./target/debug
LD_LIBRARY_PATH=./target/debug ./main