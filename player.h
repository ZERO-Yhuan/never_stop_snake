#pragma once

#include "entity.h"


class Player : public Entity {
public:
    enum class FaceDirection {
        Up,
        Down,
        Left,
        Right
    };
private:
    struct SnakeBody{
        Vector2 position;
        CollisionBox* hit_box;
    };

private:
    FaceDirection face_dir = FaceDirection::Right; // 朝向
    
    bool is_left_key_down = false;
    bool is_right_key_down = false;
    bool is_up_key_down = false;
    bool is_down_key_down = false;

    bool is_alive = true;

    std::vector<SnakeBody> bodies; // 蛇身

private:
    const float SPEED = 150.0f;
    const int BODY_DISTANCE = 12;
    const float eye_center_head_dis = 8; // 眼睛中心到头部中心的距离
    const float eye_dis = 6; // 眼睛间距
    const float body_radius = 8; // 蛇身半径
    const int body_start_size = 4; // 初始蛇身长度

private:
    static Vector2 catmull_rom(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3, float t);
    void render_body_smoothly();
    void set_death();
    void render_eyes() const; // 渲染眼睛
    void add_body(); // 添加蛇身
    int find_nearest_path_point(float target_distance); // 查找离目标点最近的路径点

public:
    Player(float x = 1080 / 2, float y = 720 / 2); // 初始位置在屏幕中心
    ~Player();

    void on_input(const ExMessage& msg) override;
    void on_update(float delta) override;
    void on_render() override;

    void on_hurt() override;

    bool get_alive() const { return is_alive; }
    
    const Vector2 get_move_axis() const; // 获取移动方向向量

};