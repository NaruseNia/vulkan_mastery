#include <iostream>
#include "./gfx/gfx_device.h"

int main() {
    std::cout << "INFO: " << "Initializing..." << std::endl;
    GfxDevice device{};

    device.initialize();
    return 0;
}
