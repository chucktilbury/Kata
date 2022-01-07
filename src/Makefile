
all:
	make -C common RELEASE=0
	make -C scanner RELEASE=0
	make -C emit RELEASE=0
	make -C symbols RELEASE=0
	make -C parser RELEASE=0
	make -C main RELEASE=0

debug:
	make -C common RELEASE=0
	make -C scanner RELEASE=0
	make -C emit RELEASE=0
	make -C symbols RELEASE=0
	make -C parser RELEASE=0
	make -C main RELEASE=0

release:
	make -C common RELEASE=1
	make -C scanner RELEASE=1
	make -C emit RELEASE=1
	make -C symbols RELEASE=1
	make -C parser RELEASE=1
	make -C main RELEASE=1

clean:
	make -C common clean
	make -C scanner clean
	make -C emit clean
	make -C symbols clean
	make -C parser clean
	make -C main clean
