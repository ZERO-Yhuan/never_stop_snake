#pragma once

#include "../entity.h"

class WhiteFlower : public Entity {
private:

public:
    WhiteFlower(float x, float y) {
        position.x = x;
        position.y = y;

        image = ResourcesManager::instance()->find_image("white_flower");
    }

    void on_render(const Camera& camera) override {
        Rect rect_dst = {
        position.x - image->getwidth() / 2,
        position.y - image->getheight() / 2,
        image->getwidth(), image->getheight() };
        putimage_alpha(camera, image, &rect_dst);

        Entity::on_render(camera);
    }

};
