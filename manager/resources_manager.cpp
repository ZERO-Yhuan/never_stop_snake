#include "resources_manager.h"

struct ImageResInfo {
    std::string id; // 图片ID
    LPCTSTR path; // 图片路径
};
struct AtlasResInfo {
    std::string id; // 图片ID
    LPCTSTR path; // 图片路径
    int num_frames; // 图片帧数

    AtlasResInfo(const std::string& id, LPCTSTR path, int num_frames) : id(id), path(path), num_frames(num_frames) {}
};

// 图片资源
static const std::vector<ImageResInfo> image_info_list = {
    { "background", _T(R"(res\assets\image\background.png)")}, 
    { "score_background", _T(R"(res\assets\image\score_background\score_background.png)")},
    { "white_flower", _T(R"(res\assets\image\white_flower\white_flower.png)")},
    { "rock_type_1", _T(R"(res\assets\animation\rock\rock_0.png)")},
    { "rock_type_2", _T(R"(res\assets\animation\rock\rock_1.png)")},
    

    { "button", _T(R"(res\assets\animation\button\button.png)")},
    { "button_selected", _T(R"(res\assets\animation\button\button_selected.png)")},
};

// 图集资源
static const std::vector<AtlasResInfo> atlas_info_list = {
    { "snake_menu", _T(R"(res\assets\image\snake_menu\snake_menu_%d.png)"), 7},
    { "tree", _T(R"(res\assets\image\tree\tree_%d.png)"), 4},

    { "big_grass", _T(R"(res\assets\image\big_grass\big_grass_%d.png)"), 4},
    { "little_grass", _T(R"(res\assets\image\little_grass\little_grass_%d.png)"), 4},
    { "red_flower", _T(R"(res\assets\image\red_flower\red_flower_%d.png)"), 4},
    { "yellow_flower", _T(R"(res\assets\image\yellow_flower\yellow_flower_%d.png)"), 4},

    { "mushroom_normal", _T(R"(res\assets\animation\mushroom_normal\mushroom_normal_%d.png)"), 2},
    { "mushroom_growth", _T(R"(res\assets\animation\mushroom_growth\mushroom_growth_%d.png)"), 7},
    { "mushroom_hide", _T(R"(res\assets\animation\mushroom_hide\mushroom_hide_%d.png)"), 4},
    { "mushroom_show", _T(R"(res\assets\animation\mushroom_show\mushroom_show_%d.png)"), 4},

    { "aerolite", _T(R"(res\assets\animation\aerolite\aerolite_%d.png)"), 2},
    { "boom", _T(R"(res\assets\animation\boom\boom_%d.png)"), 5},

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
    load_audio(_T(R"(res\assets\audio\bgm\start_scene_bgm.mp3)"), _T("start_menu_bgm"));

    load_audio(_T(R"(res\assets\audio\sfx\click.mp3)"), _T("click"));
    load_audio(_T(R"(res\assets\audio\sfx\boom.mp3)"), _T("boom"));
    load_audio(_T(R"(res\assets\audio\sfx\eat.mp3)"), _T("eat"));
    load_audio(_T(R"(res\assets\audio\sfx\game_over.mp3)"), _T("game_over"));


    AddFontResourceEx(_T(R"(res\assets\font\IPix.ttf)"), FR_PRIVATE, NULL); // 导入字体

}




