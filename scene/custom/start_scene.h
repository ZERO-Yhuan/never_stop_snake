#pragma once

#include "../../tool/util.h"
#include "../scene.h"
#include "../../manager/scene_manager.h"
#include "../../manager/resources_manager.h"
#include "../../resource/animation.h"


class StartScene : public Scene
{
private:
    LPCTSTR welcome_text = _T("按任意键开始游戏");
    Animation tree_animation; // 树动画

public:
    StartScene() = default;
    ~StartScene() = default;

    void on_enter() {
        tree_animation.add_frame(ResourcesManager::instance()->find_atlas("tree"));
        tree_animation.set_interval(0.3f);
        tree_animation.set_loop(true);
        tree_animation.set_position(Vector2(getwidth() - ResourcesManager::instance()->find_atlas("tree")->get_image(0)->getwidth() / 2,
            getheight() - ResourcesManager::instance()->find_atlas("tree")->get_image(0)->getheight() / 2));
        
        set_volume(_T("start_menu_bgm"), 30);
        play_audio(_T("start_menu_bgm"), true);
    }
    void on_exit() {

    }
    void on_update(const float delta) {
        tree_animation.on_update(delta);
    }
    void on_render(const Camera& camera) {

        Rect rect_dst(0, 0, 1080, 720);
        putimage_alpha(ResourcesManager::instance()->find_image("background"), &rect_dst);

        outtextxy_shaded((getwidth() - textwidth(welcome_text)) / 2, (getheight() - textheight(welcome_text)) / 4 * 3, welcome_text);
        tree_animation.on_render(camera);
    }
    void on_input(const ExMessage& msg) {
        if(msg.message == WM_KEYUP) {
            play_audio(_T("click"));
            SceneManager::instance()->switch_scene_to(SceneManager::SceneType::Menu);
        }
    }
};