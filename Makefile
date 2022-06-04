.PHONY: all

all:
	cargo build
	gcc main.c -o main -lmacrolang -L./target/debug
	@./run.sh