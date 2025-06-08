#include "normal_snake_p2.h"


NormalSnakeP2::NormalSnakeP2(float x, float y) : Player(x, y) {
}

NormalSnakeP2::~NormalSnakeP2() = default;

void NormalSnakeP2::on_render(const Camera& camera){

    render_body_smoothly(camera, RGB(0, 0, 139)); // ����ƽ��������

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
            if (face_dir == FaceDirection::Right) return; // �����ǰ�����ң����ܰ�A��
            is_left_key_down = true;
            break;
        case 0x27: // Right
            if (face_dir == FaceDirection::Left) return; // �����ǰ���������ܰ�D��
            is_right_key_down = true;
            break;
        case 0x26: // Up
            if (face_dir == FaceDirection::Down) return; // �����ǰ�����£����ܰ�W��
            is_up_key_down = true;
            break;
        case 0x28: // Down
            if (face_dir == FaceDirection::Up) return; // �����ǰ�����ϣ����ܰ�S��
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

