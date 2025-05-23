#pragma once

#include "vector2.h"
#include  "collision_layer.h"

#include <functional>

class CollisionManager; // 前向声明CollisionManager类

class CollisionBox
{
    // 友元类声明，CollisionManager可以访问CollisionBox的私有成员
    friend class CollisionManager;

private:
    Vector2 size; // 碰撞箱尺寸
    Vector2 position; // 碰撞箱中心位置
    bool enabled = true; // 是否启用碰撞检测
    std::function<void()> on_collide; // 碰撞回调函数
    CollisionLayer layer_src = CollisionLayer::None; // 碰撞源层
    CollisionLayer layer_dst = CollisionLayer::None; // 碰撞目标层

// 将碰箱初始化函数声明为私有函数，由友元——CollisionManager统一管理
private:
    CollisionBox() = default;
    ~CollisionBox() = default;

public:
    // 设置碰撞箱是否启用
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