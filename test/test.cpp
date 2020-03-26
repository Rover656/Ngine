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
                               "uniform sampler2D NGINE_TEXTURE;\n"
                               "void main()\n"
                               "{\n"
                               "    vec4 texelColor = texture(NGINE_TEXTURE, fragTexCoord);\n"
                               "    finalColor = fragColor;//texelColor*fragColor;\n"
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
                             "};\n"
                             "\n"
                             "VOut VSMain(float4 pos : POSITION, float4 color : COLOR) {\n"
                             "    VOut output;"
                             "    output.position = pos;\n"
                             "    output.color = color;\n"
                             "    return output;\n"
                             "}\n"
                             "\n"
                             "float4 PSMain(float4 pos : SV_POSITION, float4 color : COLOR) : SV_TARGET {\n"
                             "    return color;\n"
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
        shaderLayout.Elements.push_back({"POSITION", ngine::graphics::ElementType::Float, 3, false});
        shaderLayout.Elements.push_back({"COLOR", ngine::graphics::ElementType::Float, 4, false});

        // Create shader program
        prog = new ngine::graphics::ShaderProgram(getGraphicsDevice(), shaderLayout);
        prog->attachShader(vertShader);
        prog->attachShader(fragShader);
        prog->link();

        vb = new ngine::graphics::Buffer(getGraphicsDevice(), ngine::graphics::BufferType::Vertex,
                                         ngine::graphics::BufferUsage::Static, sizeof(float) * (3 + 4), 3);

        ngine::Vector2 v1(-0.5f, -0.5f);
        ngine::Vector2 v2(0.5f, -0.5f);
        ngine::Vector2 v3(0.0f, 0.5f);

        float dat[] = {
                v1.X, v1.Y, 0,   1, 0, 1, 1,
                v2.X, v2.Y, 0,   1, 1, 0, 1,
                v3.X, v3.Y, 0,   0, 1, 1, 1
        };
        vb->write(dat, sizeof(float) * (3 + 4), 3);

        array = new ngine::graphics::VertexArray(getGraphicsDevice(), vb, nullptr, shaderLayout);
    }

    void cleanup() override {
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
        gd->drawPrimitives(ngine::graphics::PrimitiveType::Triangles, 0, 3);

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