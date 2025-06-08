#include "speed_snake.h"


SpeedSnake::SpeedSnake(float x, float y) : Player(x, y) {
}

SpeedSnake::~SpeedSnake() = default;

void SpeedSnake::on_render(const Camera& camera) {

    render_body_smoothly(camera, RGB(50, 198, 220)); // 绘制平滑的蛇身

    render_eyes(camera, BLACK);


    Player::on_render(camera);
}

void SpeedSnake::on_input(const ExMessage& msg){
    switch (msg.vkcode) {
        case VK_SPACE:
            switch (msg.message) {
                case WM_KEYDOWN:
                    if (!is_speed_up) {
                        current_speed += delta_speed;
                        is_speed_up = true;
                    }
                    break;
                case WM_KEYUP:
                    if (is_speed_up) {
                        current_speed -= delta_speed;
                        is_speed_up = false;
                    }
                        
                    break;
            }
            break;
    }
    Player::on_input(msg);
}

