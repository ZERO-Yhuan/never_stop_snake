#include "util.h"
#pragma once

/*
* 实现对 << 的重载，待完成

struct MyText {
    int x, y;
    int size;
    std::string text;

    MyText(int x, int y, int size = 26, std::string text) : x(x), y(y), size(size), text(text) {}
};

struct MyInt {
    int x, y;
    int size;
    int num;
    MyInt(int x, int y, int size = 26, int num) : x(x), y(y), size(size), num(num) {}
};


class TextHandle {
private:
    static TextHandle* _instance;

    int default_size = 26;
public:
    static TextHandle* instance() {
        if (!_instance)
            _instance = new TextHandle(26);
        return _instance;
    };

    TextHandle(int default_size) : default_size(default_size) {}
    ~TextHandle() {}


public:

    void set_default_size(int size) {
        this->default_size = size;
    }

    friend TextHandle& operator << (TextHandle& text_handle, const MyText& text);
    friend TextHandle& operator << (TextHandle& text_handle, const MyInt& num);
};

TextHandle& operator << (TextHandle& text_handle, const MyText& text) {
    TCHAR score_text[32];
    _stprintf_s(score_text, text, score);
    settextstyle(text.size, 0, _T("IPix"));
    outtextxy_shaded(text.x + 40, text.y + 30, _T(text.text.c_str()));
    outtextxy_shaded(text.x + 40, text.y + 90, text);
    settextstyle(26, 0, _T("IPix"));
}

TextHandle& operator << (TextHandle& text_handle, const MyInt& num) {
    LPCTSTR int_str = _T("%d");
    TCHAR text[32];
    _stprintf_s(text, int_str, num.num);
    settextstyle(num.size, 0, _T("IPix"));
    outtextxy_shaded(num.x + 40, num.y + 30, text);
    outtextxy_shaded(num.x + 40, num.y + 90, text);
    settextstyle(26, 0, _T("IPix"));
}
*/