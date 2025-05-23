#include "collision_manager.h"

#include <graphics.h>

// 单例模式实现
CollisionManager* CollisionManager::_instance = nullptr;

CollisionManager* CollisionManager::instance() {
    if (_instance == nullptr) {
        _instance = new CollisionManager();
    }
    return _instance;
}

CollisionManager::CollisionManager() = default;
CollisionManager::~CollisionManager() = default;

// 工厂模式实现
CollisionBox* CollisionManager::create_collision_box() {
    CollisionBox* collision_box = new CollisionBox();
    collision_box_list.push_back(collision_box); // 将新创建的碰撞盒添加到列表中
    return collision_box;
}

void CollisionManager::destroy_collision_box(CollisionBox* collision_box) {
    collision_box_list.erase(std::remove(collision_box_list.begin(),
        collision_box_list.end(), collision_box), collision_box_list.end()); // 从列表中删除碰撞盒
    delete collision_box;
}

void CollisionManager::process_collide() {
    for(CollisionBox* collision_box_src : collision_box_list) {
        if (!collision_box_src->enabled || collision_box_src->layer_src == CollisionLayer::None) {
            continue; // 如果碰撞盒未启用或源层为None，则跳过
        }

        for (CollisionBox * collision_box_dst : collision_box_list) {
            if (!collision_box_dst->enabled || collision_box_dst->layer_src == CollisionLayer::None
                || collision_box_src->layer_dst != collision_box_dst->layer_src) {
                continue; // 如果碰撞盒未启用或目标层为None，或者源层和目标层不匹配，则跳过
            }

            // 检测x轴碰撞
            bool is_collide_x = (max(collision_box_src->position.x + collision_box_src->size.x / 2,
                collision_box_dst->position.x + collision_box_dst->size.x / 2) 
                - min(collision_box_src->position.x - collision_box_src->size.x / 2, 
                    collision_box_dst->position.x - collision_box_dst->size.x / 2) 
                <= collision_box_src->size.x + collision_box_dst->size.x); 
            // 检测y轴碰撞
            bool is_collide_y = (max(collision_box_src->position.y + collision_box_src->size.y / 2, 
                collision_box_dst->position.y + collision_box_dst->size.y / 2)
                - min(collision_box_src->position.y - collision_box_src->size.y / 2,
                    collision_box_dst->position.y - collision_box_dst->size.y / 2)
                <= collision_box_src->size.y + collision_box_dst->size.y); 

            if (is_collide_x && is_collide_y && collision_box_dst->on_collide) {
                collision_box_dst->on_collide(); // 如果发生碰撞且目标碰撞盒有处理函数，则调用处理函数
            }
        }
    }
}

void CollisionManager::on_debug_render() {
    for(CollisionBox* collision_box : collision_box_list) {
        // 绘制碰撞盒的矩形框
        setlinecolor(collision_box->enabled ? RGB(255, 195, 195) : RGB(115, 115, 175));
        rectangle(collision_box->position.x - collision_box->size.x / 2,
            collision_box->position.y - collision_box->size.y / 2,
            collision_box->position.x + collision_box->size.x / 2,
            collision_box->position.y + collision_box->size.y / 2);
        
    }
}