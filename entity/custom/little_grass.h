#pragma once

#include "../entity.h"

class LittleGrass : public Entity {
private:

public:
    LittleGrass(float x, float y) {
        position.x = x;
        position.y = y;

        Animation& normal_animation = animation_pool["normal"];
        normal_animation.add_frame(ResourcesManager::instance()->find_atlas("little_grass"));
        normal_animation.set_interval(0.2f);
        normal_animation.set_loop(true);
        normal_animation.set_position(position);
        current_animation = &animation_pool["normal"];
    }

    void on_render(const Camera& camera) override {
        if(!current_animation) return;

        render_shade(camera, position.x - 9, position.y + 12,
            position.x + 7, position.y + 18);

        Entity::on_render(camera);
    }


}; 
