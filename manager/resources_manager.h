#pragma once

#include "../resource/atlas.h"
#include "../tool/util.h"

#include <unordered_map>
#include <string>

// �������ģʽ
class ResourcesManager {
private:
    ResourcesManager();
    ~ResourcesManager();

    // ͼƬ��ת����
    // -----------------------------------------------------

private:
    static ResourcesManager* _instance; // ��������

    std::unordered_map<std::string, Atlas*> atlas_pool; // �����
    std::unordered_map<std::string, IMAGE*> image_pool; // ͼƬ��


public:
    static ResourcesManager* instance();

    void load();
    Atlas* find_atlas(const std::string& id) const; // ��������
    IMAGE* find_image(const std::string& id) const; // ����ͼƬ

};