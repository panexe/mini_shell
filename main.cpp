#include <iostream>
#include <unistd.h>
#include "MiniShell.h"
#define INPUT_BUFFER_SIZE 128

int main() {

    MiniShell shell = MiniShell();
    shell.run();

    return 0;
}
