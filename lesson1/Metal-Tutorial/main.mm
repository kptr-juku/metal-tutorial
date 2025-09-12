//
//  main.mm
//  Metal-Guide
//

#include "mtl_engine.hpp"

int main() {
    // So that XCode won't start two instances - https://developer.apple.com/forums/thread/765445?page=2
    // https://www.reddit.com/r/Xcode/comments/1g7640w/xcode_starting_running_my_programs_twice/
    sleep(1);
 
    @autoreleasepool {
        MTLEngine engine;
        engine.init();
        engine.run();
        engine.cleanup();
    }

    return 0;
}
