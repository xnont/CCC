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

./build/lib/cccmain.a: ./cccmain/src/cccmain.cpp
	mkdir -p ./build/lib
ifeq ($(OS_TYPE), windows)
	$(COMPILER) ./cccmain/src/cccmain.cpp -o ./build/lib/libcccmain.lib -c -I./cccproject/inc $(COMPILE_FLAGS)
else ifeq ($(OS_TYPE), linux)
	$(COMPILER) ./cccmain/src/cccmain.cpp -o ./build/lib/libcccmain.a -c -I./cccproject/inc $(COMPILE_FLAGS)
endif

clean:
	rm -rf ./build/lib/cccmain.a