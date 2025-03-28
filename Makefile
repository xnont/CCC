debug:
	make -f ./ccc/Makefile
	make -f ./cccmain/Makefile COMPILE_FLAGS="-Og -g -std=c++17 -W -Wall"
	make -f ./cccproject/Makefile COMPILE_FLAGS="-Og -g -std=c++17 -W -Wall"
	cp -r ./util ./build/inc

release:
	make -f ./ccc/Makefile
	make -f ./cccmain/Makefile COMPILE_FLAGS="-O2 -std=c++17 -W -Wall"
	make -f ./cccproject/Makefile COMPILE_FLAGS="-O2 -std=c++17 -W -Wall"
	cp -r ./util ./build/inc

clean:
	make -f ./cccmain/Makefile clean
	make -f ./cccproject/Makefile clean

dev:
	export PATH=$$PATH:$(shell pwd)/build/bin/
	export CCC_COMPILER=g++
	export CCC_LIBRARY_PATH=$(shell pwd)/build/lib/
	export CCC_INCLUDE_PATH=$(shell pwd)/build/inc/