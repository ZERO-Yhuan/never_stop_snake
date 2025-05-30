#pragma once

#include "../resource/atlas.h"
#include "../tool/util.h"

#include <unordered_map>
#include <string>

// 单例设计模式
class ResourcesManager {
private:
    ResourcesManager();
    ~ResourcesManager();

    // 图片旋转函数
    // -----------------------------------------------------

private:
    static ResourcesManager* _instance; // 单例对象

    std::unordered_map<std::string, Atlas*> atlas_pool; // 纹理池
    std::unordered_map<std::string, IMAGE*> image_pool; // 图片池


public:
    static ResourcesManager* instance();

    void load();
    Atlas* find_atlas(const std::string& id) const; // 查找纹理
    IMAGE* find_image(const std::string& id) const; // 查找图片

};