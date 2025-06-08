#pragma once

#include "../scene/scene.h"

extern Scene* start_scene;
extern Scene* menu_scene;
extern Scene* game_scene;
extern Scene* double_player_game_scene;
extern Scene* settings_scene;

class SceneManager {
private:
    Scene* current_scene = nullptr; // 当前场景

    static SceneManager* _instance; // 单例实例

public:
    // 场景枚举
    enum SceneType 
    { 
        Start,
        Menu,
        Game,
        DoublePlayerGame,
        Settings
    };


private:
    SceneManager();
    ~SceneManager();

public:
    static SceneManager* instance();

    void set_current_scene(Scene* scene); // 设置当前场景

    void switch_scene_to(SceneType type, int state = 0); // 切换场景

    void on_update(float delta); // 更新当前场景

    void on_render(const Camera& camera); // 渲染当前场景

    void on_input(const ExMessage& msg); // 处理输入

};