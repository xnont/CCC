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

COMPILER=g++


debug:
	make build COMPILER=$(COMPILER) COMPILE_FLAGS="-g -std=c++17 -W -Wall -Wextra" -B

release:
	make build COMPILER=$(COMPILER) COMPILE_FLAGS="-O2 -std=c++17 -W -Wall -Wextra" -B

build:
	make -f ./ccc/Makefile COMPILER=$(COMPILER) COMPILE_FLAGS="$(COMPILE_FLAGS)" -j
	make -f ./cccmain/Makefile COMPILER=$(COMPILER) COMPILE_FLAGS="$(COMPILE_FLAGS)" -j
	make -f ./cccunit/Makefile COMPILER=$(COMPILER) COMPILE_FLAGS="$(COMPILE_FLAGS)" -j
ifeq ($(OS_TYPE), windows)
	$(COMPILER) build/lib/libcccmain.lib build/lib/cccunit.dll -o ./build/bin/default_project
else ifeq ($(OS_TYPE), linux)
	cp ./build/lib/libcccunit.so .
	$(COMPILER) build/lib/libcccmain.a libcccunit.so -o ./build/bin/default_project
	rm ./libcccunit.so
endif
	cp -r ./cccunit/inc ./build

clean:
	make -f ./ccc/Makefile clean
	make -f ./cccmain/Makefile clean
	make -f ./cccunit/Makefile clean

install:
	mkdir -p $(shell echo $$HOME)/.ccc
	cp -r ./build/* $(shell echo $$HOME)/.ccc/
ifeq ($(OS_TYPE), windows)
	python ./script/windows_installer.py
endif
ifeq ($(OS_TYPE), linux)
	python ./script/linux_installer.py > /dev/null 2>&1 || python3 ./script/linux_installer.py 
endif

uninstall:
	rm -rf $(shell echo $$HOME)/.ccc
