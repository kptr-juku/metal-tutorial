//
//  main.cpp
//  lesson0-metaltutorial
//
//  Created by foobar on 17.01.2025.
//

#include <iostream>

#include "Metal/Metal.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Creating system default device..." << std::endl;

    MTL::Device* device = MTL::CreateSystemDefaultDevice();

    if (device == nullptr) {
        std::cerr << "Metal is not supported on this system.\n";
        return -1;
    }

    std::cout << "Metal device: " << device->name()->utf8String() << "\n";
    std::cout << device->maxThreadsPerThreadgroup().width << " "
              << device->maxThreadsPerThreadgroup().height << " "
              << device->maxThreadsPerThreadgroup().depth << std::endl;

    device->release();

    return 0;
}
