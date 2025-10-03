//
//  mtl_engine.hpp
//  Metal-Guide
//

#pragma once

#include <filesystem>
#include <string_view>

#define GLFW_INCLUDE_NONE
#import <GLFW/glfw3.h>

#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include <simd/simd.h>

#include "vertex_data.hpp"
#include "texture.hpp"
#include "stb/stb_image.h"


class MTLEngine {
public:
    void init(std::string_view pic);
    void run();
    void cleanup();

private:
    void initDevice();
    void initWindow();
    
    void createSquare(std::string_view pic);
    void createDefaultLibrary();
    void createCommandQueue();
    void createRenderPipeline();
    
    void encodeRenderCommand(MTL::RenderCommandEncoder* renderEncoder);
    void sendRenderCommand();
    void draw();
    
    static void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
    void resizeFrameBuffer(int width, int height);
    
    NS::AutoreleasePool* pPool;
    
    MTL::Device* metalDevice;
    GLFWwindow* glfwWindow;
    CA::MetalLayer* layer;
    CA::MetalDrawable* metalDrawable;
    
    MTL::Library* metalDefaultLibrary;
    MTL::CommandQueue* metalCommandQueue;
    MTL::CommandBuffer* metalCommandBuffer;
    MTL::RenderPipelineState* metalRenderPSO;
    MTL::Buffer* squareVertexBuffer;

    Texture* grassTexture;
};
