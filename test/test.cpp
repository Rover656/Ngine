#include <ngine/ngine.hpp>

#include <ngine/game.hpp>

#include <ngine/graphics/buffer.hpp>
#include <ngine/graphics/graphics_device.hpp>
#include <ngine/graphics/shader.hpp>
#include <ngine/graphics/shader_program.hpp>
#include <ngine/graphics/vertex_array.hpp>

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

        // Create vertex shader
        auto vertSrc = "#version 330\n"
                       "layout(location = 2) in vec3 POSITION;\n"
                       "in vec2 TEXCOORD;\n"
                       "in vec4 COLOR;\n"
                       "out vec2 fragTexCoord;\n"
                       "out vec4 fragColor;\n"
                       "uniform mat4 NGINE_MATRIX_MVP;\n"
                       "void main()\n"
                       "{\n"
                       "    fragTexCoord = TEXCOORD;\n"
                       "    fragColor = vec4(1, 1, 1, 1);//COLOR;\n"
                       "    gl_Position = vec4(POSITION, 1.0);//NGINE_MATRIX_MVP * vec4(POSITION, 1.0);\n"
                       "}";
        vertShader = new ngine::graphics::Shader(getGraphicsDevice(), vertSrc, ngine::graphics::ShaderType::Vertex);

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
        fragShader = new ngine::graphics::Shader(getGraphicsDevice(), fragSrc, ngine::graphics::ShaderType::Fragment);

        // Create shader program
        prog = new ngine::graphics::ShaderProgram(getGraphicsDevice());
        prog->attachShader(vertShader);
        prog->attachShader(fragShader);
        prog->link();

        vb = new ngine::graphics::Buffer(getGraphicsDevice(), ngine::graphics::BufferUsage::Static);
        float dat[] = {
                0, 0, 0,
                1, 0, 0,
                1, 1, 0
        };
        vb->write(ngine::graphics::BufferType::Vertex, dat, 9);

        ngine::graphics::VertexBufferLayout l;
        l.Elements.push_back({
            "POSITION", ngine::graphics::ElementType::Float, 3, false
        });

        array = new ngine::graphics::VertexArray(getGraphicsDevice(), vb, nullptr, l);
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
    ngine::WindowConfig conf;
    conf.Title = "Test";
    conf.Width = 1280;
    conf.Height = 768;

    TestGame game(conf);
    game.run();

    return 0;
}