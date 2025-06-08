#pragma once

#pragma once

#include "../player.h"


class NormalSnakeP2 : public Player {
public:
    NormalSnakeP2(float x = 1080 / 2, float y = 720 / 2); // 初始位置在屏幕中心
    ~NormalSnakeP2();


    void on_render(const Camera& camera) override;
    void on_input(const ExMessage& msg) override;
};
