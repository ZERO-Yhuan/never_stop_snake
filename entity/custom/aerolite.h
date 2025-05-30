#pragma once

#include "../entity.h"

extern Camera main_camera;

class Aerolite : public Entity {
private:
    Vector2 target_pos;

    bool is_boomed = false;


public:
    void start_fall(float x, float y) {

        target_pos.x = x;
        target_pos.y = y;
        position.x = x + 1180 + rand() % 100;
        position.y = y - 820 + rand() % 80;

        current_animation = &animation_pool["normal"];
        animation_pool["boom"].reset();

        velocity = Vector2(target_pos.x - position.x, target_pos.y - position.y).normalize() * (270 + rand() % 50);

        is_boomed = false;
    }

    Aerolite(float x = 0, float y = 0) {
        position.x = x + 1180;
        position.y = y + 820;

        Animation& normal_animation = animation_pool["normal"];
        normal_animation.add_frame(ResourcesManager::instance()->find_atlas("aerolite"));
        normal_animation.set_interval(0.4f);
        normal_animation.set_loop(true);
        normal_animation.set_position(position);

        Animation& boom_animation = animation_pool["boom"];
        boom_animation.add_frame(ResourcesManager::instance()->find_atlas("boom"));
        boom_animation.set_interval(0.2f);
        boom_animation.set_loop(false);
        boom_animation.set_position(position);
        boom_animation.set_on_finished([this] {
            hit_box->set_enabled(false);
            is_boomed = true;
            });


        hit_box->set_enabled(false);
        hit_box->set_layer_src(CollisionLayer::None);
        hit_box->set_layer_dst(CollisionLayer::PlayerBody);
        hit_box->set_size(Vector2(100, 44));
        hit_box->set_position(position);

        
    }
    ~Aerolite() = default;

    void on_update(float delta) override {
        if (is_boomed) return;

        if (position.x > target_pos.x - 5 && position.x < target_pos.x + 5
            && position.y < target_pos.y + 5 && position.y > target_pos.y - 5 && velocity.length() != 0) {
            main_camera.shake(10, 0.7f); // ±¬Õ¨»Î¶¯ 
            play_audio(_T("boom")); // ²¥·Å±¬Õ¨ÒôĞ§
            current_animation = &animation_pool["boom"]; // ÇĞ»»¶¯»­
            hit_box->set_enabled(true); // ¿ªÆôÅö×²Ìå

            velocity = { 0,0 }; // Í£Ö¹ÒÆ¶¯
        } 

        Entity::on_update(delta);
    }

    void on_render(const Camera& camera) override {
        if (is_boomed) return;


        Entity::on_render(camera);
    }


};