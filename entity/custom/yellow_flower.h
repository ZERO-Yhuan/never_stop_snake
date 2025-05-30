#pragma once

#include "../entity.h"

class YellowFlower : public Entity {
private:

public:
    YellowFlower(float x, float y) {
        position.x = x;
        position.y = y;

        Animation& normal_animation = animation_pool["normal"];
        normal_animation.add_frame(ResourcesManager::instance()->find_atlas("yellow_flower"));
        normal_animation.set_interval(0.2f);
        normal_animation.set_loop(true);
        normal_animation.set_position(position);
        current_animation = &animation_pool["normal"];

        image = ResourcesManager::instance()->find_atlas("yellow_flower")->get_image(0);
    }

    void on_render(const Camera& camera) override {
        if(!current_animation) return;

        render_shade(camera, position.x - 8, position.y + 11,
            position.x + 8, position.y + 19);

        Entity::on_render(camera);
    }

};
