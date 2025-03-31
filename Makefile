# Obtain OS type
ifeq ($(shell uname), Linux)
    OS_TYPE = linux
else ifeq ($(shell uname), Darwin)
    OS_TYPE = macos
else ifeq ($(OS), Windows_NT)
    OS_TYPE = windows
else
    OS_TYPE = unknown
endif

debug:
	make -f ./ccc/Makefile COMPILE_FLAGS="-Og -g -std=c++17 -W -Wall"
	make -f ./cccmain/Makefile COMPILE_FLAGS="-Og -g -std=c++17 -W -Wall"
	make -f ./cccproject/Makefile COMPILE_FLAGS="-Og -g -std=c++17 -W -Wall"
	g++ -o ./build/bin/cccproject build/lib/cccmain.a build/lib/cccproject.a

release:
	make -f ./ccc/Makefile COMPILE_FLAGS="-O2 -std=c++17 -W -Wall"
	make -f ./cccmain/Makefile COMPILE_FLAGS="-O2 -std=c++17 -W -Wall"
	make -f ./cccproject/Makefile COMPILE_FLAGS="-O2 -std=c++17 -W -Wall"
	g++ -o ./build/bin/cccproject build/lib/cccmain.a build/lib/cccproject.a

clean:
	make -f ./ccc/Makefile clean
	make -f ./cccmain/Makefile clean
	make -f ./cccproject/Makefile clean

dev:
	export PATH=$$PATH:$(shell pwd)/build/bin/
	export CCC_COMPILER=g++
	export CCC_LIBRARY_PATH=$(shell pwd)/build/lib/
	export CCC_INCLUDE_PATH=$(shell pwd)/build/inc/

install:
	mkdir -p $(shell echo $$HOME)/.ccc
	cp -r ./build/* $(shell echo $$HOME)/.ccc/
ifeq ($(OS_TYPE), windows)
	python ./script/windows_installer.py
endif

uninstall:
	rm -rf $(shell echo $$HOME)/.ccc