#include "game_scene.h"

void GameScene::init_place_map() {
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 20; j++) {
            can_place_at[i][j] = true; // ��ʼ������λ�ö����Է���
        }
    }
}

void GameScene::generate_mushroom() {

    for (int i = 0; i < Max_Mushroom_Num; i++) {
        if (mushrooms[i]->is_need_growth()) {
            // �ͷ�����ͼλ��
            can_place_at[static_cast<int>(mushrooms[i]->get_position().x / width_per_grid)][static_cast<int>(mushrooms[i]->get_position().x / height_per_grid)] = true;
            Vector2 place_pos = get_place_pos();
            // ����Ģ��
            mushrooms[i]->set_position(place_pos);
            mushrooms[i]->growth();

            break; // ֻ����һ��Ģ��
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
        outtextxy_shaded(rect_score.x + 20, rect_score.y + 15, _T("��ǰ�÷�:"));
        outtextxy_shaded(rect_score.x + 20, rect_score.y + 45, score_text);
    }
    else {
        settextstyle(52, 0, _T("IPix"));
        putimage_alpha(ResourcesManager::instance()->find_image("score_background"), &rect_score_game_over);
        outtextxy_shaded(rect_score_game_over.x + 40, rect_score_game_over.y + 30, _T("��ǰ�÷�:"));
        outtextxy_shaded(rect_score_game_over.x + 40, rect_score_game_over.y + 90, score_text);
        settextstyle(26, 0, _T("IPix"));
    }

}

void GameScene::generate_aerolite(int num) {
    for (int i = 0; i < num && i < Max_Aerolite_Num; i++) {
        int grid_x, grid_y;

        grid_x = rand() % 30; // �������x����
        grid_y = rand() % 20; // �������y����

        float place_x = grid_x * width_per_grid + rand() % width_per_grid;
        float place_y = grid_y * height_per_grid + rand() % height_per_grid;

        if (aerolites[i])
            aerolites[i]->start_fall(place_x, place_y); // ��������

            
    }
}


GameScene::GameScene() {
    { // ��ť��ʼ��
        int button_w = ResourcesManager::instance()->find_image("button")->getwidth();
        int button_h = ResourcesManager::instance()->find_image("button")->getheight();
        int button_x = (getwidth() - button_w) / 2 - 50;

        buttons_paused.clear();
        buttons_paused.reserve(3);
        buttons_paused.emplace_back(button_x, 250, 150, 80, _T("����"), [this]() {
            is_paused = false;
            });
        buttons_paused.emplace_back(button_x, 350, 150, 80, _T("����"));
        buttons_paused.emplace_back(button_x, 450, 150, 80, _T("�������˵�"), [this]() {
            SceneManager::instance()->switch_scene_to(SceneManager::SceneType::Menu);
            });
        
        buttons_game_over.clear();
        buttons_game_over.reserve(2);
        buttons_game_over.emplace_back(button_x - 150, 300, 150, 80, _T("���¿�ʼ"), [this]() {
            SceneManager::instance()->switch_scene_to(SceneManager::SceneType::Game);
            });
        buttons_game_over.emplace_back(button_x + 150, 300, 150, 80, _T("�������˵�"), [this]() {
            SceneManager::instance()->switch_scene_to(SceneManager::SceneType::Menu);
            });
    } 

    { // ��ʼ���߽���ײ��
        for (int i = 0; i < 4; i++) {
            collision_box_edges[i] = CollisionManager::instance()->create_collision_box();
            collision_box_edges[i]->set_layer_src(CollisionLayer::None);
            collision_box_edges[i]->set_layer_dst(CollisionLayer::Player);
        }
        collision_box_edges[0]->set_position(Vector2(-1, getheight() / 2)); // ��߽�
        collision_box_edges[0]->set_size(Vector2(2, getheight()));
        collision_box_edges[1]->set_position(Vector2(getwidth() + 1, getheight() / 2)); // �ұ߽�
        collision_box_edges[1]->set_size(Vector2(2, getheight()));
        collision_box_edges[2]->set_position(Vector2(getwidth() / 2, -1)); // �ϱ߽�
        collision_box_edges[2]->set_size(Vector2(getwidth(), 2));
        collision_box_edges[3]->set_position(Vector2(getwidth() / 2, getheight() + 1)); // �±߽�
        collision_box_edges[3]->set_size(Vector2(getwidth(), 2));
    } 
    
    { // ��ʼ��Ģ��������������
        for (int i = 0; i < Max_Mushroom_Num; i++) {
            mushrooms[i] = new Mushroom(1080 / 2, 720 / 2); // ��ʼ��Ģ��ָ��
        }

        for (int i = 0; i < Max_Aerolite_Num; i++) {
            aerolites[i] = new Aerolite(0, 0); // ��ʼ������ָ��
        }

        mushroom_generation_timer.set_one_shot(false); // ����Ϊ��һ���Լ�ʱ��
        mushroom_generation_timer.set_wait_time(Mushroom_Generation_Interval); // ����Ģ�����ɼ��ʱ��
        mushroom_generation_timer.set_on_timeout([this]() {
            generate_mushroom(); // ��������Ģ������
            });
        mushroom_generation_timer.pause(); // ��ʼ״̬����ͣ��ʱ��



        aerolite_generation_timer.set_one_shot(false);
        aerolite_generation_timer.set_wait_time(Aerolite_Generation_Interval);
        aerolite_generation_timer.set_on_timeout([this]() {
            int is_generated = rand() % 2;

            if (p1->get_score() >= 400) {
                generate_aerolite(3);
            }else if (p1->get_score() >= 300) {
                if (is_generated)
                    generate_aerolite(3); // �����������Ǻ���
            }
            else if (p1->get_score() >= 200) {
                if (is_generated)
                    generate_aerolite(2); // �����������Ǻ���
            }
            else if (p1->get_score() >= 100) {
                if (is_generated)
                    generate_aerolite(1); // �����������Ǻ���
            }
            });
        aerolite_generation_timer.pause(); // ��ʼ״̬����ͣ��ʱ��

 
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
    init_place_map(); // ��ʼ�����õ�ͼ
    can_place_at[1080 / 2 / width_per_grid][720 / 2 / height_per_grid] = false; // �������λ�ò������ɽ�����

    init_entities();
    

    buttons_paused[select_button_paused].set_selected(true);
    buttons_game_over[select_button_game_over].set_selected(true);

    if (p1) {
        delete p1;
        p1 = nullptr;
    }
    p1 = new NormalSnake();

    mushroom_generation_timer.restart();
    mushroom_generation_timer.resume(); // ����Ģ�����ɼ�ʱ��

    aerolite_generation_timer.restart();
    aerolite_generation_timer.resume();

}

void GameScene::on_exit() {
    mushroom_generation_timer.pause(); // ��ͣĢ�����ɼ�ʱ��
    aerolite_generation_timer.pause();

    destroy_entities();

    // �����Ҷ�����ڣ���ɾ����
    if (p1) {
        delete p1;
        p1 = nullptr;
    }

    buttons_paused[select_button_paused].set_selected(false); // ȡ��ѡ�е�ǰ��ť
    buttons_game_over[select_button_game_over].set_selected(false); // ȡ��ѡ�е�ǰ��ť
    select_button_paused = 0; // ����ѡ�а�ť����
    select_button_game_over = 0; // ����ѡ�а�ť����

    is_paused = false; // ������ͣ״̬
    is_game_over = false; // ������Ϸ����״̬
}

void GameScene::on_update(float delta) {
    

    if (is_paused || is_game_over) return; // �����Ϸ��ͣ��������򲻸�����Ϸ�߼�

    if (!p1->get_alive()) {
        is_game_over = true; // ��������������������Ϸ����״̬
        play_audio(_T("game_over"), false);
    }

    p1->on_update(delta);

    for (Entity* entity : entities) {
        entity->on_update(delta);
    }

    mushroom_generation_timer.on_update(delta); // ����Ģ�����ɼ�ʱ��
    aerolite_generation_timer.on_update(delta);

    for (Mushroom* mushroom : mushrooms) {
        if (!mushroom->is_need_growth()) { // ֻ�����Ѿ�������Ģ��
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
        if (!mushroom->is_need_growth()) { // ֻ��Ⱦ�Ѿ�������Ģ��
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
        if (is_paused && !is_game_over) { // �����Ϸ��ͣ�Ҳ�����Ϸ����״̬
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
        if (is_game_over && !is_paused) { // �����Ϸ�����Ҳ�����ͣ״̬
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
        grid_x = rand() % 30; // �������x����
        grid_y = rand() % 20; // �������y����

    } while (!can_place_at[grid_x][grid_y]); // ȷ������λ�ÿ��Է���Ģ��
    can_place_at[grid_x][grid_y] = false; // ��Ǹ�λ���ѱ�ռ��

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

