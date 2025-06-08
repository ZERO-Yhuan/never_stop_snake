#include "normal_snake_p2.h"


NormalSnakeP2::NormalSnakeP2(float x, float y) : Player(x, y) {
}

NormalSnakeP2::~NormalSnakeP2() = default;

void NormalSnakeP2::on_render(const Camera& camera){

    render_body_smoothly(camera, RGB(0, 0, 139)); // 绘制平滑的蛇身

    render_eyes(camera,BLACK);

    
    Player::on_render(camera);
}

void NormalSnakeP2::on_input(const ExMessage& msg) {
    if (!is_alive)return;

    switch (msg.message)
    {
    case WM_KEYDOWN:
        switch (msg.vkcode)
        {
        case 0x25: // Left
            if (face_dir == FaceDirection::Right) return; // 如果当前面向右，则不能按A键
            is_left_key_down = true;
            break;
        case 0x27: // Right
            if (face_dir == FaceDirection::Left) return; // 如果当前面向左，则不能按D键
            is_right_key_down = true;
            break;
        case 0x26: // Up
            if (face_dir == FaceDirection::Down) return; // 如果当前面向下，则不能按W键
            is_up_key_down = true;
            break;
        case 0x28: // Down
            if (face_dir == FaceDirection::Up) return; // 如果当前面向上，则不能按S键
            is_down_key_down = true;
            break;
        }
        break;
    case WM_KEYUP:
        switch (msg.vkcode)
        {
        case 0x25: // Left
            is_left_key_down = false;
            break;
        case 0x27: // Right
            is_right_key_down = false;
            break;
        case 0x26: // Up
            is_up_key_down = false;
            break;
        case 0x28: // Down
            is_down_key_down = false;
            break;
        }
        break;
    }
}

