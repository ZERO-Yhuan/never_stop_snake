#pragma once

#include "vector2.h"
#include  "collision_layer.h"

#include <functional>

class CollisionManager; // ǰ������CollisionManager��

class CollisionBox
{
    // ��Ԫ��������CollisionManager���Է���CollisionBox��˽�г�Ա
    friend class CollisionManager;

private:
    Vector2 size; // ��ײ��ߴ�
    Vector2 position; // ��ײ������λ��
    bool enabled = true; // �Ƿ�������ײ���
    std::function<void()> on_collide; // ��ײ�ص�����
    CollisionLayer layer_src = CollisionLayer::None; // ��ײԴ��
    CollisionLayer layer_dst = CollisionLayer::None; // ��ײĿ���

// �������ʼ����������Ϊ˽�к���������Ԫ����CollisionManagerͳһ����
private:
    CollisionBox() = default;
    ~CollisionBox() = default;

public:
    // ������ײ���Ƿ�����
    void set_enabled(bool enabled) 
    { 
        this->enabled = enabled; 
    } 

    void set_layer_src(CollisionLayer layer_src) 
    { 
        this->layer_src = layer_src;
    }

    void set_layer_dst(CollisionLayer layer_dst) 
    { 
        this->layer_dst = layer_dst;
    }

    void set_on_collide(std::function<void()> on_collide) 
    { 
        this->on_collide = on_collide;
    }
    void set_size(const Vector2& size) 
    { 
        this->size = size;
    }
    void set_position(const Vector2& position) 
    { 
        this->position = position; 
    }
    const Vector2& get_size() const 
    { 
        return size; 
    }
};