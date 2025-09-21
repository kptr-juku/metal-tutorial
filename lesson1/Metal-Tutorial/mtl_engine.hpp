//
//  mtl_engine.hpp
//  Metal-Guide
//

#pragma once

#define GLFW_INCLUDE_NONE
#import <GLFW/glfw3.h>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include <simd/simd.h>

class MTLEngine {
public:
    void init();
    void run();
    void cleanup();

private:
    void initDevice();
    void initWindow();
    
    void createTriangle();
    void createDefaultLibrary();
    void createCommandQueue();
    void createRenderPipeline();
    
    void encodeRenderCommand(MTL::RenderCommandEncoder* renderEncoder);
    void sendRenderCommand();
    void draw();
    
    NS::AutoreleasePool* pPool;
    
    MTL::Device* metalDevice;
    GLFWwindow* glfwWindow;
    CA::MetalLayer* layer;
    CA::MetalDrawable* metalDrawable;
    
    MTL::Library* metalDefaultLibrary;
    MTL::CommandQueue* metalCommandQueue;
    MTL::CommandBuffer* metalCommandBuffer;
    MTL::RenderPipelineState* metalRenderPSO;
    MTL::Buffer* triangleVertexBuffer;
};
