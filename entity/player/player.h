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
    int find_nearest_path_point(float target_distance); // ������Ŀ��������·����
    static Vector2 catmull_rom(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3, float t);


protected:
    struct SnakeBody{ // ����ṹ��
        Vector2 position;
        CollisionBox* hit_box = nullptr;
        CollisionBox* hurt_box = nullptr;
    };

protected: // �������ߵĻ�������
    float START_SPEED = 150.0f;
    int BODY_DISTANCE = 12; // ������

    float eye_center_head_dis = 8; // �۾����ĵ�ͷ�����ĵľ���
    float eye_dis = 6; // �۾����
    float body_radius = 8; // ����뾶
    int body_start_size = 4; // ��ʼ������

protected:
    FaceDirection face_dir = FaceDirection::Right; // ����
    
    bool is_left_key_down = false;
    bool is_right_key_down = false;
    bool is_up_key_down = false;
    bool is_down_key_down = false;

    bool is_alive = true; // �Ƿ���

    bool need_grow = false; // �Ե�Ģ��������Ҫ����
    /*
    * ��Ҫ����������������ֹ����ײ��������������嵼����ײ���б����·����ڴ��ճ�Ұָ�룬���³������
    */

    float current_speed = START_SPEED; // ��ǰ�ٶ�

    std::vector<SnakeBody> bodies; // ����

    int score = 0; // �÷�


protected:
    void render_body_smoothly(const Camera& camera, COLORREF eye_color); // ƽ������Ⱦ����
    void render_eyes(const Camera& camera, COLORREF eye_color) const; // ��Ⱦ�۾�

    void add_body(); // �������
    void increase_score() { score += Mushroom::food_score; }

public:
    Player(float x = 1080 / 2, float y = 720 / 2); // ��ʼλ������Ļ����
    ~Player();

    int get_score() const { return score; }
    bool get_alive() const { return is_alive; }
    const Vector2 get_move_axis() const; // ��ȡ�ƶ���������


    virtual void on_input(const ExMessage& msg) override;
    virtual void on_update(float delta) override;
    virtual void on_render(const Camera& camera) override;

    virtual void on_hurt() override;



};