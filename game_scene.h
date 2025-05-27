#pragma once

#include "scene.h"
#include "player.h"
#include "button.h"
#include "rock.h"
#include "mushroom.h"

class GameScene : public Scene {
private:
    static const int Max_Mushroom_Num = 3; // 最大蘑菇数量
    const float Mushroom_Generation_Interval = 5.0f; // 蘑菇生成间隔时间（秒）
    const int width_per_grid = 1080 / 30; // 每个网格的宽度
    const int height_per_grid = 720 / 20; // 每个网格的高度

private:
    CollisionBox* collision_box_edges[4]; // 碰撞箱数组，用于边界检测
    Player* p1 = nullptr;
    Timer mushroom_generation_timer; // 蘑菇生成计时器


    std::vector<Button> buttons_paused; // 暂停菜单按钮列表
    std::vector<Button> buttons_game_over; // 游戏结束菜单按钮列表

    Mushroom* mushrooms[Max_Mushroom_Num] = { nullptr }; // 蘑菇指针

    bool is_paused = false;
    bool is_game_over = false;
    int select_button_paused = 0;
    int select_button_game_over = 0;
    
    int mushroom_num = 0; // 蘑菇数量
    bool can_place_at[30][20];
    
    Rock rock{ 200, 200, 1, 1.5};
    
private:
    void init_place_map() {
        for(int i = 0 ; i < 30; i++) {
            for (int j = 0; j < 20; j++) {
                can_place_at[i][j] = true; // 初始化所有位置都可以放置
            }
        }
    }

    void generate_mushroom() {

        for (int i = 0; i < Max_Mushroom_Num; i++) {
            if (mushrooms[i]->is_need_growth()) {
                int grid_x, grid_y;

                do {

                    grid_x = rand() % 30; // 随机生成x坐标
                    grid_y = rand() % 20; // 随机生成y坐标

                } while (!can_place_at[grid_x][grid_y]); // 确保生成位置可以放置蘑菇
                can_place_at[grid_x][grid_y] = false; // 标记该位置已被占用

                // 放置蘑菇
                mushrooms[i]->set_position(
                    Vector2(grid_x * width_per_grid + rand() % width_per_grid,
                        grid_y * height_per_grid + rand() % height_per_grid));
                mushrooms[i]->growth();


                break; // 只生成一个蘑菇
            }
        }

    }

public:

    GameScene() {
        {
            int button_w = ResourcesManager::instance()->find_image("button")->getwidth();
            int button_h = ResourcesManager::instance()->find_image("button")->getheight();
            int button_x = (getwidth() - button_w) / 2 - 50;

            buttons_paused.clear();
            buttons_paused.reserve(3);
            buttons_paused.emplace_back(button_x, 250, 150, 80, _T("继续"), [this]() {
                is_paused = false;
                });
            buttons_paused.emplace_back(button_x, 350, 150, 80, _T("设置"));
            buttons_paused.emplace_back(button_x, 450, 150, 80, _T("返回主菜单"), [this]() {
                SceneManager::instance()->switch_scene_to(SceneManager::SceneType::Menu);
                });

            buttons_game_over.clear();
            buttons_game_over.reserve(2);
            buttons_game_over.emplace_back(button_x - 150, 450, 150, 80, _T("重新开始"), [this]() {
                SceneManager::instance()->switch_scene_to(SceneManager::SceneType::Game);
                });
            buttons_game_over.emplace_back(button_x + 150, 450, 150, 80, _T("返回主菜单"), [this]() {
                SceneManager::instance()->switch_scene_to(SceneManager::SceneType::Menu);
                });
        }
        {
            for(int i = 0; i < 4; i++) {
                collision_box_edges[i] = CollisionManager::instance()->create_collision_box();
                collision_box_edges[i]->set_layer_src(CollisionLayer::None);
                collision_box_edges[i]->set_layer_dst(CollisionLayer::Player);
            }
            collision_box_edges[0]->set_position(Vector2( - 1, getheight() / 2)); // 左边界
            collision_box_edges[0]->set_size(Vector2(2, getheight()));
            collision_box_edges[1]->set_position(Vector2(getwidth() + 1, getheight() / 2)); // 右边界
            collision_box_edges[1]->set_size(Vector2(2, getheight()));
            collision_box_edges[2]->set_position(Vector2(getwidth() / 2, -1)); // 上边界
            collision_box_edges[2]->set_size(Vector2(getwidth(), 2));
            collision_box_edges[3]->set_position(Vector2(getwidth() / 2, getheight() + 1)); // 下边界
            collision_box_edges[3]->set_size(Vector2(getwidth(), 2));
        }

        {
            mushroom_generation_timer.set_one_shot(false); // 设置为非一次性计时器
            mushroom_generation_timer.set_wait_time(Mushroom_Generation_Interval); // 设置蘑菇生成间隔时间
            mushroom_generation_timer.set_on_timeout([this]() {
                generate_mushroom(); // 调用生成蘑菇函数
                });
            mushroom_generation_timer.pause(); // 初始状态下暂停计时器

            for (int i = 0; i < Max_Mushroom_Num; i++) {
                mushrooms[i] = new Mushroom(0, 0); // 初始化蘑菇指针
            }
        }
    }

    ~GameScene() {
        for(int i = 0; i < 4; i++) {
            if (collision_box_edges[i]) {
                CollisionManager::instance()->destroy_collision_box(collision_box_edges[i]);
                collision_box_edges[i] = nullptr;
            }
        }
        if(p1) {
            delete p1;
            p1 = nullptr;
        }
    }



    void on_enter() {
        init_place_map(); // 初始化放置地图

        buttons_paused[select_button_paused].set_selected(true);
        buttons_game_over[select_button_game_over].set_selected(true);
        
        if (p1) {
            delete p1;
            p1 = nullptr;
        }
        p1 = new Player();

        mushroom_generation_timer.restart();
        mushroom_generation_timer.resume(); // 启动蘑菇生成计时器
    }

    void on_exit() {
        mushroom_generation_timer.pause(); // 暂停蘑菇生成计时器

        // 如果玩家对象存在，则删除它
        if (p1) { 
            delete p1;
            p1 = nullptr;
        }

        buttons_paused[select_button_paused].set_selected(false); // 取消选中当前按钮
        buttons_game_over[select_button_game_over].set_selected(false); // 取消选中当前按钮
        select_button_paused = 0; // 重置选中按钮索引
        select_button_game_over = 0; // 重置选中按钮索引

        is_paused = false; // 重置暂停状态
        is_game_over = false; // 重置游戏结束状态
    }

    void on_update(float delta) {
        if (is_paused || is_game_over) return; // 如果游戏暂停或结束，则不更新游戏逻辑

        if (!p1->get_alive()) is_game_over = true; // 如果玩家死亡，则设置游戏结束状态

        p1->on_update(delta);
        mushroom_generation_timer.on_update(delta); // 更新蘑菇生成计时器

        for(Mushroom* mushroom : mushrooms) { 
            if (!mushroom->is_need_growth()) { // 只更新已经生长的蘑菇
                mushroom->on_update(delta);
            }
        }

        CollisionManager::instance()->process_collide();
    }


    void on_render(const Camera& camera) {
        Rect rect_dst(0, 0, 1080, 720);
        putimage_alpha(ResourcesManager::instance()->find_image("background"), &rect_dst);

        for (Mushroom* mushroom : mushrooms) {
            if (!mushroom->is_need_growth()) { // 只渲染已经生长的蘑菇
                mushroom->on_render();
            }
        }

        rock.on_render();

        p1->on_render();


        if(is_game_over) {
            for(Button& button : buttons_game_over) {
                button.on_render();
            }
        }

        if (is_paused && !is_game_over) {
            for(Button& button : buttons_paused) {
                button.on_render();
            }
        }
    }


    void on_input(const ExMessage& msg) {
        if(!is_paused) {
            p1->on_input(msg);
        }

        switch (msg.message)
        {
        case WM_KEYDOWN:
            if (is_paused && !is_game_over) { // 如果游戏暂停且不是游戏结束状态
                switch (msg.vkcode)
                {
                case 0x53: // VK_S
                case 0x28: // VK_DOWN
                    buttons_paused[select_button_paused].set_selected(false);
                    select_button_paused = (select_button_paused + 1) == buttons_paused.size() ? 0 : (select_button_paused + 1);
                    buttons_paused[select_button_paused].set_selected(true);
                    break;
                case 0x57: // VK_W
                case 0x26: // VK_UP
                    buttons_paused[select_button_paused].set_selected(false);
                    select_button_paused = (select_button_paused - 1) == -1 ? (buttons_paused.size() - 1) : select_button_paused - 1;
                    buttons_paused[select_button_paused].set_selected(true);
                    break;
                case 0x20: // VK_SPACE
                case 0x0D: // VK_RETURN
                    buttons_paused[select_button_paused].start_clicked();
                    break;
                }
            }
            if (is_game_over && !is_paused) { // 如果游戏结束且不是暂停状态
                switch (msg.vkcode)
                {
                case 0x41: // VK_A
                case 0x27: // VK_RIGHT
                    buttons_game_over[select_button_game_over].set_selected(false);
                    select_button_game_over = (select_button_game_over + 1) == buttons_game_over.size() ? 0 : (select_button_game_over + 1);
                    buttons_game_over[select_button_game_over].set_selected(true);
                    break;
                case 0x44: // VK_D
                case 0x25: // VK_LEFT
                    buttons_game_over[select_button_game_over].set_selected(false);
                    select_button_game_over = (select_button_game_over - 1) == -1 ? (buttons_game_over.size() - 1) : select_button_game_over - 1;
                    buttons_game_over[select_button_game_over].set_selected(true);
                    break;
                case 0x20: // VK_SPACE
                case 0x0D: // VK_RETURN
                    buttons_game_over[select_button_game_over].start_clicked();
                    break;
                }

            }
            break;
        case WM_KEYUP:
            if(!is_game_over) {
                switch (msg.vkcode)
                {
                case 0x1B: // VK_ESCAPE
                    if (!is_game_over)
                        is_paused = !is_paused;
                    break;
                case 0x20: // VK_SPACE
                case 0x0D: // VK_RETURN
                    buttons_paused[select_button_paused].end_clicked();
                    break;
                }
            }

            if( is_game_over && !is_paused) {
                switch (msg.vkcode)
                {
                case 0x20: // VK_SPACE
                case 0x0D: // VK_RETURN
                    buttons_game_over[select_button_game_over].end_clicked();
                    break;
                }
            }
            break;

        }
    }
};