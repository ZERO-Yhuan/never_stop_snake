#include "normal_snake.h"


NormalSnake::NormalSnake(float x, float y) : Player(x, y) {
}

NormalSnake::~NormalSnake() = default;

void NormalSnake::on_render(const Camera& camera){

    render_body_smoothly(camera, RGB(237, 147, 25)); // 绘制平滑的蛇身

    render_eyes(camera,BLACK);

    
    Player::on_render(camera);
}

