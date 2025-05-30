#include <chrono>
#include <thread>
#include <graphics.h>

#include "./tool/util.h"

#include "./scene/scene.h"
#include "./scene/custom/start_scene.h"
#include "./scene/custom/game_scene.h"
#include "./scene/custom/menu_scene.h"


#include "./manager/resources_manager.h"
#include "./manager/scene_manager.h"
#include "./manager/collision_manager.h"

#include "./tool/camera.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


using namespace std::chrono;

Scene* start_scene = nullptr;
Scene* menu_scene = nullptr;    
Scene* game_scene = nullptr; 

Camera main_camera;

// 绘制背景
static void draw_background() { 
    static IMAGE* img_baground = ResourcesManager::instance()->find_image("background"); // 获取背景图片

    static Rect rect_dst = {
    0,
    0,
    1920,
    1080
    }; // 计算背景图片绘制位置和大小
    putimage_alpha(img_baground, &rect_dst); // 绘制背景图片
}

void init_scene() {
    start_scene = new StartScene(); // 创建开始场景
    menu_scene = new MenuScene(); // 创建菜单场景
    game_scene = new GameScene(); // 创建游戏场景
}

int main() {
    // HWND hwnd = initgraph(1080, 720, EW_SHOWCONSOLE); // 初始化图形窗口，设置窗口大小为1920x1080, "EW_SHOWCONSOLE":显示控制台
    HWND hwnd = initgraph(1080, 720);
    SetWindowText(hwnd, _T("Never STOP - 1.0")); // 设置窗口标题

    try {
        ResourcesManager::instance()->load(); // 加载资源
    }catch(const LPCTSTR& err_id) {
        TCHAR err_msg[512]; // 定义一个字符数组用于存储错误信息
        _stprintf_s(err_msg, _T("无法加载:%s"), err_id); // 格式化错误信息
        MessageBox(hwnd, err_msg, _T("Error"), MB_OK | MB_ICONERROR); // 弹出错误提示框
        return -1; // 返回错误代码
    }
    

    init_scene(); // 初始化场景
    SceneManager::instance()->set_current_scene(start_scene); // 设置当前场景为开始场景

    settextstyle(26, 0, _T("IPix"));
    setbkmode(TRANSPARENT); // 设置文字透明背景


    const nanoseconds frame_duration(1000000000 / 144); // 144 FPS
    steady_clock::time_point last_tick = steady_clock::now(); 

    ExMessage msg; // 消息结构体，用于存储消息信息
    bool isRunning = true; // 游戏运行标志

    //开启批量绘制模式（即双缓冲的后台绘制阶段）
    //后续绘图操作暂存于内存缓冲区，不立即显示到屏幕。
    BeginBatchDraw();

    while (isRunning)
    {
        while (peekmessage(&msg))
        {
            // 处理消息
            SceneManager::instance()->on_input(msg); // 处理当前场景的消息
        }

        steady_clock::time_point frame_start = steady_clock::now(); // 当前帧开始时间
        duration<float> delta = duration<float>(frame_start - last_tick); // 计算上一帧到当前帧的时间差

        // 处理游戏更新
        main_camera.on_update(delta.count()); // 更新摄像机
        SceneManager::instance()->on_update(delta.count()); // 更新当前场景
        CollisionManager::instance()->process_collide(); // 更新碰撞管理器

        setbkcolor(RGB(0, 0, 0 )); // 设置背景颜色为黑色
        cleardevice(); // 清空屏幕

        // 处理游戏绘图
        SceneManager::instance()->on_render(main_camera); // 绘制当前场景
        // CollisionManager::instance()->on_debug_render(main_camera); // 绘制碰撞调试信息

        FlushBatchDraw(); // 将内存缓冲区中的绘图结果一次性输出到屏幕。

        

        last_tick = frame_start; // 更新上一帧时间
        // 计算当前帧剩余时间
        nanoseconds sleep_duration = frame_duration - duration_cast<nanoseconds>(delta); 
        if(sleep_duration > nanoseconds(0)) {
            std::this_thread::sleep_for(sleep_duration); // 休眠，保持帧率
        }
    }

    // 结束批量绘制模式，释放相关资源。
    EndBatchDraw();
    _CrtDumpMemoryLeaks();
    return 0;
}