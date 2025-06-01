#pragma once

#include "../tool/camera.h"
#include "../manager/collision_manager.h"
#include "../ui/custom/button.h"

#include <graphics.h>
#include <string>

class Scene {
private:

public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void on_enter() = 0; // 场景进入时调用
    virtual void on_exit() = 0; // 场景退出时调用
    virtual void on_update(const float delta) = 0; // 场景更新时调用
    virtual void on_render(const Camera& camera) = 0; // 场景渲染时调用
    virtual void on_input(const ExMessage& msg) = 0; // 场景输入时调用
    virtual void set_scene_state(const int scene_state) {} // 设置场景状态
};