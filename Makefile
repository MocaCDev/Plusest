.PHONY: all

all:
	cargo build
	gcc main.c -o main.o -lmacrolang -L./target/debug
	@./run.sh