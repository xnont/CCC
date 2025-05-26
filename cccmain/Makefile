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

# Compile
ifeq ($(OS_TYPE), windows)
./build/lib/libcccmain.lib: ./cccmain/src/cccmain.cpp
else ifeq ($(OS_TYPE), linux)
./build/lib/libcccmain.a: ./cccmain/src/cccmain.cpp
endif
	mkdir -p ./build/lib
ifeq ($(OS_TYPE), windows)
	$(COMPILER) ./cccmain/src/cccmain.cpp -o ./build/lib/libcccmain.lib -c -I./cccunit/inc $(COMPILE_FLAGS)
else ifeq ($(OS_TYPE), linux)
	$(COMPILER) ./cccmain/src/cccmain.cpp -o ./build/lib/libcccmain.a -c -I./cccunit/inc $(COMPILE_FLAGS)
endif

clean:
	rm -rf ./build/lib/cccmain.a