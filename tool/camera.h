#pragma once

#include "vector2.h"
#include "timer.h"

#include <iostream>

class Camera {
private:
    Vector2 position; 
    Timer timer_shake;
    bool is_shaking = false;
    float shake_intensity = 0.0f; // ����ǿ��

public:

    Camera() {
        timer_shake.set_one_shot(true);
        timer_shake.set_on_timeout([this] { // ��������ʱ�Ļص�����
            is_shaking = false;
            reset();
            });
    }
    ~Camera() = default;

    const Vector2& get_position() const { return position; }

    // ���������λ��
    void reset() { 
        position.x = 0;
        position.y = 0;
    }
    // ������߼����´���
    void on_update(float delta) {
        timer_shake.on_update(delta); // ���¶�����ʱ��

        if (is_shaking) { // �������
            position.x = (-50 + rand() % 100) / 50.0F * shake_intensity; 
            position.y = (-50 + rand() % 100) / 50.0F * shake_intensity;
        }
    }

    /*
    *  ��intensity��: ����ǿ��
    *  ��duration��: ��������ʱ��
    */
    void shake(float intensity, float duration) {
        is_shaking = true;
        shake_intensity = intensity;
        timer_shake.set_wait_time(duration);
        timer_shake.restart(); // ����������ʱ��
    }

};