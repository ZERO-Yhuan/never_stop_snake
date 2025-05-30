#pragma once

#pragma comment(lib, "WINMM.lib") // ���� Windows ��ý��⣬�ṩ��Ƶ���ŵȶ�ý�幦��
#pragma comment(lib, "MSIMG32.lib") // ���� MSIMG32 �⣬�ṩ AlphaBlend ����֧��

#include <graphics.h>
#include "camera.h"

struct Rect
{
    int x, y;
    int w, h;
    Rect() : x(0), y(0), w(0), h(0) {} // Ĭ�Ϲ��캯������ʼ�����ε�����ʹ�СΪ 0


    Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {} // ���캯������ʼ�����ε�����ʹ�С
    Rect(float x, float y, float w, float h) 
        : x(static_cast<int>(x)), y(static_cast<int>(y)), 
        w(static_cast<int>(w)), h(static_cast<int>(h)) {} // ֧�ָ����������Ĺ��캯��
    Rect(float x, float y, int w, int h)
        : x(static_cast<int>(x)), y(static_cast<int>(y)),
        w(w), h(h) {
    } // ֧�ָ����������Ĺ��캯��
};

// �ṩ֧��͸��ͨ����ͼ����ƺ���
/*
* ʹ��BLENDFUNCTION�ṹ�����û��ģʽΪ AC_SRC_OVER��Դ����Ŀ�꣩
* ȫ�־�̬���� blend_func ȷ��ֻ��ʼ��һ��
* Դͼ��� Alpha ͨ��ֱ�����ڿ���͸���ȣ�AC_SRC_ALPHA��
* ͨ�� GetImageHDC ��ȡԴͼ���Ŀ��ͼ����豸������
* rect_dst��ָ��Ŀ����������ָ�룬���� x��y ����Ϳ����Ϣ
*/
inline void putimage_alpha(const Camera& camera,IMAGE* img, const Rect* rect_dst) {
    static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    const Vector2& pos_camera = camera.get_position();
    AlphaBlend(
        GetImageHDC(GetWorkingImage()), (int)(rect_dst->x - pos_camera.x), (int)(rect_dst->y - pos_camera.y), rect_dst->w, rect_dst->h,
        GetImageHDC(img), 0, 0, img->getwidth(), img->getheight(), blend_func
        );
}

// ����������� putimage_alpha �����ڻ���UI
inline void putimage_alpha(IMAGE* img, const Rect* rect_dst) { 
    static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    AlphaBlend(
        GetImageHDC(GetWorkingImage()), rect_dst->x, rect_dst->y , rect_dst->w, rect_dst->h,
        GetImageHDC(img), 0, 0, img->getwidth(), img->getheight(), blend_func
    );
}

inline void outtextxy_shaded(int x, int y, LPCTSTR str) {
    settextcolor(RGB(45, 45, 45)); // �����ı���ɫΪ���ɫ
    outtextxy(x + 2, y + 2, str); // ������Ӱ�ı�
    settextcolor(RGB(255, 255, 255)); // �����ı���ɫΪ��ɫ
    outtextxy(x, y, str); // ���������ı�
}

inline void drawtext_shaded(LPCTSTR str, Rect* rect_dst, UINT uFormat) {
    RECT rect_text = { rect_dst->x, rect_dst->y, rect_dst->w, rect_dst->h };
    RECT rect_shade = { rect_dst->x + 2, rect_dst->y + 2, rect_dst->w, rect_dst->h };
    settextcolor(RGB(45, 45, 45)); // �����ı���ɫΪ���ɫ
    drawtext(str, &rect_shade, uFormat);
    settextcolor(RGB(255, 255, 255)); // �����ı���ɫΪ��ɫ
    drawtext(str, &rect_text, uFormat);
}

// ��װ��Ƶ������غ�����������Ĳ���ת��Ϊ��Ӧ�� MCI �����ַ���
inline void load_audio(LPCTSTR path, LPCTSTR id) {
    static TCHAR str_cmd[512]; // ���ڴ洢 MCI �����ַ���
    // ��ʽ�������ַ���
    _stprintf_s(str_cmd, _T("open %s alias %s"), path, id);
    mciSendString(str_cmd, NULL, 0, NULL); // ִ�ж�Ӧ�߼�
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
    // ȷ����������Ч��Χ�� (0-100)
    volume = max(0, min(100, volume));
    volume = volume * 10; // ת��Ϊ 0-1000 ��Χ
    _stprintf_s(str_cmd, _T("setaudio %s volume to %d"), id, volume);
    mciSendString(str_cmd, NULL, 0, NULL);
}