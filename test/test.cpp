#include <ngine/ngine.hpp>

#include <ngine/game.hpp>

#include <ngine/graphics/buffer.hpp>
#include <ngine/graphics/graphics_device.hpp>
#include <ngine/graphics/shader.hpp>
#include <ngine/graphics/shader_program.hpp>
#include <ngine/graphics/vertex_array.hpp>
#include <ngine/math.hpp>

class TestGame : public ngine::Game {
    ngine::graphics::Buffer *vb;
    ngine::graphics::VertexArray *array;
    ngine::graphics::Shader *vertShader;
    ngine::graphics::Shader *fragShader;
    ngine::graphics::ShaderProgram *prog;
    ngine::graphics::Texture2D *tex;
    ngine::graphics::SamplerState *samplerState;
public:
    TestGame(ngine::WindowConfig conf) : Game(conf) {}

    void init() override {
        Game::init();

        // Get graphics API
        auto api = getWindow()->getContextDescriptor().Type;

        // Create shaders
        switch (api) {
            case ngine::graphics::ContextType::OpenGL: {
                // Create vertex shader
                auto vertSrc = "#version 130\n"
                               "in vec3 POSITION;\n"
                               "in vec2 TEXCOORD;\n"
                               "in vec4 COLOR;\n"
                               "out vec2 fragTexCoord;\n"
                               "out vec4 fragColor;\n"
                               "uniform mat4 NGINE_MATRIX_MVP;\n"
                               "void main()\n"
                               "{\n"
                               "    fragTexCoord = TEXCOORD;\n"
                               "    fragColor = COLOR;\n"
                               "    gl_Position = vec4(POSITION, 1.0);//NGINE_MATRIX_MVP * vec4(POSITION, 1.0);\n"
                               "}";
                vertShader = new ngine::graphics::Shader(getGraphicsDevice(), vertSrc,
                                                         ngine::graphics::ShaderType::Vertex);

                // Create fragment shader
                auto fragSrc = "#version 130\n"
                               "in vec2 fragTexCoord;\n"
                               "in vec4 fragColor;\n"
                               "out vec4 finalColor;\n"
                               "uniform sampler2D Textures[2];\n"
                               "void main()\n"
                               "{\n"
                               "    vec4 texelColor = texture(Textures[1], fragTexCoord);\n"
                               "    finalColor = texelColor*fragColor;\n"
                               "}";
                fragShader = new ngine::graphics::Shader(getGraphicsDevice(), fragSrc,
                                                         ngine::graphics::ShaderType::Fragment);
                break;
            }
            case ngine::graphics::ContextType::OpenGLES:
                break;
            case ngine::graphics::ContextType::DirectX: {
                auto dxSrc = "struct VOut {\n"
                             "    float4 position : SV_POSITION;\n"
                             "    float4 color : COLOR;\n"
                             "    float2 texcoord : TEXCOORD;\n"
                             "};\n"
                             "\n"
                             "VOut VSMain(float4 pos : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) {\n"
                             "    VOut output;"
                             "    output.position = pos;\n"
                             "    output.color = color;\n"
                             "    output.texcoord = texcoord;\n"
                             "    return output;\n"
                             "}\n"
                             "\n"
                             "Texture2D Textures[2];\n"
                             "SamplerState Samplers[2];\n"
                             "float4 PSMain(float4 pos : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET {\n"
                             "    float4 texelColor = Textures[1].Sample(Samplers[1], texcoord);"
                             "    return texelColor;// * color;\n"
                             "}";
                vertShader = new ngine::graphics::Shader(getGraphicsDevice(), dxSrc,
                                                         ngine::graphics::ShaderType::Vertex);
                fragShader = new ngine::graphics::Shader(getGraphicsDevice(), dxSrc,
                                                         ngine::graphics::ShaderType::Fragment);
                break;
            }
            case ngine::graphics::ContextType::Vulkan:
                break;
        }

        // Setup shader layout
        ngine::graphics::VertexBufferLayout shaderLayout;
        shaderLayout.Elements.push_back({"POSITION", ngine::graphics::ElementType::Float, ngine::graphics::ElementUse::Position, 3, false});
        shaderLayout.Elements.push_back({"COLOR", ngine::graphics::ElementType::Float, ngine::graphics::ElementUse::Color, 4, false});
        shaderLayout.Elements.push_back({"TEXCOORD", ngine::graphics::ElementType::Float, ngine::graphics::ElementUse::Texcoords, 2, false});

        // Create shader program
        prog = new ngine::graphics::ShaderProgram(getGraphicsDevice(), shaderLayout);
        prog->attachShader(vertShader);
        prog->attachShader(fragShader);
        prog->link();
        prog->ExpectedSamplerCount = 2;

        vb = new ngine::graphics::Buffer(getGraphicsDevice(), ngine::graphics::BufferType::Vertex,
                                         ngine::graphics::BufferUsage::Static, sizeof(float) * (3 + 4 + 2), 3);

        float dat[] = {
                -0.5f, -0.5f, 0,   1, 1, 1, 1,   0, 0,
                0.5f,  -0.5f, 0,   1, 1, 1, 1,   1.0f, 0,
                0.0f,  0.5f,  0,   1, 1, 1, 1,   0.5f, 1
        };
        vb->write(dat, sizeof(float) * (3 + 4 + 2), 3);

        array = new ngine::graphics::VertexArray(getGraphicsDevice(), vb, nullptr, shaderLayout);

        unsigned char imgdat[] = {
                255, 255, 255, 255,
                0  , 255, 255, 255,
                255, 0  , 255, 255,
                255, 255,   0, 255
        };
        tex = new ngine::graphics::Texture2D(getGraphicsDevice(), 2, 2, imgdat, ngine::graphics::PixelFormat::R8G8B8A8);

        samplerState = new ngine::graphics::SamplerState(getGraphicsDevice());
        samplerState->Filter = ngine::graphics::TextureFilter::Point;
    }

    void cleanup() override {
        delete tex;
        delete samplerState;
        delete vb;
        delete array;
        delete vertShader;
        delete fragShader;
        delete prog;

        Game::cleanup();
    }

    void draw() override {
        prog->use();
        auto gd = getGraphicsDevice();
        gd->bindVertexArray(array);
        gd->bindSamplerState(1, samplerState);
        gd->bindTexture(1, tex);
        gd->drawPrimitives(ngine::graphics::PrimitiveType::TriangleList, 0, 3);

        Game::draw();
    }
};

NGINE_GAME_ENTRY {
    ngine::graphics::ContextDescriptor desc;
    desc.Type = ngine::graphics::ContextType::DirectX;

    // The below only apply to OGL
    desc.MajorVersion = 4;
    desc.MinorVersion = 6;

    ngine::WindowConfig conf;
    conf.Title = "Test";
    conf.Width = 1280;
    conf.Height = 768;
    conf.ContextDescriptor = desc;

    TestGame game(conf);
    game.run();

    return 0;
}