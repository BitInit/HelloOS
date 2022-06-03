MAKEFLAGS = --no-print-directory

build:
	@make -C src build
	bash build_iso.sh

clean:
	@make -C src clean
	@rm -rf HelloOS*
