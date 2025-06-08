#pragma once

#include "../player.h"


class SpeedSnake : public Player {
private:
    bool is_speed_up = false; // 是否加速
    float delta_speed = 80.0f; // 加速速度
        
public:
    SpeedSnake(float x = 1080 / 2, float y = 720 / 2); // 初始位置在屏幕中心
    ~SpeedSnake();


    void on_render(const Camera& camera) override;
    void on_input(const ExMessage& msg) override;
};
