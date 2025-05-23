#pragma once

#include "util.h"
#include "timer.h"
#include "atlas.h"
#include "vector2.h"

#include <vector>
#include <functional>

/*
* �����ࣺʹ�ö�ʱ����֡�б���ʵ�ֶ���Ч��������ê�����
*/
class Animation {
private:
    Timer timer;
    Vector2 position;
    bool is_loop = false;
    size_t index_frame = 0; // ��ǰ֡����
    std::vector<IMAGE*> frame_list;
    std::function<void()> on_finished; // ���������ص�����


public:
    Animation() {
        timer.set_one_shot(false); 
        timer.set_on_timeout([&]() {
            index_frame++; // �л�����һ֡
            if(index_frame >= frame_list.size()) {
                if (is_loop) { // �����ѭ������������������
                    index_frame = 0;
                }
                else { // ����ֹͣ����
                    if (on_finished) 
                        on_finished();
                }
            }
        });
    }
    ~Animation() = default;

    // ���ö���
    void reset() { 
        timer.restart();
        index_frame = 0;
    }   
    // ���ö���λ��
    void set_position(const Vector2& pos) { 
        position = pos; 
    }
    // �����Ƿ�ѭ��
    void set_loop(bool is_loop) {
        is_loop = is_loop; 
    }
    // ���ö������
    void set_interval(float interval) {
        timer.set_wait_time(interval);
    }
    // ���ö��������ص�����
    void set_on_finished(std::function<void()> func) {
        on_finished = func;
    }
    // ���֡
    void add_frame(Atlas* atlas) { 
        for(int i = 0; i < atlas->get_size(); i++) {
            IMAGE* img = atlas->get_image(i);
            frame_list.emplace_back(img);
        }
    }
    // ���¶���
    void on_update(float delta) {
        timer.on_update(delta); 
    }
    void on_render() {
        IMAGE* img = frame_list[index_frame]; // ��ȡ��ǰ֡
        Rect rect_dst; // ����Ŀ�����
        rect_dst.x = (int)(position.x - img->getwidth() / 2); // ���Ķ���
        rect_dst.y = (int)(position.y - img->getheight() / 2); // ���Ķ���
        rect_dst.w = img->getwidth();
        rect_dst.h = img->getheight();

        putimage_alpha(img, &rect_dst); // ���Ƶ�ǰ֡
    }
};
