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
    static const int Max_Mushroom_Num = 3; // 最大蘑菇数量
    static const int Max_Aerolite_Num = 3; // 最大流星数量
    const float Mushroom_Generation_Interval = 5.0f; // 蘑菇生成间隔时间（秒）
    const float Aerolite_Generation_Interval = 20.0f; // 蘑菇生成间隔时间（秒）
    const int width_per_grid = 1080 / 30; // 每个网格的宽度
    const int height_per_grid = 720 / 20; // 每个网格的高度

    const float Limit_Time = 180.0f; // 游戏时间限制（秒）

private:
    CollisionBox* collision_box_edges[4]; // 碰撞箱数组，用于边界检测
    Player* p1 = nullptr;
    Player* p2 = nullptr;
    Timer mushroom_generation_timer; // 蘑菇生成计时器
    Timer aerolite_generation_timer;

    Timer time_limit_timer; // 时间限制计时器

    Rect rect_P1score = { 0,0,160, 80 };
    Rect rect_P2score = { 920,0,160, 80 };

    Rect rect_time = { 450,0,160, 80 };
    Rect rect_score_game_over = { 380, 0, 320, 160 };

    std::vector<Button> buttons_paused; // 暂停菜单按钮列表
    std::vector<Button> buttons_game_over; // 游戏结束菜单按钮列表

    std::vector<Entity*> entities;

    Mushroom* mushrooms[Max_Mushroom_Num] = { nullptr }; // 蘑菇指针
    Aerolite* aerolites[Max_Aerolite_Num] = { nullptr }; // 流星指针 

    bool is_paused = false;
    bool is_game_over = false;

    bool is_P1game_over = false;
    bool is_P2game_over = false;

    bool is_game_draw = false;

    int current_remainder_time = Limit_Time; // 当前剩余时间（秒）

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
    void render_time_text();

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