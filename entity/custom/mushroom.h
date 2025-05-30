#pragma once

#include "../entity.h"


class Mushroom : public Entity {
private:
	Timer to_show_timer;
    Timer to_hide_timer;

	bool need_growth = true;
	
public:
	static const int food_score = 10;

public:
    Mushroom(float x = 1080 / 2, float y = 720 / 2){

		{
            to_show_timer.set_one_shot(true);
			to_show_timer.set_wait_time(5.0f);
			to_show_timer.set_on_timeout([this]() {
				current_animation = &animation_pool["show"];
				current_animation->reset();
                });
			to_show_timer.pause();

            to_hide_timer.set_one_shot(true);
            to_hide_timer.set_wait_time(5.0f);
			to_hide_timer.set_on_timeout([this]() {
                int should_hide = rand() % 2;

				if (should_hide) {
					current_animation = &animation_pool["hide"];
					current_animation->reset();
				}
				});
            to_hide_timer.pause();
		}


        position.x = x;
        position.y = y;

        Animation& animation_growth = animation_pool["growth"];
		animation_growth.set_loop(false);
        animation_growth.add_frame(ResourcesManager::instance()->find_atlas("mushroom_growth"));
        animation_growth.set_interval(0.2f);
		animation_growth.set_position(position);
		animation_growth.set_on_finished([this]() {
            hurt_box->set_enabled(true);
            current_animation = &animation_pool["normal"];
            to_hide_timer.restart();
			to_hide_timer.resume();
		});
        Animation& animation_hide = animation_pool["hide"];
		animation_hide.set_loop(false);
		animation_hide.add_frame(ResourcesManager::instance()->find_atlas("mushroom_hide"));
		animation_hide.set_interval(0.2f);
		animation_hide.set_position(position);
		animation_hide.set_on_finished([this]() {
			hurt_box->set_enabled(false);
			to_show_timer.restart();
			to_show_timer.resume();

			});
        Animation& animation_show = animation_pool["show"];
		animation_show.set_loop(false);
		animation_show.add_frame(ResourcesManager::instance()->find_atlas("mushroom_show"));
		animation_show.set_interval(0.2f);
		animation_show.set_position(position);
		animation_show.set_on_finished([this]() {
			hurt_box->set_enabled(true);
			to_hide_timer.restart();
            current_animation = &animation_pool["normal"];
			});
        Animation& animation_normal = animation_pool["normal"];
        animation_normal.set_loop(true);
        animation_normal.add_frame(ResourcesManager::instance()->find_atlas("mushroom_normal"));
        animation_normal.set_interval(0.4f);
        animation_normal.set_position(position);


        hurt_box->set_position(position);
        hurt_box->set_size(23, 25);
		hurt_box->set_layer_src(CollisionLayer::Food);
		hurt_box->set_layer_dst(CollisionLayer::None);
		hurt_box->set_on_collide([this] {
			play_audio(_T("eat"));
            need_growth = true;
            hurt_box->set_enabled(false);
            to_show_timer.pause();
            to_hide_timer.pause();
			});
		hurt_box->set_enabled(false);	

		image = ResourcesManager::instance()->find_atlas("mushroom_normal")->get_image(0);
	}

	void growth() {
        need_growth = false;
		for(auto& animation : animation_pool) {
			animation.second.set_position(position);
        }


		current_animation = &animation_pool["growth"];
        current_animation->reset();
	}

	void on_update(float delta) override {
		if (need_growth) return;

		to_show_timer.on_update(delta);
		to_hide_timer.on_update(delta);

        Entity::on_update(delta);
    }

	void on_render(const Camera& camera) override {
        if (need_growth) return;

		render_shade(camera, position.x - 9, position.y + image->getheight() / 2 - 3 - 4,
			position.x + 9, position.y + image->getheight() / 2 - 3 + 6);

        Entity::on_render(camera);
	}

	bool is_need_growth() const {
        return need_growth;
	}


};