#pragma once

#include "../tool/camera.h"
#include "../manager/collision_manager.h"

#include <graphics.h>
#include <string>

class Scene {
private:

public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void on_enter() = 0; // ��������ʱ����
    virtual void on_exit() = 0; // �����˳�ʱ����
    virtual void on_update(float delta) = 0; // ��������ʱ����
    virtual void on_render(const Camera& camera) = 0; // ������Ⱦʱ����
    virtual void on_input(const ExMessage& msg) = 0; // ��������ʱ����

};