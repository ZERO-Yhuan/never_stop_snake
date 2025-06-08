#pragma once

#include "../scene.h"
#include "../../tool/timer.h"
#include "../../manager/scene_manager.h"
#include "../../manager/resources_manager.h"
#include "../../resource/animation.h"
#include "../../tool/snake_type_handle.h"



class MenuScene : public Scene {
private:
    Animation snake_menu_animation; // �ߵĶ���
    std::vector<Button> buttons; // �˵���ť
    std::vector<Button> game_chose_buttons; // ��Ϸģʽѡ��ť

    bool is_chose_game = false;  // �Ƿ��ڽ�����Ϸģʽѡ��

    int select_button = 0; // ��ǰѡ��İ�ť�±�



public:

    MenuScene() {
        buttons.clear();
        buttons.reserve(3);
        buttons.emplace_back(50, 100, 150, 80, _T("��ʼ��Ϸ"), [&]() {
            is_chose_game = true;
            game_chose_buttons[select_button].set_selected(true);
            });
        buttons.emplace_back(50, 200, 150, 80, _T("����"), [&] {
            SceneManager::instance()->switch_scene_to(SceneManager::SceneType::Settings);
            });
        buttons.emplace_back(50, 300, 150, 80, _T("�˳���Ϸ"), [&]() {
            exit(0);
            });

        game_chose_buttons.emplace_back(50, 100, 150, 80, _T("�޾�ģʽ"), [&]() {
            SceneManager::instance()->switch_scene_to(SceneManager::SceneType::Game, 0);
            });
        game_chose_buttons.emplace_back(50, 200, 150, 80, _T("��ʱģʽ"), [&]() {
            SceneManager::instance()->switch_scene_to(SceneManager::SceneType::Game, 1);
            });
        game_chose_buttons.emplace_back(50, 300, 150, 80, _T("����ģʽ"), [&]() {
            SceneManager::instance()->switch_scene_to(SceneManager::SceneType::DoublePlayerGame);
            });
        game_chose_buttons.emplace_back(50, 400, 150, 80, _T("����"), [&]() {
            game_chose_buttons[select_button].set_selected(false);
            on_exit();
            on_enter();
            });
        game_chose_buttons.emplace_back(50, 0, 150, 80, _T("��ͨ��"), [&]() {
            SnakeTypeHandle::instance()->current_snake_type = 
                (SnakeTypeHandle::instance()->current_snake_type + 1) == SnakeTypeHandle::instance()->Max_Snake_Type ? 0 : SnakeTypeHandle::instance()->current_snake_type + 1;
            game_chose_buttons[select_button].set_text(
                SnakeTypeHandle::instance()->snake_type_names[SnakeTypeHandle::instance()->current_snake_type]);
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
        if(!is_chose_game)
            buttons[select_button].set_selected(false);
        else
            game_chose_buttons[select_button].set_selected(false);
        is_chose_game = false;
        select_button = 0;
    }
    void on_update(const float delta) {
        snake_menu_animation.on_update(delta);
    }
    void on_render(const Camera& camera) {
        Rect rect_dst(0, 0, 1080, 720);
        putimage_alpha(ResourcesManager::instance()->find_image("background"), &rect_dst); // ����

        snake_menu_animation.on_render(camera); // ��Ⱦ�߶���
        
        if (!is_chose_game) { // ��Ⱦ�˵���ť
            for (Button& button : buttons)
                button.on_render();
        } 
        else { // ��Ⱦ��Ϸģʽѡ��ť
            for (Button& button : game_chose_buttons)
                button.on_render();
        }
        
    }
    void on_input(const ExMessage& msg) {
        switch (msg.message)
        {
        case WM_KEYDOWN:
            switch (msg.vkcode)
            {
            case 0x53: // VK_S
            case 0x28: // VK_DOWN
                if (!is_chose_game) {
                    buttons[select_button].set_selected(false);
                    select_button = (select_button + 1) == buttons.size() ? 0 : (select_button + 1);
                    buttons[select_button].set_selected(true);
                }
                else {
                    game_chose_buttons[select_button].set_selected(false);
                    select_button = (select_button + 1) == game_chose_buttons.size() ? 0 : (select_button + 1);
                    game_chose_buttons[select_button].set_selected(true);
                }
                break;
            case 0x57: // VK_W
            case 0x26: // VK_UP
                if (!is_chose_game) {
                    buttons[select_button].set_selected(false);
                    select_button = (select_button - 1) == -1 ? (buttons.size() - 1) : select_button - 1;
                    buttons[select_button].set_selected(true);
                }
                else {
                    game_chose_buttons[select_button].set_selected(false);
                    select_button = (select_button - 1) == -1 ? (game_chose_buttons.size() - 1) : select_button - 1;
                    game_chose_buttons[select_button].set_selected(true);
                }
                break;
            case 0x20: // VK_SPACE
            case 0x0D: // VK_RETURN
                if (!is_chose_game) {
                    buttons[select_button].start_clicked();
                }
                else {
                    game_chose_buttons[select_button].start_clicked();
                }
                break;
            }
            break;
        case WM_KEYUP:
            switch (msg.vkcode)
            {
            case 0x1B: // VK_ESCAPE
                if(!is_chose_game){
                    SceneManager::instance()->switch_scene_to(SceneManager::SceneType::Start);
                }else {
                    game_chose_buttons[select_button].set_selected(false);
                    on_exit();
                    on_enter();
                }
                break;
            case 0x20: // VK_SPACE
            case 0x0D: // VK_RETURN
                if (!is_chose_game) {
                    buttons[select_button].end_clicked();
                }else {
                    game_chose_buttons[select_button].end_clicked();
                }
                break;
            }
            break;
        }


    }

};