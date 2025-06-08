#pragma once

#include "../scene.h"
#include "../../manager/scene_manager.h"
#include "../../manager/resources_manager.h"
#include "../../ui/custom/button.h"
#include "../../tool/util.h"
#include "../../tool/settings.h"
#include <fstream>
#include <string>

class SettingsScene : public Scene {
private:
    std::vector<Button> buttons;
    int select_button = 0;


public:
    SettingsScene() {
        buttons.clear();
        buttons.reserve(4);

        // 音量设置按钮（每次点击增加10，超过100则重置为0）
        buttons.emplace_back(50, 100, 150, 80, _T("音量:"), [&]() {
            Settings::instance()->set_setting_volume(
                (Settings::instance()->get_setting_volume() + 10) > 100 ? 0 : (Settings::instance()->get_setting_volume() + 10));
            set_volume(_T("start_menu_bgm"), Settings::instance()->get_setting_volume());
            });

        // 帧数上限按钮（在60和144之间切换）
        buttons.emplace_back(50, 200, 150, 80, _T("帧数上限"), [&]() {
            Settings::instance()->set_fps((Settings::instance()->get_fps() == 60) ? 144 : 60);
            });

        // Debug模式按钮（切换Debug模式）
        buttons.emplace_back(50, 300, 150, 80, _T("Debug模式"), [&]() {
            Settings::instance()->set_is_debug_on(!Settings::instance()->get_is_debug_on());
            });
       
        // 返回菜单按钮
        buttons.emplace_back(50, 400, 150, 80, _T("保存并返回"), [&]() {
            Settings::instance()->save();
            SceneManager::instance()->switch_scene_to(SceneManager::SceneType::Menu);
            });
       
           
    }

    void on_enter() {
        buttons[select_button].set_selected(true);
    }

    void on_exit() {
        buttons[select_button].set_selected(false);
        select_button = 0;
    }

    void on_update(float delta) {}

    void on_render(const Camera& camera) {
        Rect rect_dst(0, 0, 1080, 720);
        putimage_alpha(ResourcesManager::instance()->find_image("background"), &rect_dst);

        // 绘制音量和帧数的标签与数值
        settextstyle(24, 0, _T("IPix")); // 设置字体大小和类型

        TCHAR volume_text[32];
        _stprintf_s(volume_text, _T("%d"), Settings::instance()->get_setting_volume()); // 将volume转换为字符串
        outtextxy(200, 125, _T("音量:")); // 音量标签位置
        outtextxy(270, 125, volume_text); // 音量数值位置

        TCHAR fps_text[32];
        _stprintf_s(fps_text, _T("%d"), Settings::instance()->get_fps()); // 将fps转换为字符串
        outtextxy(200, 225, _T("帧数上限:")); // 帧数标签位置
        outtextxy(320, 225, fps_text); // 帧数数值位置
        outtextxy(400, 225, _T("(需要重启游戏生效)")); // 帧数标签位置

        if(Settings::instance()->get_is_debug_on())
            outtextxy(200, 325, _T("开启")); // 帧数标签位置
        else
            outtextxy(200, 325, _T("关闭")); // 帧数标签位置

        for (Button& button : buttons)
            button.on_render();
    }

    void on_input(const ExMessage& msg) {
        switch (msg.message) {
        case WM_KEYDOWN:
            switch (msg.vkcode) {
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
            switch (msg.vkcode) {
            case 0x1B: // VK_ESCAPE
                SceneManager::instance()->switch_scene_to(SceneManager::SceneType::Menu);
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