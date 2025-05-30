#pragma once

#include "../scene.h"
#include "../../tool/timer.h"
#include "../../manager/scene_manager.h"
#include "../../manager/resources_manager.h"
#include "../../resource/animation.h"
#include "../../ui/custom/button.h"



class MenuScene : public Scene {
private:
    Animation snake_menu_animation;
    std::vector<Button> buttons;

    int select_button = 0;

public:

    MenuScene() {
        buttons.clear();
        buttons.reserve(3);
        buttons.emplace_back(50, 100, 150, 80, _T("开始游戏"), [&]() {
            SceneManager::instance()->switch_scene_to(SceneManager::SceneType::Game);
            });
        buttons.emplace_back(50, 200, 150, 80, _T("设置"));
        buttons.emplace_back(50, 300, 150, 80, _T("退出游戏"), [&]() {
            exit(0);
            });

        snake_menu_animation.set_position(Vector2(getwidth() / 2, getheight() / 2));
        snake_menu_animation.set_interval(0.5f);
        snake_menu_animation.set_loop(true);
        snake_menu_animation.add_frame(ResourcesManager::instance()->find_atlas("snake_menu"));
    }

    void on_enter() {

        buttons[select_button].set_selected(true);
    }
    void on_exit() {
        buttons[select_button].set_selected(false);
        select_button = 0;
    }
    void on_update(float delta) {
        snake_menu_animation.on_update(delta);
    }
    void on_render(const Camera& camera) {
        Rect rect_dst(0, 0, 1080, 720);
        putimage_alpha(ResourcesManager::instance()->find_image("background"), &rect_dst);

        snake_menu_animation.on_render(camera);
        

        for (Button& button : buttons)
            button.on_render();
        
    }
    void on_input(const ExMessage& msg) {
        switch (msg.message)
        {
        case WM_KEYDOWN:
            switch (msg.vkcode)
            {
            case 0x53: // VK_S
            case 0x28: // VK_DOWN

                buttons[select_button].set_selected(false);
                select_button = (select_button + 1) == buttons.size() ? 0 : (select_button + 1);
                buttons[select_button].set_selected(true);
                break;
            case 0x57: // VK_W
            case 0x26: // VK_UP
                buttons[select_button].set_selected(false);
                select_button = (select_button - 1) == -1 ? (buttons.size() - 1) : select_button - 1;
                buttons[select_button].set_selected(true);
                break;
            case 0x20: // VK_SPACE
            case 0x0D: // VK_RETURN
                buttons[select_button].start_clicked();
                break;
            }
            break;
        case WM_KEYUP:
            switch (msg.vkcode)
            {
            case 0x1B: // VK_ESCAPE
                SceneManager::instance()->switch_scene_to(SceneManager::SceneType::Start);
                break;
            case 0x20: // VK_SPACE
            case 0x0D: // VK_RETURN
                buttons[select_button].end_clicked();
                break;
            }
            break;
        }


    }

};