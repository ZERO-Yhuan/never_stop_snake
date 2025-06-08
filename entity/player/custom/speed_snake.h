#pragma once

#include "../player.h"


class SpeedSnake : public Player {
private:
    bool is_speed_up = false; // �Ƿ����
    float delta_speed = 80.0f; // �����ٶ�
        
public:
    SpeedSnake(float x = 1080 / 2, float y = 720 / 2); // ��ʼλ������Ļ����
    ~SpeedSnake();


    void on_render(const Camera& camera) override;
    void on_input(const ExMessage& msg) override;
};
