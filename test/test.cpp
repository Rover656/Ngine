#include <ngine/ngine.hpp>

#include <ngine/game.hpp>

#include <ngine/graphics/graphics_device.hpp>
#include <ngine/math.hpp>

#if defined(PLATFORM_UWP)
#include <ngine/utils/UWP/uwp_bootstrap.hpp>
#endif

class TestGame : public ngine::Game {
    ngine::graphics::Buffer *vb;
    ngine::graphics::Buffer *uniformBuffer;
    ngine::graphics::IUniformDataManager *uniformData;
    ngine::graphics::VertexArray *array;
    ngine::graphics::Shader *vertShader;
    ngine::graphics::Shader *fragShader;
    ngine::graphics::ShaderProgram *prog;
    ngine::graphics::Image *img;
    ngine::graphics::Texture2D *tex;
    ngine::graphics::SamplerState *samplerState;
public:
    TestGame(ngine::WindowConfig conf) : Game(conf) {}

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
                vertShader = new ngine::graphics::Shader(getGraphicsDevice(), vertSrc,
                                                         ngine::graphics::ShaderStage::Vertex);

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
                fragShader = new ngine::graphics::Shader(getGraphicsDevice(), fragSrc,
                                                         ngine::graphics::ShaderStage::Fragment);
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
                vertShader = new ngine::graphics::Shader(getGraphicsDevice(), vertSrc,
                                                         ngine::graphics::ShaderStage::Vertex);

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
                fragShader = new ngine::graphics::Shader(getGraphicsDevice(), fragSrc,
                                                         ngine::graphics::ShaderStage::Fragment);
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
                vertShader = new ngine::graphics::Shader(getGraphicsDevice(), dxSrc,
                                                         ngine::graphics::ShaderStage::Vertex);
                fragShader = new ngine::graphics::Shader(getGraphicsDevice(), dxSrc,
                                                         ngine::graphics::ShaderStage::Fragment);
                break;
            }
            case ngine::graphics::ContextType::Vulkan:
                break;
        }

        // Setup shader layout
        ngine::graphics::BufferLayout shaderVertexLayout;
        shaderVertexLayout.Elements.push_back(
                {"POSITION", ngine::graphics::ElementType::Float, ngine::graphics::ElementUse::Position, 3, false});
        shaderVertexLayout.Elements.push_back(
                {"COLOR", ngine::graphics::ElementType::Float, ngine::graphics::ElementUse::Color, 4, false});
        shaderVertexLayout.Elements.push_back(
                {"TEXCOORD", ngine::graphics::ElementType::Float, ngine::graphics::ElementUse::Texcoords, 2, false});

        // Create shader program
        prog = new ngine::graphics::ShaderProgram(getGraphicsDevice(), shaderVertexLayout);
        prog->attachShader(vertShader);
        prog->attachShader(fragShader);
        prog->link();
        prog->ExpectedSamplerCount = 2;

        // Create uniform data
        std::vector<ngine::graphics::Uniform> uniforms;
        uniforms.push_back({"Model", ngine::graphics::ElementType::Matrix, 1});
        uniforms.push_back({"View", ngine::graphics::ElementType::Matrix, 1});
        uniforms.push_back({"Projection", ngine::graphics::ElementType::Matrix, 1});
        uniforms.push_back({"Test", ngine::graphics::ElementType::Float, 3});

        uniformData = graphicsDevice->createUniformDataManager(uniforms);

        // Set uniform data
        ngine::Matrix test = ngine::Matrix::Identity;
        uniformData->setUniform("Model", test);
        uniformData->setUniform("View", test);
        uniformData->setUniform("Projection", test);
        uniformData->setUniform("Test", ngine::Vector3(0.2f, 0.2f, 0.0f));

        // Create uniform buffer
        uniformBuffer = new ngine::graphics::Buffer(getGraphicsDevice(), ngine::graphics::BufferType::Uniform,
                                                    ngine::graphics::BufferUsage::Dynamic, uniformData->getData(),
                                                    uniformData->getDataSize(), 1);

        // Create vertex buffer from data
        float dat[] = {
                -0.5f, -0.5f, 0, 1, 1, 1, 1, 0, 0,
                0.5f, -0.5f, 0, 1, 1, 1, 1, 1.0f, 0,
                0.0f, 0.5f, 0, 1, 1, 1, 1, 0.5f, 1
        };
        vb = new ngine::graphics::Buffer(getGraphicsDevice(), ngine::graphics::BufferType::Vertex,
                                         ngine::graphics::BufferUsage::Static, &dat[0], sizeof(float) * (3 + 4 + 2), 3);

        // Create vertex array
        array = new ngine::graphics::VertexArray(getGraphicsDevice(), vb, nullptr, shaderVertexLayout);

        // Create image from array
        unsigned char imgdat[] = {
                255, 255, 255, 255,
                0, 255, 255, 255,
                255, 0, 255, 255,
                255, 255, 0, 255
        };

        img = new ngine::graphics::Image(2, 2, imgdat, ngine::graphics::PixelFormat::R8G8B8A8);

        // Create texture from image
        tex = new ngine::graphics::Texture2D(getGraphicsDevice(), img);

        // Create sampler state
        samplerState = new ngine::graphics::SamplerState(getGraphicsDevice());
        samplerState->Filter = ngine::graphics::TextureFilter::Point;
    }

    void cleanup() override {
        delete tex;
        delete img;
        delete samplerState;
        delete vb;
        delete array;
        delete uniformData;
        delete uniformBuffer;
        delete vertShader;
        delete fragShader;
        delete prog;

        Game::cleanup();
    }

    void draw() override {
        prog->use();
        array->bind();
        samplerState->bind(1);
        tex->bind(1);

        auto gd = getGraphicsDevice();
        gd->bindUniformBuffer(0, uniformBuffer);
        gd->drawPrimitives(ngine::graphics::PrimitiveType::TriangleList, 0, 3);

        Game::draw();
    }
};

NGINE_GAME_ENTRY {
    ngine::graphics::ContextDescriptor desc = {};
    desc.Type = ngine::graphics::ContextType::DirectX;

    // The below only apply to OGL
    if (desc.Type == ngine::graphics::ContextType::OpenGLES) {
        desc.MajorVersion = 3;
        desc.MinorVersion = 0;
    } else if (desc.Type == ngine::graphics::ContextType::OpenGL) {
        desc.MajorVersion = 3;
        desc.MinorVersion = 3;
    }

    ngine::WindowConfig conf;
    conf.Title = "Test";
    conf.Width = 1280;
    conf.Height = 768;
    conf.ContextDescriptor = desc;

    TestGame game(conf);

#if defined(PLATFORM_UWP)
    CoreApplication::Run(ref new ngine::utils::UWP::BootstrappedGameSource(&game));
#else
    game.run();
#endif

    return 0;
}