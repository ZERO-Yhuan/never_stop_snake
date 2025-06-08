#pragma once

#include "../scene.h"
#include "../../manager/scene_manager.h"

#include "../../entity/player/custom/normal_snake.h"
#include "../../entity/player/custom/normal_snake_p2.h"

#include "../../ui/custom/button.h"

#include "../../entity/custom/rock.h"
#include "../../entity/custom/mushroom.h"
#include "../../entity/custom/big_grass.h"
#include "../../entity/custom/little_grass.h"
#include "../../entity/custom/red_flower.h"
#include "../../entity/custom/white_flower.h"
#include "../../entity/custom/yellow_flower.h"

#include "../../entity/custom/aerolite.h"




class DoublePlayerGameScene : public Scene {
private:
    static const int Max_Mushroom_Num = 3; // ���Ģ������
    static const int Max_Aerolite_Num = 3; // �����������
    const float Mushroom_Generation_Interval = 5.0f; // Ģ�����ɼ��ʱ�䣨�룩
    const float Aerolite_Generation_Interval = 20.0f; // Ģ�����ɼ��ʱ�䣨�룩
    const int width_per_grid = 1080 / 30; // ÿ������Ŀ��
    const int height_per_grid = 720 / 20; // ÿ������ĸ߶�

private:
    CollisionBox* collision_box_edges[4]; // ��ײ�����飬���ڱ߽���
    Player* p1 = nullptr;
    Player* p2 = nullptr;
    Timer mushroom_generation_timer; // Ģ�����ɼ�ʱ��

    Timer aerolite_generation_timer;

    Rect rect_P1score = { 0,0,160, 80 };
    Rect rect_P2score = { 920,0,160, 80 };

    Rect rect_score_game_over = { 380, 0, 320, 160 };

    std::vector<Button> buttons_paused; // ��ͣ�˵���ť�б�
    std::vector<Button> buttons_game_over; // ��Ϸ�����˵���ť�б�

    std::vector<Entity*> entities;

    Mushroom* mushrooms[Max_Mushroom_Num] = { nullptr }; // Ģ��ָ��
    Aerolite* aerolites[Max_Aerolite_Num] = { nullptr }; // ����ָ�� 

    bool is_paused = false;
    bool is_game_over = false;

    bool is_P1game_over = false;
    bool is_P2game_over = false;

    bool is_game_draw = false;

    int select_button_paused = 0;
    int select_button_game_over = 0;

    bool can_place_at[30][20];

private:
    Vector2 get_place_pos();
    void init_entities();
    void destroy_entities();

    void init_place_map();
    void generate_mushroom();
    void render_score_text();

    void generate_aerolite(int num);

public:

    DoublePlayerGameScene();
    ~DoublePlayerGameScene();

    void on_enter();
    void on_exit();
    void on_update(float delta);
    void on_render(const Camera& camera);
    void on_input(const ExMessage& msg);
};