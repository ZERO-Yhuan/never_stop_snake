#include <chrono>
#include <thread>
#include <graphics.h>

#include "util.h"
#include "resources_manager.h"

using namespace std::chrono;

// ���Ʊ���
static void draw_background() { 
    static IMAGE* img_baground = ResourcesManager::instance()->find_image("background"); // ��ȡ����ͼƬ
    static Rect rect_dst = {
        (getwidth() - img_baground->getwidth()) / 2,
        (getheight() - img_baground->getheight()) / 2,
        img_baground->getwidth(),
        img_baground->getheight()
    }; // ���㱳��ͼƬ����λ�úʹ�С
    putimage_alpha(img_baground, &rect_dst); // ���Ʊ���ͼƬ
}

int main() {
    HWND hwnd = initgraph(1920, 1080, EW_SHOWCONSOLE); // ��ʼ��ͼ�δ��ڣ����ô��ڴ�СΪ1920x1080
    SetWindowText(hwnd, _T("Never STOP Snake")); // ���ô��ڱ���


    try {
        ResourcesManager::instance()->load(); // ������Դ
    }catch(const LPCTSTR& err_id) {
        TCHAR err_msg[512]; // ����һ���ַ��������ڴ洢������Ϣ
        _stprintf_s(err_msg, _T("�޷�����:%s"), err_id); // ��ʽ��������Ϣ
        MessageBox(hwnd, err_msg, _T("Error"), MB_OK | MB_ICONERROR); // ����������ʾ��
        return -1; // ���ش������
    }

    const nanoseconds frame_duration(1000000000 / 144); // 144 FPS
    steady_clock::time_point last_tick = steady_clock::now(); 

    ExMessage msg; // ��Ϣ�ṹ�壬���ڴ洢��Ϣ��Ϣ
    bool isRunning = true; // ��Ϸ���б�־

    //������������ģʽ����˫����ĺ�̨���ƽ׶Σ�
    //������ͼ�����ݴ����ڴ滺��������������ʾ����Ļ��
    BeginBatchDraw();

    while (isRunning)
    {
        while (peekmessage(&msg))
        {
            // ������Ϣ
        }

        steady_clock::time_point frame_start = steady_clock::now(); // ��ǰ֡��ʼʱ��
        duration<float> delta = duration<float>(frame_start - last_tick); // ������һ֡����ǰ֡��ʱ���

        // ������Ϸ����

        setbkcolor(RGB(0, 0, 0, )); // ���ñ�����ɫΪ��ɫ
        cleardevice(); // �����Ļ

        // ������Ϸ��ͼ
        draw_background(); // ���Ʊ���

        FlushBatchDraw(); // ���ڴ滺�����еĻ�ͼ���һ�����������Ļ��

        

        last_tick = frame_start; // ������һ֡ʱ��
        // ���㵱ǰ֡ʣ��ʱ��
        nanoseconds sleep_duration = frame_duration - duration_cast<nanoseconds>(delta); 
        if(sleep_duration > nanoseconds(0)) {
            std::this_thread::sleep_for(sleep_duration); // ���ߣ�����֡��
        }
    }

    // ������������ģʽ���ͷ������Դ��
    EndBatchDraw();

    return 0;
}