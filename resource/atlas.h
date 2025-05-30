#pragma once

#include <vector>
#include <graphics.h>


// 图集类
class Atlas{
private:
    // 使用 vector 数组储存IMAGE对象
    std::vector<IMAGE> img_list;

public:
    Atlas() = default;
    ~Atlas() = default;

    // 图片加载函数
    void load(LPCTSTR path_template, int num) {
        img_list.clear();
        img_list.resize(num);
        
        TCHAR path_file[256];
        for (int i = 0; i < num; i++) {
            _stprintf_s(path_file, path_template, i);
            loadimage(&img_list[i], path_file);
        }
    }

    void clear() {
        img_list.clear();
    }

    int get_size() const {
        return (int)img_list.size();
    }

    IMAGE* get_image(int index) {
        if (index < 0 || index >= img_list.size()) {
            return nullptr;
        }
        return &img_list[index];
    }

    void add_image(const IMAGE& img) {
        img_list.push_back(img);
    }

};