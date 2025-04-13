# <div align="center"> ccc </div>

## <div align="center">Directory</div>
- [Project Name](#project-name)
- [Project Introduction](#project-introduction)
- [Install and Uninstall](#install-and-uninstall)
- [Tutorial](#tutorial)

## Project Name
ccc

## Project Introduction

## Install and Uninstall
#### 1. Download the ccc
```shell
git clone https://github.com/buttfa/ccc.git
cd ccc
```

#### 2. Compile ccc
```shell
make release
# or
ccc release # If you have installed ccc
```

#### 3. Install ccc
```shell
make install
```

#### 4. Uninstall ccc
```shell
make uninstall
```

## Tutorial
#### 1. Execution([Hello world](./example/hello_world))
##### First, create the src/main.cpp file and write the following content.
```cpp
#include <iostream>
using namespace std;

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
```

##### Then create project.cpp in the project root directory to describe project information.
```cpp
#include "ccc/project.h"
#include <string>
#include <vector>
using namespace ccc;
using namespace std;

/* In the init_project function, we can describe and add the libraries and
 * applications that make up the project. */
void init_project(project* self, string cmd, vector<string> args) {
    execution exe("hello_world", "Say hello world!"); // Create an application
    exe.add_source_files({"./src"}, {".cpp"});        // Add source files
    self->add_exe(exe); // Add the application to the project
}

/* We did not request any resources that need to be manually processed in
 * init_project, so we did nothing in exit_project. */
void exit_project(project* self, string cmd, vector<string> args) {}

/* Register a project and specify its initialization and exit functions. */
project hello_world("hello_world", init_project, exit_project,
                    "Say hello world!");
```

##### Finally, run the following command to compile and run the project.
```shell
ccc && ./build/hello_world
```

##### The phenomenon of executing commands is as follows
```shell
$ ccc && ./build/hello_world
g++ ./build/obj//src\main.obj -o ./build//hello_world  
Hello, World!
```

#### 2. Library(Math library)

#### 3. Dependency

#### 4. Description

#### 5. Command