#include "../include/ConfigParser.h"
#include "../include/SceneParser.h"
#include "../include/Renderer.h"

int main(int, char**)
{
    Config config = ConfigParser::GetInstance().ParseConfig("../config/Config.xml");

    Scene scene = SceneParser::GetInstance().ParseScene(config.scene_description_path_);

    // Computation necessary for scene constructions, generating samples for samplers, etc
    scene.world_->Build(); // TODO: Figure out where to put this
    // Maye need to generate samples for each sampler instance here

    Canvas canvas {config.width_, config.height_};

    Renderer::GetInstance().Render(scene, canvas);

    canvas.Flush(config.output_path_);
}