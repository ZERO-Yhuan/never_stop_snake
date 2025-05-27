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
    bool has_played = false; // �Ƿ��Ѿ����Ź�����
    size_t index_frame = 0; // ��ǰ֡����
    std::vector<IMAGE*> frame_list;
    std::function<void()> on_finished; // ���������ص�����


public:
    Animation() {
        timer.set_one_shot(false); 
        timer.set_on_timeout([&]() {

            index_frame++; // �л�����һ֡

            if(index_frame >= frame_list.size()) {
                index_frame = is_loop ? 0 : frame_list.size() - 1; // ѭ����ͣ�����һ֡
                bool can_shot = is_loop || (!is_loop && !has_played);
                if(can_shot && on_finished) {
                    on_finished(); // ���ö��������ص�����
                    has_played = true; // ��Ƕ����Ѿ����Ź�
                }
            }
        });
    }
    ~Animation() = default;

    // ���ö���
    void reset() { 
        timer.restart();
        index_frame = 0;
        has_played = false; // ���ò���״̬
    }   
    // ���ö���λ��
    void set_position(const Vector2& pos) { 
        position = pos; 
    }

    const Vector2& get_position() const { 
        return position; 
    }

    // �����Ƿ�ѭ��
    void set_loop(bool is_loop) {
        this->is_loop = is_loop; 
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
    void add_frame(IMAGE* img) {
        frame_list.emplace_back(img);
        
    }
    // ���¶���
    void on_update(float delta) {
        timer.on_update(delta); 
    }

    // ������Ⱦ����(������ԭ��С��Ⱦ)
    void on_render() { 
        IMAGE* img = frame_list[index_frame]; // ��ȡ��ǰ֡
        Rect rect_dst; // ����Ŀ�����
        rect_dst.x = (int)(position.x - img->getwidth() / 2); // ���Ķ���
        rect_dst.y = (int)(position.y - img->getheight() / 2); // ���Ķ���
        rect_dst.w = img->getwidth();
        rect_dst.h = img->getheight();

        putimage_alpha(img, &rect_dst); // ���Ƶ�ǰ֡
    }
    // ������Ⱦ����(������������Ⱦ)
    void on_render(const Rect& rect_dst) {
        IMAGE* img = frame_list[index_frame]; // ��ȡ��ǰ֡
        putimage_alpha(img, &rect_dst); // ���Ƶ�ǰ֡
    }
};
