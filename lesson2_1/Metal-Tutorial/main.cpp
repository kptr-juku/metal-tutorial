//
//  main.mm
//  Metal-Guide
//

#include <iostream>

#include "mtl_engine.hpp"

int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <image file path>" << std::endl;
        return 1;
    }

    // So that XCode won't start two instances - https://developer.apple.com/forums/thread/765445?page=2
    // https://www.reddit.com/r/Xcode/comments/1g7640w/xcode_starting_running_my_programs_twice/
    sleep(1);
 
    MTLEngine engine;
    engine.init(argv[1 ]);
    engine.run();
    engine.cleanup();

    return 0;
}
