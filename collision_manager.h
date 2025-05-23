#pragma once

#include "collision_box.h"

#include <vector>

// ����ģʽ
class CollisionManager {
public:
    static CollisionManager* instance();

    // ����ģʽ
    CollisionBox* create_collision_box();
    void destroy_collision_box(CollisionBox* collision_box);

    void process_collide();
    void on_debug_render();

private:
    static CollisionManager* _instance;

    std::vector<CollisionBox*> collision_box_list;

private:
    CollisionManager();
    ~CollisionManager();

};