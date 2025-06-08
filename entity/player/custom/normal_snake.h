#pragma once

#include "../player.h"


class NormalSnake : public Player {
public:
    NormalSnake(float x = 1080 / 2, float y = 720 / 2); // 初始位置在屏幕中心
    ~NormalSnake();


    void on_render(const Camera& camera) override;

};
