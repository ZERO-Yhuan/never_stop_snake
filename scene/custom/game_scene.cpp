#include "game_scene.h"

void GameScene::init_place_map() {
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 20; j++) {
            can_place_at[i][j] = true; // 初始化所有位置都可以放置
        }
    }
}

void GameScene::generate_mushroom() {

    for (int i = 0; i < Max_Mushroom_Num; i++) {
        if (mushrooms[i]->is_need_growth()) {
            // 释放生成图位置
            can_place_at[static_cast<int>(mushrooms[i]->get_position().x / width_per_grid)][static_cast<int>(mushrooms[i]->get_position().x / height_per_grid)] = true;
            Vector2 place_pos = get_place_pos();
            // 放置蘑菇
            mushrooms[i]->set_position(place_pos);
            mushrooms[i]->growth();

            break; // 只生成一个蘑菇
        }
    }

}

void GameScene::render_score_text() {
    int score = p1->get_score();
    LPCTSTR score_str = _T("%d");
    TCHAR score_text[32];
    _stprintf_s(score_text, score_str, score);
    if (!is_game_over) {
        putimage_alpha(ResourcesManager::instance()->find_image("score_background"), &rect_score);
        outtextxy_shaded(rect_score.x + 20, rect_score.y + 15, _T("当前得分:"));
        outtextxy_shaded(rect_score.x + 20, rect_score.y + 45, score_text);
    }
    else {
        settextstyle(52, 0, _T("IPix"));
        putimage_alpha(ResourcesManager::instance()->find_image("score_background"), &rect_score_game_over);
        outtextxy_shaded(rect_score_game_over.x + 40, rect_score_game_over.y + 30, _T("当前得分:"));
        outtextxy_shaded(rect_score_game_over.x + 40, rect_score_game_over.y + 90, score_text);
        settextstyle(26, 0, _T("IPix"));
    }

}

void GameScene::render_time_text() {
    LPCTSTR time_str = _T("%d");
    TCHAR time_text[32];
    _stprintf_s(time_text, time_str, current_remainder_time);
    if (!is_game_over) {
        putimage_alpha(ResourcesManager::instance()->find_image("score_background"), &rect_time);
        outtextxy_shaded(rect_time.x + 20, rect_time.y + 15, _T("当前时间:"));
        outtextxy_shaded(rect_time.x + 20, rect_time.y + 45, time_text);
    }
}

void GameScene::generate_aerolite(int num) {
    for (int i = 0; i < num && i < Max_Aerolite_Num; i++) {
        int grid_x, grid_y;

        grid_x = rand() % 30; // 随机生成x坐标
        grid_y = rand() % 20; // 随机生成y坐标

        float place_x = grid_x * width_per_grid + rand() % width_per_grid;
        float place_y = grid_y * height_per_grid + rand() % height_per_grid;

        if (aerolites[i])
            aerolites[i]->start_fall(place_x, place_y); // 放置流星

            
    }
}


GameScene::GameScene() {
    { // 按钮初始化
        int button_w = ResourcesManager::instance()->find_image("button")->getwidth();
        int button_h = ResourcesManager::instance()->find_image("button")->getheight();
        int button_x = (getwidth() - button_w) / 2 - 50;

        buttons_paused.clear();
        buttons_paused.reserve(3);
        buttons_paused.emplace_back(button_x, 250, 150, 80, _T("继续"), [this]() {
            is_paused = false;
            });
        buttons_paused.emplace_back(button_x, 350, 150, 80, _T("返回主菜单"), [this]() {
            SceneManager::instance()->switch_scene_to(SceneManager::SceneType::Menu);
            });
        
        buttons_game_over.clear();
        buttons_game_over.reserve(2);
        buttons_game_over.emplace_back(button_x - 150, 300, 150, 80, _T("重新开始"), [this]() {
            on_exit();
            on_enter();
            });
        buttons_game_over.emplace_back(button_x + 150, 300, 150, 80, _T("返回主菜单"), [this]() {
            SceneManager::instance()->switch_scene_to(SceneManager::SceneType::Menu);
            });
    } 

    { // 初始化边界碰撞盒
        for (int i = 0; i < 4; i++) {
            collision_box_edges[i] = CollisionManager::instance()->create_collision_box();
            collision_box_edges[i]->set_layer_src(CollisionLayer::None);
            collision_box_edges[i]->set_layer_dst(CollisionLayer::Player);
        }
        collision_box_edges[0]->set_position(Vector2(-1, getheight() / 2)); // 左边界
        collision_box_edges[0]->set_size(Vector2(2, getheight()));
        collision_box_edges[1]->set_position(Vector2(getwidth() + 1, getheight() / 2)); // 右边界
        collision_box_edges[1]->set_size(Vector2(2, getheight()));
        collision_box_edges[2]->set_position(Vector2(getwidth() / 2, -1)); // 上边界
        collision_box_edges[2]->set_size(Vector2(getwidth(), 2));
        collision_box_edges[3]->set_position(Vector2(getwidth() / 2, getheight() + 1)); // 下边界
        collision_box_edges[3]->set_size(Vector2(getwidth(), 2));
    } 
    
    { // 初始化蘑菇和流星生成器
        for (int i = 0; i < Max_Mushroom_Num; i++) {
            mushrooms[i] = new Mushroom(1080 / 2, 720 / 2); // 初始化蘑菇指针
        }

        for (int i = 0; i < Max_Aerolite_Num; i++) {
            aerolites[i] = new Aerolite(0, 0); // 初始化流星指针
        }

        mushroom_generation_timer.set_one_shot(false); // 设置为非一次性计时器
        mushroom_generation_timer.set_wait_time(Mushroom_Generation_Interval); // 设置蘑菇生成间隔时间
        mushroom_generation_timer.set_on_timeout([this]() {
            generate_mushroom(); // 调用生成蘑菇函数
            });
        mushroom_generation_timer.pause(); // 初始状态下暂停计时器



        aerolite_generation_timer.set_one_shot(false);
        aerolite_generation_timer.set_wait_time(Aerolite_Generation_Interval);
        aerolite_generation_timer.set_on_timeout([this]() {
            int is_generated = rand() % 2;

            if (p1->get_score() >= 400) {
                generate_aerolite(3);
            }else if (p1->get_score() >= 300) {
                if (is_generated)
                    generate_aerolite(3); // 调用生成流星函数
            }
            else if (p1->get_score() >= 200) {
                if (is_generated)
                    generate_aerolite(2); // 调用生成流星函数
            }
            else if (p1->get_score() >= 100) {
                if (is_generated)
                    generate_aerolite(1); // 调用生成流星函数
            }
            });
        aerolite_generation_timer.pause(); // 初始状态下暂停计时器

 
    } 

    {   // 初始化时间限制计时器
        time_limit_timer.set_one_shot(false);
        time_limit_timer.set_wait_time(1);
        time_limit_timer.set_on_timeout([this]() {
            current_remainder_time--;
            if (current_remainder_time == 0) {
                is_game_over = true; // 如果时间限制结束，则设置游戏结束状态
                play_audio(_T("game_over"), false);
            }
        });
        time_limit_timer.pause(); // 初始状态下暂停计时器
    } 
}

GameScene::~GameScene() {
    for (int i = 0; i < 4; i++) {
        if (collision_box_edges[i]) {
            CollisionManager::instance()->destroy_collision_box(collision_box_edges[i]);
            collision_box_edges[i] = nullptr;
        }
    }
    for (Mushroom* mushroom : mushrooms) {
        if (mushroom) {
            delete mushroom;
            mushroom = nullptr;
        }
    }

    for (Aerolite* aerolite : aerolites) {
        if (aerolite) {
            delete aerolite;
            aerolite = nullptr;
        }
    }

    if (p1) {
        delete p1;
        p1 = nullptr;
    }
    
}


void GameScene::on_enter() {
    init_place_map(); // 初始化放置地图
    can_place_at[1080 / 2 / width_per_grid][720 / 2 / height_per_grid] = false; // 玩家生成位置不可生成建筑物

    init_entities(); // 初始化实体
    

    buttons_paused[select_button_paused].set_selected(true); 
    buttons_game_over[select_button_game_over].set_selected(true);

    if (p1) {
        delete p1;
        p1 = nullptr;
    }
    switch (SnakeTypeHandle::instance()->current_snake_type)
    {
    case 0:
        p1 = new NormalSnake(); // 初始化玩家对象
        break;
    case 1:
        p1 = new SpeedSnake();
        break;
    case 2:
        // ......
        break;
    default:
        p1 = new NormalSnake(); // 初始化玩家对象
        break;
    }
   

    mushroom_generation_timer.restart(); // 重置蘑菇生成计时器
    mushroom_generation_timer.resume(); // 启动蘑菇生成计时器

    aerolite_generation_timer.restart(); // 重置流星生成计时器
    aerolite_generation_timer.resume(); // 启动流星生成计时器

    if (game_state == 1) {
        time_limit_timer.restart(); // 重置时间限制计时器
        time_limit_timer.resume(); // 启动时间限制计时器
    }
}

void GameScene::on_exit() {
    mushroom_generation_timer.pause(); // 暂停蘑菇生成计时器
    aerolite_generation_timer.pause();
    time_limit_timer.pause();

    destroy_entities();

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
    current_remainder_time = Limit_Time; // 重置时间限制
}

void GameScene::on_update(float delta) {
    

    if (is_paused || is_game_over) return; // 如果游戏暂停或结束，则不更新游戏逻辑

    if (!p1->get_alive()) {
        is_game_over = true; // 如果玩家死亡，则设置游戏结束状态
        play_audio(_T("game_over"), false);
    }

    p1->on_update(delta);

    for (Entity* entity : entities) {
        entity->on_update(delta);
    }

    if (game_state == 1)
        time_limit_timer.on_update(delta);

    mushroom_generation_timer.on_update(delta); // 更新蘑菇生成计时器
    aerolite_generation_timer.on_update(delta); // 更新流星生成计时器

    for (Mushroom* mushroom : mushrooms) {
        if (!mushroom->is_need_growth()) { // 只更新已经生长的蘑菇
            mushroom->on_update(delta);
        }
    }

    for (Aerolite* aerolite : aerolites) {
        if(aerolite)
            aerolite->on_update(delta);
    }

    CollisionManager::instance()->process_collide();
}


void GameScene::on_render(const Camera& camera) {
    Rect rect_dst(0, 0, 1080, 720);
    putimage_alpha(ResourcesManager::instance()->find_image("background"), &rect_dst);

    for (Mushroom* mushroom : mushrooms) {
        if (!mushroom->is_need_growth()) { // 只渲染已经生长的蘑菇
            mushroom->on_render(camera);
        }
    }

    for (Entity* entity : entities) {
        entity->on_render(camera);
    }
    
    p1->on_render(camera);
    
    for (Aerolite* aerolite : aerolites) {
        if(aerolite)
            aerolite->on_render(camera);
    }

    render_score_text();

    if(game_state == 1 && !is_game_over)
        render_time_text();

    if (is_game_over) {
        for (Button& button : buttons_game_over) {
            button.on_render();
        }
    }

    if (is_paused && !is_game_over) {
        for (Button& button : buttons_paused) {
            button.on_render();
        }
    }
}


void GameScene::on_input(const ExMessage& msg) {
    if (!is_paused) {
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
        if (!is_game_over) {
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

        if (is_game_over && !is_paused) {
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


Vector2 GameScene::get_place_pos() {
    int grid_x, grid_y;

    do {
        grid_x = rand() % 30; // 随机生成x坐标
        grid_y = rand() % 20; // 随机生成y坐标

    } while (!can_place_at[grid_x][grid_y]); // 确保生成位置可以放置蘑菇
    can_place_at[grid_x][grid_y] = false; // 标记该位置已被占用

    float place_x = grid_x * width_per_grid + rand() % width_per_grid;
    float place_y = grid_y * height_per_grid + rand() % height_per_grid;

    return Vector2(place_x, place_y);
}

void GameScene::init_entities() {
    
    for (int i = 0; i < 3; i++) {
        Vector2 place_pos = get_place_pos();
        Rock* rock = new Rock(place_pos.x, place_pos.y, rand() % 2, 1.5);
        entities.push_back(rock);
    }
    
    for (int i = 0; i < 7; i++) {
        Vector2 place_pos = get_place_pos();
        BigGrass* big_grass = new BigGrass(place_pos.x, place_pos.y);
        entities.push_back(big_grass);
    }

    for (int i = 0; i < 9; i++) {
        Vector2 place_pos = get_place_pos();
        LittleGrass* little_grass = new LittleGrass(place_pos.x, place_pos.y);
        entities.push_back(little_grass);
    }

    for (int i = 0; i < 4; i++) {
        Vector2 place_pos = get_place_pos();
        WhiteFlower* white_flower = new WhiteFlower(place_pos.x, place_pos.y);
        entities.push_back(white_flower);
    }

    for (int i = 0; i < 4; i++) {
        Vector2 place_pos = get_place_pos();
        RedFlower* red_grass = new RedFlower(place_pos.x, place_pos.y);
        entities.push_back(red_grass);
    }

    for (int i = 0; i < 3; i++) {
        Vector2 place_pos = get_place_pos();
        YellowFlower* yellow_grass = new YellowFlower(place_pos.x, place_pos.y);
        entities.push_back(yellow_grass);
    }
}

void GameScene::destroy_entities() {
    for (Entity* entity : entities) {
        delete entity;
        entity = nullptr;
    }

    entities.clear();
}

void GameScene::set_scene_state(const int game_state) {
    if(game_state < 0 || game_state > 2) return; // 无效的游戏状态
    this->game_state = game_state;
}

