#include "resources_manager.h"

struct ImageResInfo {
    std::string id; // 图片ID
    LPCTSTR path; // 图片路径
};
struct AtlasResInfo {
    std::string id; // 图片ID
    LPCTSTR path; // 图片路径
    int num_frames; // 图片帧数
};

// 图片资源
static const std::vector<ImageResInfo> image_info_list = {
    { "background", _T(R"(res\background.png)")}, // 背景图片
    // { "player", _T(R"(res\snake\snake.png)")}, // 玩家图片
};

// 图集资源
static const std::vector<AtlasResInfo> atlas_info_list = {
    /*
    { "food", _T(R"(res\food\food.png)"), 1}, // 食物图片
    { "enemy", _T(R"(res\enemy\enemy.png)"), 1}, // 敌人图片
    { "bullet", _T(R"(res\bullet\bullet.png)"), 1}, // 子弹图片
    */
};

// 检查图片是否有效
static inline bool check_image_valid(IMAGE* image) {
    return GetImageBuffer(image);
}

// 初始化单例对象
ResourcesManager* ResourcesManager::_instance = nullptr; 

// 获取单例对象
ResourcesManager* ResourcesManager::instance() { 
    if (_instance == nullptr) {
        _instance = new ResourcesManager();
    }
    return _instance;
}

ResourcesManager::ResourcesManager() = default;
ResourcesManager::~ResourcesManager() = default;

// 查找图片资源
IMAGE* ResourcesManager::find_image(const std::string& id) const { 
    const auto& it = image_pool.find(id); // 查找图片资源
    if (it == image_pool.end()) // 如果没有找到
        return nullptr; // 返回空指针
    return it->second; // 返回图片资源
}
// 查找图集资源
Atlas* ResourcesManager::find_atlas(const std::string& id) const {
    const auto& it = atlas_pool.find(id);
    if (it == atlas_pool.end())
        return nullptr;
    return it->second;
}

// 加载资源
void ResourcesManager::load() {
    // 加载图片资源
    for (const auto& info : image_info_list) { 
        IMAGE* image = new IMAGE;
        loadimage(image, info.path);

        if (!check_image_valid(image)) // 检测图片是否有效
            throw info.path;

        image_pool[info.id] = image;
    }

    // 加载图集资源
    for (const auto& info : atlas_info_list) { 
        Atlas* atlas = new Atlas;
        atlas->load(info.path, info.num_frames);

        for (int i = 0; i < atlas->get_size(); i++) { // 检测每一帧图片是否有效
            IMAGE* image = atlas->get_image(i);
            if(!check_image_valid(image))
                throw info.path;
        }

        atlas_pool[info.id] = atlas;
    }

    /*
    load_audio(); // 加载音频资源
    */
}




