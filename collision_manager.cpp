#include "collision_manager.h"

#include <graphics.h>

// ����ģʽʵ��
CollisionManager* CollisionManager::_instance = nullptr;

CollisionManager* CollisionManager::instance() {
    if (_instance == nullptr) {
        _instance = new CollisionManager();
    }
    return _instance;
}

CollisionManager::CollisionManager() = default;
CollisionManager::~CollisionManager() = default;

// ����ģʽʵ��
CollisionBox* CollisionManager::create_collision_box() {
    CollisionBox* collision_box = new CollisionBox();
    collision_box_list.push_back(collision_box); // ���´�������ײ����ӵ��б���
    return collision_box;
}

void CollisionManager::destroy_collision_box(CollisionBox* collision_box) {
    collision_box_list.erase(std::remove(collision_box_list.begin(),
        collision_box_list.end(), collision_box), collision_box_list.end()); // ���б���ɾ����ײ��
    delete collision_box;
}

#include <iostream>
void CollisionManager::process_collide() {
    for(CollisionBox* collision_box_src : collision_box_list) {

        if (!collision_box_src->enabled || collision_box_src->layer_dst == CollisionLayer::None) {
            continue; // �����ײ��δ���û�Դ��ΪNone��������
        }


        for (CollisionBox * collision_box_dst : collision_box_list) {
            if (!collision_box_dst) {
                std::cout << "collision_box_dst is nullptr" << std::endl;
                continue;
            }

            if (collision_box_dst == collision_box_src ) {
                continue; // �����ײ��δ���û�Ŀ���ΪNone������Դ���Ŀ��㲻ƥ�䣬������
            }

            if (!collision_box_dst->enabled
                || collision_box_src->layer_dst != collision_box_dst->layer_src) continue;

            // ���x����ײ
            bool is_collide_x = (max(collision_box_src->position.x + collision_box_src->size.x / 2,
                collision_box_dst->position.x + collision_box_dst->size.x / 2) 
                - min(collision_box_src->position.x - collision_box_src->size.x / 2, 
                    collision_box_dst->position.x - collision_box_dst->size.x / 2) 
                <= collision_box_src->size.x + collision_box_dst->size.x); 
            // ���y����ײ
            bool is_collide_y = (max(collision_box_src->position.y + collision_box_src->size.y / 2, 
                collision_box_dst->position.y + collision_box_dst->size.y / 2)
                - min(collision_box_src->position.y - collision_box_src->size.y / 2,
                    collision_box_dst->position.y - collision_box_dst->size.y / 2)
                <= collision_box_src->size.y + collision_box_dst->size.y); 

            if (is_collide_x && is_collide_y) {
                if( collision_box_dst->on_collide)
                    collision_box_dst->on_collide(); // ���������ײ��Ŀ����ײ���д�����������ô�����
                if( collision_box_src->on_collide)
                    collision_box_src->on_collide(); // ���������ײ��Դ��ײ���д�����������ô�����
            }
        }
    }
}

void CollisionManager::on_debug_render() {
    for(CollisionBox* collision_box : collision_box_list) {
        // ������ײ�еľ��ο�
        setlinecolor(collision_box->enabled ? RGB(255, 195, 195) : RGB(115, 115, 175));
        rectangle(collision_box->position.x - collision_box->size.x / 2,
            collision_box->position.y - collision_box->size.y / 2,
            collision_box->position.x + collision_box->size.x / 2,
            collision_box->position.y + collision_box->size.y / 2);
        
    }
}