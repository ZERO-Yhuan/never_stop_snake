#pragma once

#pragma comment(lib, "WINMM.lib") // 链接 Windows 多媒体库，提供音频播放等多媒体功能
#pragma comment(lib, "MSIMG32.lib") // 链接 MSIMG32 库，提供 AlphaBlend 函数支持

#include <graphics.h>

struct Rect
{
    int x, y;
    int w, h;
};

// 提供支持透明通道的图像绘制函数
/*
* 使用BLENDFUNCTION结构体配置混合模式为 AC_SRC_OVER（源覆盖目标）
* 全局静态变量 blend_func 确保只初始化一次
* 源图像的 Alpha 通道直接用于控制透明度（AC_SRC_ALPHA）
* 通过 GetImageHDC 获取源图像和目标图像的设备上下文
* rect_dst：指向目标矩形区域的指针，包含 x、y 坐标和宽高信息
*/
inline void putimage_alpha(IMAGE* img, const Rect* rect_dst) {
    static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    AlphaBlend(
        GetImageHDC(GetWorkingImage()), rect_dst->x, rect_dst->y, rect_dst->w, rect_dst->h,
        GetImageHDC(img), 0, 0, img->getwidth(), img->getheight(), blend_func
        );
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