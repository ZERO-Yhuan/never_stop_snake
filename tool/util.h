#pragma once

#pragma comment(lib, "WINMM.lib") // 链接 Windows 多媒体库，提供音频播放等多媒体功能
#pragma comment(lib, "MSIMG32.lib") // 链接 MSIMG32 库，提供 AlphaBlend 函数支持

#include <graphics.h>
#include "camera.h"

struct Rect
{
    int x, y;
    int w, h;
    Rect() : x(0), y(0), w(0), h(0) {} // 默认构造函数，初始化矩形的坐标和大小为 0


    Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {} // 构造函数，初始化矩形的坐标和大小
    Rect(float x, float y, float w, float h) 
        : x(static_cast<int>(x)), y(static_cast<int>(y)), 
        w(static_cast<int>(w)), h(static_cast<int>(h)) {} // 支持浮点数参数的构造函数
    Rect(float x, float y, int w, int h)
        : x(static_cast<int>(x)), y(static_cast<int>(y)),
        w(w), h(h) {
    } // 支持浮点数参数的构造函数
};

// 提供支持透明通道的图像绘制函数
/*
* 使用BLENDFUNCTION结构体配置混合模式为 AC_SRC_OVER（源覆盖目标）
* 全局静态变量 blend_func 确保只初始化一次
* 源图像的 Alpha 通道直接用于控制透明度（AC_SRC_ALPHA）
* 通过 GetImageHDC 获取源图像和目标图像的设备上下文
* rect_dst：指向目标矩形区域的指针，包含 x、y 坐标和宽高信息
*/
inline void putimage_alpha(const Camera& camera,IMAGE* img, const Rect* rect_dst) {
    static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    const Vector2& pos_camera = camera.get_position();
    AlphaBlend(
        GetImageHDC(GetWorkingImage()), (int)(rect_dst->x - pos_camera.x), (int)(rect_dst->y - pos_camera.y), rect_dst->w, rect_dst->h,
        GetImageHDC(img), 0, 0, img->getwidth(), img->getheight(), blend_func
        );
}

// 无视摄像机的 putimage_alpha 可用于绘制UI
inline void putimage_alpha(IMAGE* img, const Rect* rect_dst) { 
    static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    AlphaBlend(
        GetImageHDC(GetWorkingImage()), rect_dst->x, rect_dst->y , rect_dst->w, rect_dst->h,
        GetImageHDC(img), 0, 0, img->getwidth(), img->getheight(), blend_func
    );
}

inline void outtextxy_shaded(int x, int y, LPCTSTR str) {
    settextcolor(RGB(45, 45, 45)); // 设置文本颜色为深灰色
    outtextxy(x + 2, y + 2, str); // 绘制阴影文本
    settextcolor(RGB(255, 255, 255)); // 设置文本颜色为白色
    outtextxy(x, y, str); // 绘制正常文本
}

inline void drawtext_shaded(LPCTSTR str, Rect* rect_dst, UINT uFormat) {
    RECT rect_text = { rect_dst->x, rect_dst->y, rect_dst->w, rect_dst->h };
    RECT rect_shade = { rect_dst->x + 2, rect_dst->y + 2, rect_dst->w, rect_dst->h };
    settextcolor(RGB(45, 45, 45)); // 设置文本颜色为深灰色
    drawtext(str, &rect_shade, uFormat);
    settextcolor(RGB(255, 255, 255)); // 设置文本颜色为白色
    drawtext(str, &rect_text, uFormat);
}

// 封装音频播放相关函数，将传入的参数转换为对应的 MCI 命令字符串
inline void load_audio(LPCTSTR path, LPCTSTR id) {
    static TCHAR str_cmd[512]; // 用于存储 MCI 命令字符串
    // 格式化命令字符串
    _stprintf_s(str_cmd, _T("open %s alias %s"), path, id);
    mciSendString(str_cmd, NULL, 0, NULL); // 执行对应逻辑
}

inline void play_audio(LPCTSTR id, bool is_loop = false) {
    static TCHAR str_cmd[512]; 
    _stprintf_s(str_cmd, _T("play %s %s from 0"), id, is_loop ? _T("repeat") : _T(""));
    mciSendString(str_cmd, NULL, 0, NULL);
}

inline void stop_audio(LPCTSTR id) {
    static TCHAR str_cmd[512]; 
    _stprintf_s(str_cmd, _T("stop %s"), id);
    mciSendString(str_cmd, NULL, 0, NULL);
}

inline void set_volume(LPCTSTR id, int volume) {
    static TCHAR str_cmd[512];
    // 确保音量在有效范围内 (0-100)
    volume = max(0, min(100, volume));
    volume = volume * 10; // 转换为 0-1000 范围
    _stprintf_s(str_cmd, _T("setaudio %s volume to %d"), id, volume);
    mciSendString(str_cmd, NULL, 0, NULL);
}