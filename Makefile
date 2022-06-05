MAKEFLAGS = --no-print-directory

all: clean build

build:
	@make -C src build
	bash build_iso.sh

clean:
	@make -C src clean
	@rm -rf HelloOS*
