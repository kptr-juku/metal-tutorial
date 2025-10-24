//
//  mtl_engine.mm
//  Metal-Guide
//

#include "mtl_engine.hpp"

#include <chrono>
#include <format>
#include <iostream>

#include "AAPLMathUtilities.h"
#include "GLFWBridge.h"

void MTLEngine::init(std::string_view pic) {
    std::cout << "init()" << std::endl;
    initDevice();
    initWindow();
    
    createCube(pic);
    createDefaultLibrary();
    createCommandQueue();
    createRenderPipeline();
}

void MTLEngine::run() {
    std::cout << "run()" << std::endl;
    while (!glfwWindowShouldClose(glfwWindow)) {
        pPool = NS::AutoreleasePool::alloc()->init();
        metalDrawable = metalLayer->nextDrawable();
        draw();
        pPool->release();
        glfwPollEvents();
    }
}

void MTLEngine::cleanup() {
    std::cout << "cleanup()" << std::endl;
    glfwTerminate();
}

void MTLEngine::initDevice() {
    metalDevice = MTL::CreateSystemDefaultDevice();
}

void MTLEngine::initWindow() {
    std::cout << "initWindow()" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindow = glfwCreateWindow(800, 600, "Metal Engine", NULL, NULL);
    
    if (!glfwWindow) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    int width, height;
    glfwGetFramebufferSize(glfwWindow, &width, &height);
    
    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetFramebufferSizeCallback(glfwWindow, frameBufferSizeCallback);

    metalLayer = CA::MetalLayer::layer();
    metalLayer->setDevice(metalDevice);
    metalLayer->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
    metalLayer->setDrawableSize(CGSizeMake(width, height));
    GLFWBridge::AddLayerToWindow(glfwWindow, metalLayer);
}

void MTLEngine::createCube(std::string_view pic) {
    // Cube for use in a right-handed coordinate system with triangle faces
    // specified with a Counter-Clockwise winding order.
    VertexData cubeVertices[] = {
        // Front face
        {{-0.5, -0.5, 0.5, 1.0}, {0.0, 0.0}},
        {{0.5, -0.5, 0.5, 1.0}, {1.0, 0.0}},
        {{0.5, 0.5, 0.5, 1.0}, {1.0, 1.0}},
        {{0.5, 0.5, 0.5, 1.0}, {1.0, 1.0}},
        {{-0.5, 0.5, 0.5, 1.0}, {0.0, 1.0}},
        {{-0.5, -0.5, 0.5, 1.0}, {0.0, 0.0}},

        // Back face
        {{0.5, -0.5, -0.5, 1.0}, {0.0, 0.0}},
        {{-0.5, -0.5, -0.5, 1.0}, {1.0, 0.0}},
        {{-0.5, 0.5, -0.5, 1.0}, {1.0, 1.0}},
        {{-0.5, 0.5, -0.5, 1.0}, {1.0, 1.0}},
        {{0.5, 0.5, -0.5, 1.0}, {0.0, 1.0}},
        {{0.5, -0.5, -0.5, 1.0}, {0.0, 0.0}},

        // Top face
        {{-0.5, 0.5, 0.5, 1.0}, {0.0, 0.0}},
        {{0.5, 0.5, 0.5, 1.0}, {1.0, 0.0}},
        {{0.5, 0.5, -0.5, 1.0}, {1.0, 1.0}},
        {{0.5, 0.5, -0.5, 1.0}, {1.0, 1.0}},
        {{-0.5, 0.5, -0.5, 1.0}, {0.0, 1.0}},
        {{-0.5, 0.5, 0.5, 1.0}, {0.0, 0.0}},

        // Bottom face
        {{-0.5, -0.5, -0.5, 1.0}, {0.0, 0.0}},
        {{0.5, -0.5, -0.5, 1.0}, {1.0, 0.0}},
        {{0.5, -0.5, 0.5, 1.0}, {1.0, 1.0}},
        {{0.5, -0.5, 0.5, 1.0}, {1.0, 1.0}},
        {{-0.5, -0.5, 0.5, 1.0}, {0.0, 1.0}},
        {{-0.5, -0.5, -0.5, 1.0}, {0.0, 0.0}},

        // Left face
        {{-0.5, -0.5, -0.5, 1.0}, {0.0, 0.0}},
        {{-0.5, -0.5, 0.5, 1.0}, {1.0, 0.0}},
        {{-0.5, 0.5, 0.5, 1.0}, {1.0, 1.0}},
        {{-0.5, 0.5, 0.5, 1.0}, {1.0, 1.0}},
        {{-0.5, 0.5, -0.5, 1.0}, {0.0, 1.0}},
        {{-0.5, -0.5, -0.5, 1.0}, {0.0, 0.0}},

        // Right face
        {{0.5, -0.5, 0.5, 1.0}, {0.0, 0.0}},
        {{0.5, -0.5, -0.5, 1.0}, {1.0, 0.0}},
        {{0.5, 0.5, -0.5, 1.0}, {1.0, 1.0}},
        {{0.5, 0.5, -0.5, 1.0}, {1.0, 1.0}},
        {{0.5, 0.5, 0.5, 1.0}, {0.0, 1.0}},
        {{0.5, -0.5, 0.5, 1.0}, {0.0, 0.0}},
    };

    cubeVertexBuffer = metalDevice->newBuffer(&cubeVertices, sizeof(cubeVertices), MTL::ResourceStorageModeShared);

    transformationBuffer = metalDevice->newBuffer(sizeof(TransformationData), MTL::ResourceStorageModeShared);
    
    grassTexture = new Texture(pic.data(), metalDevice);
}


void MTLEngine::createDefaultLibrary() {
    metalDefaultLibrary = metalDevice->newDefaultLibrary();
    if(!metalDefaultLibrary){
        std::cerr << "Failed to load default library.";
        std::exit(-1);
    }
}

void MTLEngine::createCommandQueue() {
    metalCommandQueue = metalDevice->newCommandQueue();
}

void MTLEngine::createRenderPipeline() {
    MTL::Function* vertexShader = metalDefaultLibrary->newFunction(NS::String::string("vertexShader", NS::ASCIIStringEncoding));
    assert(vertexShader);
    MTL::Function* fragmentShader = metalDefaultLibrary->newFunction(NS::String::string("fragmentShader", NS::ASCIIStringEncoding));
    assert(fragmentShader);
    
    MTL::RenderPipelineDescriptor* renderPipelineDescriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    assert(renderPipelineDescriptor);
    renderPipelineDescriptor->setLabel(NS::String::string("Triangle Rendering Pipeline", NS::ASCIIStringEncoding));
    renderPipelineDescriptor->setVertexFunction(vertexShader);
    renderPipelineDescriptor->setFragmentFunction(fragmentShader);
    MTL::PixelFormat pixelFormat = (MTL::PixelFormat)metalLayer->pixelFormat();
    renderPipelineDescriptor->colorAttachments()->object(0)->setPixelFormat(pixelFormat);
    
    NS::Error* error;
    metalRenderPSO = metalDevice->newRenderPipelineState(renderPipelineDescriptor, &error);
    
    renderPipelineDescriptor->release();
}

void MTLEngine::draw() {
    // Get current system time
    //auto now = std::chrono::system_clock::now();

    // Convert to time since epoch (seconds)
    //auto now_time_t = std::chrono::system_clock::to_time_t(now);

    // Break into calendar time
    //std::tm local_tm = *std::localtime(&now_time_t);

    // Print time with std::format (C++20)
    //std::cout << "draw at() " << std::format("Current time: {:02}:{:02}:{:02}\n",
    //                                         local_tm.tm_hour,
    //                                         local_tm.tm_min,
    //                                        local_tm.tm_sec);
    sendRenderCommand();
}

void MTLEngine::sendRenderCommand() {
    metalCommandBuffer = metalCommandQueue->commandBuffer();
    
    MTL::RenderPassDescriptor* renderPassDescriptor = MTL::RenderPassDescriptor::alloc()->init();
    MTL::RenderPassColorAttachmentDescriptor* cd = renderPassDescriptor->colorAttachments()->object(0);
    cd->setTexture(metalDrawable->texture());
    cd->setLoadAction(MTL::LoadActionClear);
    cd->setClearColor(MTL::ClearColor(41.0f/255.0f, 42.0f/255.0f, 48.0f/255.0f, 1.0)); // Background color
    cd->setStoreAction(MTL::StoreActionStore);
    
    MTL::RenderCommandEncoder* renderCommandEncoder = metalCommandBuffer->renderCommandEncoder(renderPassDescriptor);
    encodeRenderCommand(renderCommandEncoder);
    renderCommandEncoder->endEncoding();

    metalCommandBuffer->presentDrawable(metalDrawable);
    metalCommandBuffer->commit();
    metalCommandBuffer->waitUntilCompleted();
    
    renderPassDescriptor->release();
}

void MTLEngine::encodeRenderCommand(MTL::RenderCommandEncoder* renderCommandEncoder) {
    // Moves the Cube 2 units down the negative Z-axis
    matrix_float4x4 translationMatrix = matrix4x4_translation(0, 0,-1.0);

    float angleInDegrees = glfwGetTime()/2.0 * 45;
    float angleInRadians = angleInDegrees * M_PI / 180.0f;
    matrix_float4x4 rotationMatrix = matrix4x4_rotation(angleInRadians, 0.0, 1.0, 0.0);

    matrix_float4x4 modelMatrix = simd_mul(translationMatrix, rotationMatrix);

    simd::float3 R = simd::float3 {1, 0, 0}; // Unit-Right
    simd::float3 U = simd::float3 {0, 1, 0}; // Unit-Up
    simd::float3 F = simd::float3 {0, 0,-1}; // Unit-Forward
    simd::float3 P = simd::float3 {0, 0, 1}; // Camera Position in World Space

    matrix_float4x4 viewMatrix = matrix_make_rows(R.x, R.y, R.z, dot(-R, P),
                                                  U.x, U.y, U.z, dot(-U, P),
                                                 -F.x,-F.y,-F.z, dot( F, P),
                                                  0, 0, 0, 1);

    const auto layerSize = metalLayer->drawableSize();
    float aspectRatio = (layerSize.width / layerSize.height);
    float fov = 90 * (M_PI / 180.0f);
    float nearZ = 0.1f;
    float farZ = 100.0f;

    matrix_float4x4 perspectiveMatrix = matrix_perspective_right_hand(fov, aspectRatio, nearZ, farZ);

    TransformationData transformationData = { modelMatrix, viewMatrix, perspectiveMatrix };
    memcpy(transformationBuffer->contents(), &transformationData, sizeof(transformationData));

    renderCommandEncoder->setRenderPipelineState(metalRenderPSO);
    renderCommandEncoder->setVertexBuffer(cubeVertexBuffer, 0, 0);
    renderCommandEncoder->setVertexBuffer(transformationBuffer, 0, 1);
    MTL::PrimitiveType typeTriangle = MTL::PrimitiveTypeTriangle;
    NS::UInteger vertexStart = 0;
    NS::UInteger vertexCount = 36;
    renderCommandEncoder->setFragmentTexture(grassTexture->texture, 0);
    renderCommandEncoder->drawPrimitives(typeTriangle, vertexStart, vertexCount);
}


void MTLEngine::frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    MTLEngine* engine = (MTLEngine*)glfwGetWindowUserPointer(window);
    engine->resizeFrameBuffer(width, height);
}

void MTLEngine::resizeFrameBuffer(int width, int height) {
    //std::cout << __FUNCTION__ << " " << width << "x" << height << std::endl;
    metalLayer->setDrawableSize(CGSizeMake(width, height));
}
