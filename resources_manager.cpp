#include "resources_manager.h"

struct ImageResInfo {
    std::string id; // ͼƬID
    LPCTSTR path; // ͼƬ·��
};
struct AtlasResInfo {
    std::string id; // ͼƬID
    LPCTSTR path; // ͼƬ·��
    int num_frames; // ͼƬ֡��
};

// ͼƬ��Դ
static const std::vector<ImageResInfo> image_info_list = {
    { "background", _T(R"(res\background.png)")}, // ����ͼƬ
    // { "player", _T(R"(res\snake\snake.png)")}, // ���ͼƬ
};

// ͼ����Դ
static const std::vector<AtlasResInfo> atlas_info_list = {
    /*
    { "food", _T(R"(res\food\food.png)"), 1}, // ʳ��ͼƬ
    { "enemy", _T(R"(res\enemy\enemy.png)"), 1}, // ����ͼƬ
    { "bullet", _T(R"(res\bullet\bullet.png)"), 1}, // �ӵ�ͼƬ
    */
};

// ���ͼƬ�Ƿ���Ч
static inline bool check_image_valid(IMAGE* image) {
    return GetImageBuffer(image);
}

// ��ʼ����������
ResourcesManager* ResourcesManager::_instance = nullptr; 

// ��ȡ��������
ResourcesManager* ResourcesManager::instance() { 
    if (_instance == nullptr) {
        _instance = new ResourcesManager();
    }
    return _instance;
}

ResourcesManager::ResourcesManager() = default;
ResourcesManager::~ResourcesManager() = default;

// ����ͼƬ��Դ
IMAGE* ResourcesManager::find_image(const std::string& id) const { 
    const auto& it = image_pool.find(id); // ����ͼƬ��Դ
    if (it == image_pool.end()) // ���û���ҵ�
        return nullptr; // ���ؿ�ָ��
    return it->second; // ����ͼƬ��Դ
}
// ����ͼ����Դ
Atlas* ResourcesManager::find_atlas(const std::string& id) const {
    const auto& it = atlas_pool.find(id);
    if (it == atlas_pool.end())
        return nullptr;
    return it->second;
}

// ������Դ
void ResourcesManager::load() {
    // ����ͼƬ��Դ
    for (const auto& info : image_info_list) { 
        IMAGE* image = new IMAGE;
        loadimage(image, info.path);

        if (!check_image_valid(image)) // ���ͼƬ�Ƿ���Ч
            throw info.path;

        image_pool[info.id] = image;
    }

    // ����ͼ����Դ
    for (const auto& info : atlas_info_list) { 
        Atlas* atlas = new Atlas;
        atlas->load(info.path, info.num_frames);

        for (int i = 0; i < atlas->get_size(); i++) { // ���ÿһ֡ͼƬ�Ƿ���Ч
            IMAGE* image = atlas->get_image(i);
            if(!check_image_valid(image))
                throw info.path;
        }

        atlas_pool[info.id] = atlas;
    }

    /*
    load_audio(); // ������Ƶ��Դ
    */
}




