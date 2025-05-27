#include "entity.h"

Entity::Entity() {
    hit_box = CollisionManager::instance()->create_collision_box();
    hurt_box = CollisionManager::instance()->create_collision_box();

}
Entity::~Entity() {
    CollisionManager::instance()->destroy_collision_box(hit_box);
    CollisionManager::instance()->destroy_collision_box(hurt_box);

}

void Entity::on_update(float delta) {
    position += velocity * delta;

    hit_box->set_position(position);
    hurt_box->set_position(position);

    if (!current_animation) return;

    current_animation->on_update(delta);
    current_animation->set_position(position);
}

void Entity::on_render() {
    if (!current_animation) return;
    current_animation->on_render();
}

void Entity::set_animation(const std::string& id) {
    current_animation = &animation_pool[id];
    current_animation->reset();
}

void Entity::on_hurt() {
}

void Entity::on_input(const ExMessage& msg) {
}