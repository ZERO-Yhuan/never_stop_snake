#pragma once

#pragma comment(lib, "WINMM.lib") // ���� Windows ��ý��⣬�ṩ��Ƶ���ŵȶ�ý�幦��
#pragma comment(lib, "MSIMG32.lib") // ���� MSIMG32 �⣬�ṩ AlphaBlend ����֧��

#include <graphics.h>

struct Rect
{
    int x, y;
    int w, h;
};

// �ṩ֧��͸��ͨ����ͼ����ƺ���
/*
* ʹ��BLENDFUNCTION�ṹ�����û��ģʽΪ AC_SRC_OVER��Դ����Ŀ�꣩
* ȫ�־�̬���� blend_func ȷ��ֻ��ʼ��һ��
* Դͼ��� Alpha ͨ��ֱ�����ڿ���͸���ȣ�AC_SRC_ALPHA��
* ͨ�� GetImageHDC ��ȡԴͼ���Ŀ��ͼ����豸������
* rect_dst��ָ��Ŀ����������ָ�룬���� x��y ����Ϳ����Ϣ
*/
inline void putimage_alpha(IMAGE* img, const Rect* rect_dst) {
    static BLENDFUNCTION blend_func = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
    AlphaBlend(
        GetImageHDC(GetWorkingImage()), rect_dst->x, rect_dst->y, rect_dst->w, rect_dst->h,
        GetImageHDC(img), 0, 0, img->getwidth(), img->getheight(), blend_func
        );
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