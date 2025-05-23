#pragma once

#include "util.h"
#include "timer.h"
#include "atlas.h"
#include "vector2.h"

#include <vector>
#include <functional>

/*
* 动画类：使用定时器和帧列表来实现动画效果，动画锚点居中
*/
class Animation {
private:
    Timer timer;
    Vector2 position;
    bool is_loop = false;
    size_t index_frame = 0; // 当前帧索引
    std::vector<IMAGE*> frame_list;
    std::function<void()> on_finished; // 动画结束回调函数


public:
    Animation() {
        timer.set_one_shot(false); 
        timer.set_on_timeout([&]() {
            index_frame++; // 切换到下一帧
            if(index_frame >= frame_list.size()) {
                if (is_loop) { // 如果是循环动画，则重置索引
                    index_frame = 0;
                }
                else { // 否则停止动画
                    if (on_finished) 
                        on_finished();
                }
            }
        });
    }
    ~Animation() = default;

    // 重置动画
    void reset() { 
        timer.restart();
        index_frame = 0;
    }   
    // 设置动画位置
    void set_position(const Vector2& pos) { 
        position = pos; 
    }
    // 设置是否循环
    void set_loop(bool is_loop) {
        is_loop = is_loop; 
    }
    // 设置动画间隔
    void set_interval(float interval) {
        timer.set_wait_time(interval);
    }
    // 设置动画结束回调函数
    void set_on_finished(std::function<void()> func) {
        on_finished = func;
    }
    // 添加帧
    void add_frame(Atlas* atlas) { 
        for(int i = 0; i < atlas->get_size(); i++) {
            IMAGE* img = atlas->get_image(i);
            frame_list.emplace_back(img);
        }
    }
    // 更新动画
    void on_update(float delta) {
        timer.on_update(delta); 
    }
    void on_render() {
        IMAGE* img = frame_list[index_frame]; // 获取当前帧
        Rect rect_dst; // 计算目标矩形
        rect_dst.x = (int)(position.x - img->getwidth() / 2); // 中心对齐
        rect_dst.y = (int)(position.y - img->getheight() / 2); // 中心对齐
        rect_dst.w = img->getwidth();
        rect_dst.h = img->getheight();

        putimage_alpha(img, &rect_dst); // 绘制当前帧
    }
};
