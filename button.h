#pragma once

#include <functional>

#include "ui.h"
#include "timer.h"


class Button : public UI {
private:
    Rect normal_rect;
    bool is_selected = false;
    bool is_pressed = false; // 是否被按下
    std::function<void()> on_click = []() {};
    
private:


public:
    Button() = default;
    Button(Rect& rect_dst, LPCTSTR text, std::function<void()> on_click = []() {}) 
        : UI(rect_dst, text), on_click(on_click){
        normal_rect = rect_dst;
    }
    Button(int x, int y, int w, int h, LPCTSTR text, std::function<void()> on_click = []() {}) 
        : UI(x,y,w,h,text), on_click(on_click) {
        normal_rect = rect_dst;
    }
    ~Button() = default;

    void on_update(float delta) override {
        UI::on_update(delta);
    }

    void on_render() override {

        if (is_selected) {
            putimage_alpha(ResourcesManager::instance()->find_image("button_selected"), &rect_dst);
        }
        else {
            putimage_alpha(ResourcesManager::instance()->find_image("button"), &rect_dst);
        }

        UI::on_render();
    }

    void set_rect(Rect& rect) override {
        UI::set_rect(rect);
        normal_rect = rect_dst;
    }

    void set_size(int w, int h) override {
        UI::set_size(w, h);
        normal_rect = rect_dst;
    }

    void set_position(int x, int y) override {
        UI::set_position(x, y);
        normal_rect = rect_dst;
    }

    void set_selected(bool selected) {
        is_selected = selected;
    }

    void set_on_clicked(std::function<void()> on_click) {
        this->on_click = on_click;
    }

    void start_clicked() {
        if (!is_pressed) {
            play_audio(_T("click"));
            rect_dst.x = rect_dst.x + rect_dst.w * 0.1f / 2;
            rect_dst.y = rect_dst.y + rect_dst.h * 0.1f / 2;
            rect_dst.w = rect_dst.w * 0.9f;
            rect_dst.h = rect_dst.h * 0.9f;
            is_pressed = true;
        }
    }

    void end_clicked() {
        if (is_pressed) {
            is_pressed = false;
            rect_dst = normal_rect;
            if (on_click) {
                on_click();
            }
        }

    }

    bool is_clicked() const {
        return is_pressed;
    }
};