#pragma once

#include "entity.h"

#include <iostream>
class Rock : public Entity {
private:
    int rock_type; 
    float render_ratio; // äÖÈ¾±ÈÀý

public:
    Rock(float x = 0, float y = 0, int rock_type = 1, float render_ratio = 1.0f)
        : rock_type(rock_type), render_ratio(render_ratio){
        this->set_position({ x, y });

        hit_box = CollisionManager::instance()->create_collision_box();
        switch (rock_type) {
        case 1:
            image = ResourcesManager::instance()->find_image("rock_type_1");
            break;
        case 2:
            image = ResourcesManager::instance()->find_image("rock_type_2");
            break;
        }
        hit_box->set_size(Vector2(image->getwidth() * render_ratio, image->getheight() * render_ratio));
        hit_box->set_layer_src(CollisionLayer::None);
        hit_box->set_layer_dst(CollisionLayer::Player);
        hit_box->set_position({ x, y });
        
    }
    ~Rock() override = default;

    void set_position(const float x, const float y) override{
        Entity::set_position(x, y);

        if (hit_box) {
            hit_box->set_position({ x, y });
        }
        if (hurt_box) {
            hurt_box->set_position({ x, y });
        }
    }

    void set_position(const Vector2& position) override {
        Entity::set_position(position);


        if (hit_box) {
            hit_box->set_position(position);
        }
        if (hurt_box) {
            hurt_box->set_position(position);
        }
    }

    void on_render() override {
        setfillcolor(DARKGRAY);
        solidellipse(
            position.x - image->getwidth() * render_ratio / 2, position.y + image->getheight() * render_ratio / 2 - 3 - 6,
            position.x + image->getwidth() * render_ratio / 2, position.y + image->getheight() * render_ratio / 2 - 3 + 6);


        Rect rect_dst = { 
            position.x - image->getwidth() * render_ratio / 2,
            position.y - image->getheight() * render_ratio / 2,
            image->getwidth()* render_ratio, image->getheight()* render_ratio };
        putimage_alpha(image, &rect_dst);
        
        
    }
};
