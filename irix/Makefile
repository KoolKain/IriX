all: build

build:
	cd src; make
	cd config; make

generic:
	cd src; cp Makefile.generic Makefile; make
	cd config; make

sunos:
	cd src; cp Makefile.sunos Makefile; make
	cd config; make

bsd:
	cd src; cp Makefile.bsd Makefile; make
	cd config; cp Makefile.bsd Makefile; make

rh5:
	cd src; cp Makefile.bsd Makefile; make
	cd config; cp Makefile.bsd Makefile; make
