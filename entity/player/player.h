#pragma once

#include "../entity.h"
#include "../custom/mushroom.h"


class Player : public Entity {
public:
    enum class FaceDirection {
        Up,
        Down,
        Left,
        Right
    };
private:
    std::vector<Vector2> path_point; 
    int max_path_points = 900;
    float get_distance(const Vector2& a, const Vector2& b);
    int find_nearest_path_point(float target_distance); // 查找离目标点最近的路径点
    static Vector2 catmull_rom(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3, float t);


protected:
    struct SnakeBody{ // 蛇身结构体
        Vector2 position;
        CollisionBox* hit_box = nullptr;
        CollisionBox* hurt_box = nullptr;
    };

protected: // 以下是蛇的基础属性
    float START_SPEED = 150.0f;
    int BODY_DISTANCE = 12; // 身体间距

    float eye_center_head_dis = 8; // 眼睛中心到头部中心的距离
    float eye_dis = 6; // 眼睛间距
    float body_radius = 8; // 蛇身半径
    int body_start_size = 4; // 初始蛇身长度

protected:
    FaceDirection face_dir = FaceDirection::Right; // 朝向
    
    bool is_left_key_down = false;
    bool is_right_key_down = false;
    bool is_up_key_down = false;
    bool is_down_key_down = false;

    bool is_alive = true; // 是否存活

    bool need_grow = false; // 吃到蘑菇后标记需要生长
    /*
    * 重要！！！！！！，防止在碰撞检测过程中添加身体导致碰撞箱列表重新分配内存照成野指针，导致程序崩溃
    */

    float current_speed = START_SPEED; // 当前速度

    std::vector<SnakeBody> bodies; // 蛇身

    int score = 0; // 得分


protected:
    void render_body_smoothly(const Camera& camera, COLORREF eye_color); // 平滑的渲染身体
    void render_eyes(const Camera& camera, COLORREF eye_color) const; // 渲染眼睛

    void add_body(); // 添加蛇身
    void increase_score() { score += Mushroom::food_score; }

public:
    Player(float x = 1080 / 2, float y = 720 / 2); // 初始位置在屏幕中心
    ~Player();

    int get_score() const { return score; }
    bool get_alive() const { return is_alive; }
    const Vector2 get_move_axis() const; // 获取移动方向向量


    virtual void on_input(const ExMessage& msg) override;
    virtual void on_update(float delta) override;
    virtual void on_render(const Camera& camera) override;

    virtual void on_hurt() override;



};