#include <chrono>
#include <thread>
#include <graphics.h>

#include "./tool/util.h"
#include "./tool/settings.h"

#include "./scene/scene.h"
#include "./scene/custom/start_scene.h"
#include "./scene/custom/game_scene.h"
#include "./scene/custom/menu_scene.h"
#include "./scene/custom/double_player_game_scene.h"
#include "./scene/custom/settings_scene.h"


#include "./manager/resources_manager.h"
#include "./manager/scene_manager.h"
#include "./manager/collision_manager.h"

#include "./tool/camera.h"


using namespace std::chrono;

Scene* start_scene = nullptr;
Scene* menu_scene = nullptr;    
Scene* game_scene = nullptr; 
Scene* double_player_game_scene = nullptr;
Scene* settings_scene = nullptr;

Camera main_camera;

// ���Ʊ���
static void draw_background() { 
    static IMAGE* img_background = ResourcesManager::instance()->find_image("background"); // ��ȡ����ͼƬ

    static Rect rect_dst = {
    0,
    0,
    1920,
    1080
    }; // ���㱳��ͼƬ����λ�úʹ�С
    putimage_alpha(img_background, &rect_dst); // ���Ʊ���ͼƬ
}

void init_scene() {
    start_scene = new StartScene(); // ������ʼ����
    menu_scene = new MenuScene(); // �����˵�����
    game_scene = new GameScene(); // ������Ϸ����
    double_player_game_scene = new DoublePlayerGameScene(); // ����˫����Ϸ����
    settings_scene = new SettingsScene(); // �������ó���
}

int main() {
    // HWND hwnd = initgraph(1080, 720, EW_SHOWCONSOLE); // ��ʼ��ͼ�δ��ڣ����ô��ڴ�СΪ1920x1080, "EW_SHOWCONSOLE":��ʾ����̨
    HWND hwnd = initgraph(1080, 720);
    SetWindowText(hwnd, _T("Never STOP - 2.0")); // ���ô��ڱ���

    try {
        ResourcesManager::instance()->load(); // ������Դ
    }catch(const LPCTSTR& err_id) {
        TCHAR err_msg[512]; // ����һ���ַ��������ڴ洢������Ϣ
        _stprintf_s(err_msg, _T("�޷�����:%s"), err_id); // ��ʽ��������Ϣ
        MessageBox(hwnd, err_msg, _T("Error"), MB_OK | MB_ICONERROR); // ����������ʾ��
        return -1; // ���ش������
    }
    
    Settings::instance()->load(); // ��������

    init_scene(); // ��ʼ������
    SceneManager::instance()->set_current_scene(start_scene); // ���õ�ǰ����Ϊ��ʼ����

    settextstyle(26, 0, _T("IPix"));
    setbkmode(TRANSPARENT); // ��������͸������


    const nanoseconds frame_duration(1000000000 / Settings::instance()->get_fps()); // 144 FPS
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
            SceneManager::instance()->on_input(msg); // ����ǰ��������Ϣ
        }

        steady_clock::time_point frame_start = steady_clock::now(); // ��ǰ֡��ʼʱ��
        duration<float> delta = duration<float>(frame_start - last_tick); // ������һ֡����ǰ֡��ʱ���

        // ������Ϸ����
        main_camera.on_update(delta.count()); // ���������
        SceneManager::instance()->on_update(delta.count()); // ���µ�ǰ����

        setbkcolor(RGB(0, 0, 0 )); // ���ñ�����ɫΪ��ɫ
        cleardevice(); // �����Ļ

        // ������Ϸ��ͼ
        SceneManager::instance()->on_render(main_camera); // ���Ƶ�ǰ����
        if(Settings::instance()->get_is_debug_on()) CollisionManager::instance()->on_debug_render(main_camera); // ������ײ������Ϣ

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