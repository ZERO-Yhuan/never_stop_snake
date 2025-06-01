#pragma once

#include "../scene.h"
#include "../../manager/scene_manager.h"

#include "../../entity/player/custom/normal_snake.h"

#include "../../entity/custom/rock.h"
#include "../../entity/custom/mushroom.h"
#include "../../entity/custom/big_grass.h"
#include "../../entity/custom/little_grass.h"
#include "../../entity/custom/red_flower.h"
#include "../../entity/custom/white_flower.h"
#include "../../entity/custom/yellow_flower.h"

#include "../../entity/custom/aerolite.h"




class GameScene : public Scene {
private:
    static const int Max_Mushroom_Num = 3; // ���Ģ������
    static const int Max_Aerolite_Num = 3; // �����������
    const float Mushroom_Generation_Interval = 5.0f; // Ģ�����ɼ��ʱ�䣨�룩
    const float Aerolite_Generation_Interval = 20.0f; // Ģ�����ɼ��ʱ�䣨�룩
    const int width_per_grid = 1080 / 30; // ÿ������Ŀ��
    const int height_per_grid = 720 / 20; // ÿ������ĸ߶�

    const float Limit_Time = 180.0f; // ��Ϸʱ�����ƣ��룩

private:
    CollisionBox* collision_box_edges[4]; // ��ײ�����飬���ڱ߽���
    Player* p1 = nullptr;
    Timer mushroom_generation_timer; // Ģ�����ɼ�ʱ��
    Timer aerolite_generation_timer;

    Timer time_limit_timer; // ʱ�����Ƽ�ʱ��

    Rect rect_score = { 0,0,160, 80 };
    Rect rect_time = { 920,0,160, 80 };
    Rect rect_score_game_over = { 380, 0, 320, 160 };

    std::vector<Button> buttons_paused; // ��ͣ�˵���ť�б�
    std::vector<Button> buttons_game_over; // ��Ϸ�����˵���ť�б�

    std::vector<Entity*> entities;

    Mushroom* mushrooms[Max_Mushroom_Num] = { nullptr }; // Ģ��ָ��
    Aerolite* aerolites[Max_Aerolite_Num] = { nullptr }; // ����ָ��

    bool is_paused = false;
    bool is_game_over = false;
    int select_button_paused = 0;
    int select_button_game_over = 0;

    int current_remainder_time = Limit_Time; // ��ǰʣ��ʱ�䣨�룩

    bool can_place_at[30][20];

    bool game_state = 0;

private:
    Vector2 get_place_pos();
    void init_entities();
    void destroy_entities();

    void init_place_map();

    void render_score_text();
    void render_time_text();

    void generate_mushroom();
    void generate_aerolite(int num);

public:

    GameScene();
    ~GameScene();

    void on_enter() override;
    void on_exit() override;
    void on_update(const float delta) override;
    void on_render(const Camera& camera) override;
    void on_input(const ExMessage& msg) override;

    void set_scene_state(const int game_state) override;
};