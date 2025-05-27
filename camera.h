#pragma once

#include "vector2.h"
#include "timer.h"

#include <iostream>

class Camera {
private:
    Vector2 position; 
    Timer timer_shake;
    bool is_shaking = false;
    float shake_intensity = 0.0f; // 抖动强度

public:

    Camera() {
        timer_shake.set_one_shot(true);
        timer_shake.set_on_timeout([this] { // 抖动结束时的回调函数
            is_shaking = false;
            reset();
            });
    }
    ~Camera() = default;

    const Vector2& get_position() const { return position; }

    // 重置摄像机位置
    void reset() { 
        position.x = 0;
        position.y = 0;
    }
    // 摄像机逻辑更新代码
    void on_update(float delta) {
        timer_shake.on_update(delta); // 更新抖动计时器

        if (is_shaking) { // 随机抖动
            position.x = (-50 + rand() % 100) / 50.0F * shake_intensity; 
            position.y = (-50 + rand() % 100) / 50.0F * shake_intensity;
        }
    }

    /*
    *  “intensity”: 抖动强度
    *  “duration”: 抖动持续时间
    */
    void shake(float intensity, float duration) {
        is_shaking = true;
        shake_intensity = intensity;
        timer_shake.set_wait_time(duration);
        timer_shake.restart(); // 启动抖动计时器
    }

};