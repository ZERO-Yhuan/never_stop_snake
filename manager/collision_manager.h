#pragma once


#include "../collision/collision_box.h"

#include <vector>

#include "../tool/camera.h"

// ����ģʽ
class CollisionManager {
public:
    static CollisionManager* instance();

    // ����ģʽ
    CollisionBox* create_collision_box();
    void destroy_collision_box(CollisionBox* collision_box);

    void process_collide();
    void on_debug_render(const Camera& camera);

private:
    static CollisionManager* _instance;

    std::vector<CollisionBox*> collision_box_list;

private:
    CollisionManager();
    ~CollisionManager();

};