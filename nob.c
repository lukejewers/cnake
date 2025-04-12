#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD_FOLDER "build/"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);
    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;
    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "gcc",
                   "-Wall", "-Wextra", "-ggdb",
                   "-I.", "-I./thirdparty/raylib/src/",
                   "-o", BUILD_FOLDER"main", "main.c",
                   "-L./thirdparty/raylib/src/", "-lraylib",
                   // macOS specific frameworks
                   "-framework", "CoreVideo",
                   "-framework", "IOKit",
                   "-framework", "Cocoa",
                   "-framework", "GLUT",
                   "-framework", "OpenGL",
                   "-framework", "CoreFoundation");
    if (!nob_cmd_run_sync(cmd)) return 1;
    return 0;
}
