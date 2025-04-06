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

test:
	make clean
	make debug
	make install

debug:
	make build COMPILE_FLAGS="-Og -g -std=c++17 -W -Wall -Wextra" -B

release:
	make build COMPILE_FLAGS="-O2 -std=c++17 -W -Wall -Wextra" -B

build:
	make -f ./ccc/Makefile COMPILE_FLAGS="$(COMPILE_FLAGS)" -j
	make -f ./cccmain/Makefile COMPILE_FLAGS="$(COMPILE_FLAGS)" -j
	make -f ./cccproject/Makefile COMPILE_FLAGS="$(COMPILE_FLAGS)" -j
	g++ build/lib/cccmain.a build/lib/cccproject.a -o ./build/bin/default_project
	cp -r ./cccproject/inc ./build

clean:
	make -f ./ccc/Makefile clean
	make -f ./cccmain/Makefile clean
	make -f ./cccproject/Makefile clean

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


