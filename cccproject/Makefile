#### General parameters 
# Set Target type (lib or app)
TARGET_TYPE=lib

# Set Language (c or cpp)
LANGUAGE=cpp
# Set Compiler
COMPILER=g++
# Set Compile flags (Optional. And enclosed in single quotation marks.)
COMPILE_FLAGS=
# Set Linker
LINKER=g++
# Set Linker flags (Optional. And enclosed in single quotation marks.)
LINK_FLAGS=

# Set Source folders (Please write each folder containing source files into it. And enclosed in single quotation marks.)
SOURCE_FOLDERS='./cccproject/src/ccc/'
# Set Header folders (Please write each folder containing header files into it. And enclosed in single quotation marks.)
HEADER_FOLDERS='./cccproject/inc/ ./cccproject/inc/ccc/'
# Set Library_files
LIBRARY_FILES=
# Set Object files path
OBJ_PATH=./cccproject/build/obj/

#### If TARGET_TYPE=lib, Please set the following parameters.
# Set Lib type (static or dynamic)
LIB_TYPE=static
# LIB_PATH is an optional parameter used to set the storage path for compiled library file. (default to./build/lib)
LIB_PATH=./build/lib/
LIB_NAME=cccproject.a
# Set the folder path for storing the header files of the library project. (Will preserve the file organization structure)
HEADER_PATH=./build/inc/

#### If TARGET_TYPE=app, Please set the following parameters.
APP_PATH=
APP_NAME=


PROJECT_ARGS=LANGUAGE=$(LANGUAGE) COMPILER=$(COMPILER) COMPILE_FLAGS='$(COMPILE_FLAGS)' LINKER=$(LINKER) LINK_FLAGS=$(LINK_FLAGS) LIBRARY_FILES=$(LIBRARY_FILES) TARGET_TYPE=$(TARGET_TYPE) SOURCE_FOLDERS=$(SOURCE_FOLDERS) HEADER_FOLDERS=$(HEADER_FOLDERS) OBJ_PATH=$(OBJ_PATH) LIB_NAME=$(LIB_NAME) LIB_TYPE=$(LIB_TYPE) LIB_PATH=$(LIB_PATH) HEADER_PATH=$(HEADER_PATH) APP_PATH=$(APP_PATH) APP_NAME=$(APP_NAME) -f ./third_party/make-tmpl/Makefile

compile:
	make $(PROJECT_ARGS)
	mv ./build/inc/cccproject/inc/ccc ./build/inc/ccc || true
	rm -rf ./build/inc/cccproject

info:
	make info $(PROJECT_ARGS)

clean:
	make clean $(PROJECT_ARGS)
