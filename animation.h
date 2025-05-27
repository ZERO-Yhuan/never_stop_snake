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
    bool has_played = false; // 是否已经播放过动画
    size_t index_frame = 0; // 当前帧索引
    std::vector<IMAGE*> frame_list;
    std::function<void()> on_finished; // 动画结束回调函数


public:
    Animation() {
        timer.set_one_shot(false); 
        timer.set_on_timeout([&]() {

            index_frame++; // 切换到下一帧

            if(index_frame >= frame_list.size()) {
                index_frame = is_loop ? 0 : frame_list.size() - 1; // 循环或停在最后一帧
                bool can_shot = is_loop || (!is_loop && !has_played);
                if(can_shot && on_finished) {
                    on_finished(); // 调用动画结束回调函数
                    has_played = true; // 标记动画已经播放过
                }
            }
        });
    }
    ~Animation() = default;

    // 重置动画
    void reset() { 
        timer.restart();
        index_frame = 0;
        has_played = false; // 重置播放状态
    }   
    // 设置动画位置
    void set_position(const Vector2& pos) { 
        position = pos; 
    }

    const Vector2& get_position() const { 
        return position; 
    }

    // 设置是否循环
    void set_loop(bool is_loop) {
        this->is_loop = is_loop; 
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
    void add_frame(IMAGE* img) {
        frame_list.emplace_back(img);
        
    }
    // 更新动画
    void on_update(float delta) {
        timer.on_update(delta); 
    }

    // 动画渲染函数(按动画原大小渲染)
    void on_render() { 
        IMAGE* img = frame_list[index_frame]; // 获取当前帧
        Rect rect_dst; // 计算目标矩形
        rect_dst.x = (int)(position.x - img->getwidth() / 2); // 中心对齐
        rect_dst.y = (int)(position.y - img->getheight() / 2); // 中心对齐
        rect_dst.w = img->getwidth();
        rect_dst.h = img->getheight();

        putimage_alpha(img, &rect_dst); // 绘制当前帧
    }
    // 动画渲染函数(按参数矩形渲染)
    void on_render(const Rect& rect_dst) {
        IMAGE* img = frame_list[index_frame]; // 获取当前帧
        putimage_alpha(img, &rect_dst); // 绘制当前帧
    }
};
