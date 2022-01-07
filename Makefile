
all:
	make -C src RELEASE=0

debug:
	make-C src RELEASE=0

release:
	make -C src RELEASE=1

clean:
	make -C src clean
