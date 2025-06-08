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

        // �������ð�ť��ÿ�ε������10������100������Ϊ0��
        buttons.emplace_back(50, 100, 150, 80, _T("����:"), [&]() {
            Settings::instance()->set_setting_volume(
                (Settings::instance()->get_setting_volume() + 10) > 100 ? 0 : (Settings::instance()->get_setting_volume() + 10));
            set_volume(_T("start_menu_bgm"), Settings::instance()->get_setting_volume());
            });

        // ֡�����ް�ť����60��144֮���л���
        buttons.emplace_back(50, 200, 150, 80, _T("֡������"), [&]() {
            Settings::instance()->set_fps((Settings::instance()->get_fps() == 60) ? 144 : 60);
            });

        // Debugģʽ��ť���л�Debugģʽ��
        buttons.emplace_back(50, 300, 150, 80, _T("Debugģʽ"), [&]() {
            Settings::instance()->set_is_debug_on(!Settings::instance()->get_is_debug_on());
            });
       
        // ���ز˵���ť
        buttons.emplace_back(50, 400, 150, 80, _T("���沢����"), [&]() {
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

        // ����������֡���ı�ǩ����ֵ
        settextstyle(24, 0, _T("IPix")); // ���������С������

        TCHAR volume_text[32];
        _stprintf_s(volume_text, _T("%d"), Settings::instance()->get_setting_volume()); // ��volumeת��Ϊ�ַ���
        outtextxy(200, 125, _T("����:")); // ������ǩλ��
        outtextxy(270, 125, volume_text); // ������ֵλ��

        TCHAR fps_text[32];
        _stprintf_s(fps_text, _T("%d"), Settings::instance()->get_fps()); // ��fpsת��Ϊ�ַ���
        outtextxy(200, 225, _T("֡������:")); // ֡����ǩλ��
        outtextxy(320, 225, fps_text); // ֡����ֵλ��
        outtextxy(400, 225, _T("(��Ҫ������Ϸ��Ч)")); // ֡����ǩλ��

        if(Settings::instance()->get_is_debug_on())
            outtextxy(200, 325, _T("����")); // ֡����ǩλ��
        else
            outtextxy(200, 325, _T("�ر�")); // ֡����ǩλ��

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