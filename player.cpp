#include "player.h"
#include "resources_manager.h"

#include <iostream>

std::vector<Vector2> path_point;
int max_path_points = 900;
float get_distance(const Vector2& a, const Vector2& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

Player::Player(float x, float y) 
{
    position.x = x;
    position.y = y;

    path_point.push_back(position);

    hit_box->set_size({ 10, 10 });
    hurt_box->set_size({ 10, 10 });

    hit_box->set_layer_src(CollisionLayer::None);
    hit_box->set_layer_dst(CollisionLayer::Food);
    hit_box->set_on_collide([&]() {
        add_body();
        });
    hurt_box->set_layer_src(CollisionLayer::Player);
    hurt_box->set_layer_dst(CollisionLayer::None);
    hurt_box->set_on_collide([&]() {
        set_death();
        });

    for (int i = 0; i < body_start_size; i++) {
        add_body();
        if (bodies.size() <= 2) // ǰ�������岻������ײ��⣬���Ż���������
            bodies[i].hit_box->set_enabled(false);
    }
    velocity = Vector2(1, 0) * SPEED; // ��ʼ�ٶ�����
    
}


void Player::add_body() {
    SnakeBody body;
    body.position.x = bodies.size() == 0 ? position.x - BODY_DISTANCE : bodies.back().position.x - BODY_DISTANCE;
    body.position.y = bodies.size() == 0 ? position.y: bodies.back().position.y;

    body.hit_box = CollisionManager::instance()->create_collision_box();
    body.hit_box->set_layer_src(CollisionLayer::None);
    body.hit_box->set_layer_dst(CollisionLayer::Player);
    body.hit_box->set_size({ 12, 12 });
    body.hit_box->set_position(body.position);
    bodies.push_back(body);
}

Player::~Player() {
    for(SnakeBody& body : bodies) {
        CollisionManager::instance()->destroy_collision_box(body.hit_box);
    }
}

void Player::set_death() {
    is_alive = false;
};

const Vector2 Player::get_move_axis() const {
    return Vector2(is_left_key_down ? -1 : 0 + is_right_key_down ? 1 : 0,
        is_up_key_down ? -1 : 0 + is_down_key_down ? 1 : 0).normalize();
}

void Player::on_hurt() {

}

// ���Ҿ���ָ����������Ĺ켣������
int Player::find_nearest_path_point(float target_distance) {
    float current_distance = 0.0f;

    for (size_t i = path_point.size() - 1; i > 0; i--) {
        current_distance += get_distance(path_point[i], path_point[i - 1]);

        if (current_distance >= target_distance) {
            return i;
        }
    }

    return 0; // ���û���ҵ������ص�һ����
}


void Player::on_update(float delta) {
    if (!is_alive)return;

    Vector2 move_axis = get_move_axis();
    if (move_axis.length() > 0)
        velocity = move_axis * SPEED;

    // ���½�ɫ����
    if(move_axis.x > 0 && move_axis.y == 0)
        face_dir = FaceDirection::Right;
    else if(move_axis.x < 0 && move_axis.y == 0)
        face_dir = FaceDirection::Left;
    else if(move_axis.y > 0 && move_axis.x == 0)
        face_dir = FaceDirection::Down;
    else if(move_axis.y < 0 && move_axis.x == 0)
        face_dir = FaceDirection::Up;

    Entity::on_update(delta);

    {
        // ��ӵ�ǰλ�õ�·����
        if (path_point.empty() || get_distance(position, path_point.back()) > 2.0f) {
            path_point.push_back(position);

            if (path_point.size() > max_path_points) {
                path_point.erase(path_point.begin());
            }
        }

        // ��������λ��

        {
            const float move_distance = SPEED * delta;

            for (int i = 0; i < bodies.size(); i++) {
                float target_distance = (i + 1) * BODY_DISTANCE;
                int target_index = find_nearest_path_point(target_distance);

                if (target_index < path_point.size()) {
                    const Vector2& target_pos = path_point[target_index];
                    SnakeBody& body = bodies[i];

                    // ƽ���ƶ���Ŀ��λ��
                    Vector2 direction = target_pos - body.position;
                    float dist = direction.length();

                    if (dist > 0.1f) {
                        // ����ÿ֡�ƶ����룬��������
                        float move_dist = dist < move_distance * 1.5f ? dist : move_distance * 1.5f;
                        body.position += direction.normalize() * move_dist;

                        // ������ײ��Ͷ���
                        body.hit_box->set_position(body.position);
                    }
                }
            }

        }
    }
}

void Player::on_input(const ExMessage& msg) {
    if (!is_alive)return;

    switch (msg.message)
    {
        case WM_KEYDOWN:
            switch (msg.vkcode)
            {
            case 0x41: // A
                if (face_dir == FaceDirection::Right) return; // �����ǰ�����ң����ܰ�A��
                is_left_key_down = true;
                break;
            case 0x44: // D
                if (face_dir == FaceDirection::Left) return; // �����ǰ���������ܰ�D��
                is_right_key_down = true;
                break;
            case 0x57: // W
                if (face_dir == FaceDirection::Down) return; // �����ǰ�����£����ܰ�W��
                is_up_key_down = true;
                break;
            case 0x53: // S
                if (face_dir == FaceDirection::Up) return; // �����ǰ�����ϣ����ܰ�S��
                is_down_key_down = true;
                break;
            }
            break;
        case WM_KEYUP:
            switch (msg.vkcode)
            {
            case 0x41: // A
                is_left_key_down = false;
                break;
            case 0x44: // D
                is_right_key_down = false;
                break;
            case 0x57: // W
                is_up_key_down = false;
                break;
            case 0x53: // S
                is_down_key_down = false;
                break;
            }
            break;
        case WM_LBUTTONDOWN: // ����������
            // ���������������¼�
            // ���磬������ҵ�Ŀ��λ��Ϊ�������λ��
            break;  
    }
}



// ��Player������Ӳ�ֵ����
void Player::render_body_smoothly() {
    setfillcolor(RGB(237, 147, 25)); // �������������ɫΪ��ɫ

    // ����һ������ͷ������������ڵ���б�
    std::vector<Vector2> control_points;
    control_points.push_back(position); // ���ͷ���ڵ�

    for (const SnakeBody& body : bodies) { // �������ڵ�
        control_points.push_back(body.position); 
    }

    // ȷ��������4��������Catmull-Rom��ֵ
    if (control_points.size() < 4) {
        // �㲻��ʱʹ�ü򵥻���
        for (int i = 0; i < control_points.size(); i++) {
            solidcircle(control_points[i].x, control_points[i].y, body_radius);
        }
        return;
    }

    // ʹ��Catmull-Rom������ֵ����ƽ������
    for (int i = 0; i < control_points.size() - 1; i++) {

        // Ϊÿ���߶�����10����ֵ��
        for (int j = 0; j < 10; j++) {
            float t = (float)j / 10.0f;

            // ����Catmull-Rom��ֵ��
            Vector2 new_point;

            // ����˵����
            if (i == 0) {
                // ��һ�Σ�ʹ��ǰ������͵ڶ��������һ����
                new_point = catmull_rom(
                    control_points[0],
                    control_points[0],
                    control_points[1],
                    control_points[2],
                    t
                );
            }
            else if (i == control_points.size() - 2) {
                // ���һ�Σ�ʹ�����������
                new_point = catmull_rom(
                    control_points[i - 1],
                    control_points[i],
                    control_points[i + 1],
                    control_points[i + 1],
                    t
                );
            }
            else {
                // �м�Σ�ʹ���ĸ���
                new_point = catmull_rom(
                    control_points[i - 1],
                    control_points[i],
                    control_points[i + 1],
                    control_points[i + 2],
                    t
                );
            }

            // ���Ʋ�ֵ��
            solidcircle(new_point.x, new_point.y, body_radius);
        }
    }
}

// Catmull-Rom������ֵ����(��������������)
Vector2 Player::catmull_rom(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3, float t) {
    float t2 = t * t;
    float t3 = t2 * t;

    float f1 = -0.5f * t3 + t2 - 0.5f * t;
    float f2 = 1.5f * t3 - 2.5f * t2 + 1.0f;
    float f3 = -1.5f * t3 + 2.0f * t2 + 0.5f * t;
    float f4 = 0.5f * t3 - 0.5f * t2;

    return Vector2(
        p0.x * f1 + p1.x * f2 + p2.x * f3 + p3.x * f4,
        p0.y * f1 + p1.y * f2 + p2.y * f3 + p3.y * f4
    );
}


void Player::on_render() {

    render_body_smoothly(); // ����ƽ��������

    render_eyes();

    Entity::on_render();
}

void Player::render_eyes() const {
    float dir_angle = std::atan2(velocity.y, velocity.x); // ���㵱ǰ�ƶ�����(�۾�����)�ĽǶ�
    float delta_angle = std::atan(eye_dis / 2 / eye_center_head_dis); // �����۾����۾����ĵĽǶ�ƫ����

    // �������ۺ����۵ĽǶ�
    float right_eye_angle = dir_angle + delta_angle;
    float left_eye_angle = dir_angle - delta_angle;

    // �������ۺ����۵�XY����ϵƫ����
    float right_eye_delta_x = eye_dis * std::cos(right_eye_angle);
    float right_eye_delta_y = eye_dis * std::sin(right_eye_angle);
    float left_eye_delta_x = eye_dis * std::cos(left_eye_angle);
    float left_eye_delta_y = eye_dis * std::sin(left_eye_angle);

    setfillcolor(BLACK); // �����۾������ɫΪ��ɫ
    fillcircle(position.x + right_eye_delta_x, position.y + right_eye_delta_y, 2);
    fillcircle(position.x + left_eye_delta_x, position.y + left_eye_delta_y, 2);
}


