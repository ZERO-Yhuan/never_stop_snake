#pragma once

#include <string>
#include <unordered_map>
#include <graphics.h>

#include "animation.h"
#include "vector2.h"
#include "collision_box.h"
#include "collision_manager.h"
#include "resources_manager.h"


class Entity {
public:
    Entity();
    virtual ~Entity();


protected:
    Vector2 position;
    Vector2 velocity;
    CollisionBox* hit_box;
    CollisionBox* hurt_box;
    std::unordered_map<std::string, Animation> animation_pool;
    Animation* current_animation = nullptr;
    IMAGE* image = nullptr;

public:
    virtual void set_position(const float x, const float y) {
        position.x = x;
        position.y = y;
    }

    virtual void set_position(const Vector2& position) {
        this->position = position;
    }

    virtual const Vector2& get_position() const {
        return position;
    }

    virtual void set_velocity(const float x, const float y) {
        velocity.x = x;
        velocity.y = y;
    }

    virtual void set_velocity(const Vector2& velocity) {
        this->velocity = velocity;
    }

    virtual const Vector2& get_velocity() const {
        return velocity;
    }

    virtual CollisionBox* get_hit_box() const {
        return hit_box;
    }
    virtual CollisionBox* get_hurt_box() const {
        return hurt_box;
    }

    virtual void on_input(const ExMessage& msg);
    virtual void on_update(float delta);
    virtual void on_render();

    virtual void on_hurt();

    void set_animation(const std::string& id);

};