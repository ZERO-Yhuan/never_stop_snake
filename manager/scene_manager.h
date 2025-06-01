#pragma once

#include "../scene/scene.h"

extern Scene* start_scene;
extern Scene* menu_scene;
extern Scene* game_scene;

class SceneManager {
private:
    Scene* current_scene = nullptr; // ��ǰ����

    static SceneManager* _instance; // ����ʵ��

public:
    // ����ö��
    enum SceneType 
    { 
        Start,
        Menu,
        Game
    };


private:
    SceneManager();
    ~SceneManager();

public:
    static SceneManager* instance();

    void set_current_scene(Scene* scene); // ���õ�ǰ����

    void switch_scene_to(SceneType type, int state = 0); // �л�����

    void on_update(float delta); // ���µ�ǰ����

    void on_render(const Camera& camera); // ��Ⱦ��ǰ����

    void on_input(const ExMessage& msg); // ��������

};