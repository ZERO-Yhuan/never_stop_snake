#pragma once

#include "util.h"
#include "animation.h"
#include "graphics.h"
#include "resources_manager.h" 

class UI {
protected:
    Rect rect_dst;
    LPCTSTR text;

public:
    UI() = default;
    UI(Rect& rect_dst, LPCTSTR text) : rect_dst(rect_dst), text(text) {}
    UI(int x, int y, int w, int h, LPCTSTR text) : rect_dst({ x, y, w, h }), text(text) {}
    virtual ~UI() = default;

    virtual void on_update(float delta) {
    }

    virtual void on_render() {
        outtextxy_shaded(rect_dst.x + (rect_dst.w - textwidth(text)) / 2, rect_dst.y + (rect_dst.h - textheight(text)) / 2, text);
    }

    virtual void set_rect(Rect& rect) {
        rect_dst = rect;
    }

    virtual void set_size(int width, int height) {
        rect_dst.w = width;
        rect_dst.h = height;
    }

    virtual void set_position(int x, int y) {
        rect_dst.x = x;
        rect_dst.y = y;
    }   

    virtual void set_text(const LPCTSTR button_text) {
        text = button_text;
    }
};