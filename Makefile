INCLUDES = -Iinclude
LINKS = -lboost_program_options

all:
	cd lib; make

clean:
	rm -f poketest
	cd lib; make clean
