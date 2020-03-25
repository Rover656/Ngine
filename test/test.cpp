#include <ngine/ngine.hpp>

#include <ngine/game.hpp>

#include <ngine/graphics/buffer.hpp>
#include <ngine/graphics/graphics_device.hpp>
#include <ngine/graphics/shader.hpp>
#include <ngine/graphics/shader_program.hpp>

class TestGame : public ngine::Game {
    ngine::graphics::Buffer *vb;
    ngine::graphics::Shader *vertShader;
    ngine::graphics::Shader *fragShader;
    ngine::graphics::ShaderProgram *prog;
public:
    TestGame(ngine::WindowConfig conf) : Game(conf) {}

    void init() override {
        Game::init();

        // Create vertex shader
        auto vertSrc = "#version 130\n"
                       "in vec3 NG_VertexPos;\n"
                       "in vec2 NG_VertexTexCoord;\n"
                       "in vec4 NG_VertexColor;\n"
                       "out vec2 fragTexCoord;\n"
                       "out vec4 fragColor;\n"
                       "uniform mat4 NGINE_MATRIX_MVP;\n"
                       "void main()\n"
                       "{\n"
                       "    fragTexCoord = NG_VertexTexCoord;\n"
                       "    fragColor = NG_VertexColor;\n"
                       "    gl_Position = NGINE_MATRIX_MVP * vec4(NG_VertexPos, 1.0);\n"
                       "}\n";
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
                       "    finalColor = texelColor*fragColor;\n"
                       "}\n";
        fragShader = new ngine::graphics::Shader(getGraphicsDevice(), fragSrc, ngine::graphics::ShaderType::Fragment);

        // Create shader program
        prog = new ngine::graphics::ShaderProgram(getGraphicsDevice());
        prog->attachShader(vertShader);
        prog->attachShader(fragShader);
        prog->link();

        vb = new ngine::graphics::Buffer(getGraphicsDevice(), ngine::graphics::BufferUsage::Static);
        int dat[] = {
                0, 0,
                1, 0,
                1, 1
        };
        vb->write(ngine::graphics::BufferType::Vertex, dat, 3);
    }

    void draw() override {
        auto gd = getGraphicsDevice();
        gd->bindBuffer(ngine::graphics::BufferType::Vertex, vb);
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