#include "cccmain.h"

int main(int argc, char* argv[]) {
    for (auto project : ccc::projects) {
        project->init_func(project);
        project->exit_func(project);
    }

    return 0;
}