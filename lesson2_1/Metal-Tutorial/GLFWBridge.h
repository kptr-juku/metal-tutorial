#ifndef GLFWBridge_h
#define GLFWBridge_h
struct GLFWwindow;

#include <CoreFoundation/CFCGTypes.h>

namespace CA {
    class MetalLayer;
}
namespace GLFWBridge {
    void AddLayerToWindow(GLFWwindow *window, CA::MetalLayer *layer);
}
#endif /* GLFWBridge_h */
