
all:
	make -C src debug

debug:
	make-C src debug

release:
	make -C src release

clean:
	make -C src clean
