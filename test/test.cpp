#include <ngine/ngine.hpp>

#include <ngine/game.hpp>

#include <ngine/graphics/graphics_device.hpp>
#include <ngine/math.hpp>

#if defined(PLATFORM_UWP)
#include <ngine/utils/UWP/uwp_bootstrap.hpp>
#endif

class TestGame : public ngine::Game {
    ngine::graphics::Buffer vertexBuffer;
public:
    TestGame(ngine::WindowDesc conf) : Game(conf) {}

    void init() override {
        // Init game
        Game::init();

        // Get graphics API
        auto api = getWindow()->getContextDescriptor().Type;

        // Get graphics device
        auto graphicsDevice = getGraphicsDevice();

        // Create shaders
        switch (api) {
            case ngine::graphics::ContextType::OpenGL: {
                // Create vertex shader
                auto vertSrc = "#version 330 core\n"
                               "in vec3 POSITION;\n"
                               "in vec2 TEXCOORD;\n"
                               "in vec4 COLOR;\n"
                               "out vec2 fragTexCoord;\n"
                               "out vec4 fragColor;\n"
                               "\n"
                               "layout (binding = 0, std140) uniform Matrices {\n"
                               "    mat4 Model;\n"
                               "    mat4 View;\n"
                               "    mat4 Projection;\n"
                               "    vec3 Test;\n"
                               "};\n"
                               "\n"
                               "void main()\n"
                               "{\n"
                               "    fragTexCoord = TEXCOORD;\n"
                               "    fragColor = COLOR;\n"
                               "    mat4 MVP = Model * View * Projection;\n"
                               "    gl_Position = MVP * vec4(POSITION, 1.0) + vec4(Test, 0.0);\n"
                               "}";
                //vertShader = graphicsDevice->createShader(vertSrc, ngine::graphics::ShaderStage::Vertex);

                // Create fragment shader
                auto fragSrc = "#version 330 core\n"
                               "in vec2 fragTexCoord;\n"
                               "in vec4 fragColor;\n"
                               "out vec4 outColor;\n"
                               "uniform sampler2D Textures[2];\n"
                               "void main()\n"
                               "{\n"
                               "    vec4 texelColor = texture(Textures[1], fragTexCoord);\n"
                               "    outColor = texelColor*fragColor;\n"
                               "}";
                //fragShader = graphicsDevice->createShader(fragSrc, ngine::graphics::ShaderStage::Fragment);
                break;
            }
            case ngine::graphics::ContextType::OpenGLES: {
                // Create vertex shader
                auto vertSrc = "#version 300 es\n"
                               "in vec3 POSITION;\n"
                               "in vec2 TEXCOORD;\n"
                               "in vec4 COLOR;\n"
                               "out vec2 fragTexCoord;\n"
                               "out vec4 fragColor;\n"
                               "layout (std140) uniform Matrices {\n"
                               "    mat4 Model;\n"
                               "    mat4 View;\n"
                               "    mat4 Projection;\n"
                               "    vec3 Test;\n"
                               "};\n"
                               "void main()\n"
                               "{\n"
                               "    fragTexCoord = TEXCOORD;\n"
                               "    fragColor = COLOR;\n"
                               "    mat4 MVP = Model * View * Projection;\n"
                               "    gl_Position = MVP * vec4(POSITION, 1.0) + vec4(Test, 0.0);\n"
                               "}\n";
                //vertShader = graphicsDevice->createShader(vertSrc, ngine::graphics::ShaderStage::Vertex);

                // Create fragment shader
                auto fragSrc = "#version 300 es\n"
                               "precision mediump float;\n"
                               "in vec2 fragTexCoord;\n"
                               "in vec4 fragColor;\n"
                               "uniform sampler2D Textures[2];\n"
                               "out vec4 outColor;\n"
                               "void main()\n"
                               "{\n"
                               "    vec4 texelColor = texture(Textures[1], fragTexCoord);\n"
                               "    outColor = texelColor*fragColor;\n"
                               "}\n";
                //fragShader = graphicsDevice->createShader(fragSrc, ngine::graphics::ShaderStage::Fragment);
                break;
            }
            case ngine::graphics::ContextType::DirectX: {
                auto dxSrc = "struct VOut {\n"
                             "    float4 position : SV_POSITION;\n"
                             "    float4 color : COLOR;\n"
                             "    float2 texcoord : TEXCOORD;\n"
                             "};\n"
                             "\n"
                             "cbuffer ConstantBuffer : register(b0) {\n"
                             "    matrix Model;\n"
                             "    matrix View;\n"
                             "    matrix Projection;\n"
                             "    float3 Test;\n"
                             "}\n"
                             "\n"
                             "VOut VSMain(float4 pos : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) {\n"
                             "    VOut output;"
                             "    matrix MVP = mul(mul(Model, View), Projection);\n"
                             "    output.position = mul(MVP, pos) + float4(Test, 0.0);\n"
                             "    output.color = color;\n"
                             "    output.texcoord = texcoord;\n"
                             "    return output;\n"
                             "}\n"
                             "\n"
                             "Texture2D Textures[2];\n"
                             "SamplerState Samplers[2];\n"
                             "float4 PSMain(float4 pos : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET {\n"
                             "    float4 texelColor = Textures[1].Sample(Samplers[1], texcoord);"
                             "    return texelColor * color;\n"
                             "}";
                //vertShader = graphicsDevice->createShader(dxSrc, ngine::graphics::ShaderStage::Vertex);
                //fragShader = graphicsDevice->createShader(dxSrc, ngine::graphics::ShaderStage::Fragment);
                break;
            }
            case ngine::graphics::ContextType::Vulkan:
                break;
        }

        // Create vertex buffer from data
        float dat[] = {
                -0.5f, -0.5f, 0, 1, 1, 1, 1, 0, 0,
                0.5f, -0.5f, 0, 1, 1, 1, 1, 1.0f, 0,
                0.0f, 0.5f, 0, 1, 1, 1, 1, 0.5f, 1
        };

        ngine::graphics::BufferDesc vertexBufferDesc;
        vertexBufferDesc.InitialData = dat;
        vertexBufferDesc.Usage = ngine::graphics::BufferUsage::Static;
        vertexBufferDesc.Size = sizeof(dat);

        vertexBuffer = graphicsDevice->createBuffer(vertexBufferDesc);
    }

    void cleanup() override {
        auto graphicsDevice = getGraphicsDevice();

        graphicsDevice->releaseBuffer(vertexBuffer);

        Game::cleanup();
    }

    void draw() override {
        Game::draw();
    }
};

NGINE_GAME_ENTRY {
    ngine::graphics::ContextDescriptor desc = {};
    desc.Type = ngine::graphics::ContextType::DirectX; // OGL not implemented yet.

    // The below only apply to OGL
    if (desc.Type == ngine::graphics::ContextType::OpenGLES) {
        desc.MajorVersion = 3;
        desc.MinorVersion = 0;
    } else if (desc.Type == ngine::graphics::ContextType::OpenGL) {
        desc.MajorVersion = 3;
        desc.MinorVersion = 3;
    }

    ngine::graphics::GraphicsDeviceDesc gdDesc;
    gdDesc.ContextDescriptor = desc;

    ngine::WindowDesc conf;
    conf.Title = "Test";
    conf.Width = 1280;
    conf.Height = 768;
    conf.GraphicsDeviceDesc = gdDesc;

    TestGame game(conf);

#if defined(PLATFORM_UWP)
    CoreApplication::Run(ref new ngine::utils::UWP::BootstrappedGameSource(&game));
#else
    game.run();
#endif

    return 0;
}