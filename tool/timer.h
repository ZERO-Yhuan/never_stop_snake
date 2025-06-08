#pragma once

#include <functional>

class Timer {
private:
    float pass_time = 0; // 经过时间
    float wait_time = 0; // 需要等待时间
    bool paused = false; // 暂停
    bool shotted = false; // 定时器已经触发
    bool one_shot = false; // 只触发一次
    std::function<void()> on_timeout; // 目标函数

public:
    Timer(){}
    ~Timer(){}

    // 定时器重启
    void restart() {
        pass_time = 0;
        shotted = false;
    }
    // 设置需要等待时间
    void set_wait_time(float val) {
        wait_time = val;
    }
    // 设置只触发一次
    void set_one_shot(bool flag) {
        one_shot = flag;
    }
    // 设置目标函数
    void set_on_timeout(std::function<void()> on_timeout) {
        this->on_timeout = on_timeout;
    }
    // 暂停
    void pause() {
        paused = true;
    }
    // 继续
    void resume() {
        paused = false;
    }

    // 更新函数
    /*
    * 定时器更新逻辑：传入帧与帧之间的时间间隔，累计经过时间到【pass_time】，检测【pass_time】是否到达等待时间
    * 如果到达则判断是否能触发，如果能触发并且目标函数不为空则触发函数，最后将【pass_time】还原
    */
    void on_update(float delta) {
        // 更新游戏状态，如果游戏暂停则直接返回
        if (paused) return;

        // 累加时间，检查是否达到等待时间
        pass_time += delta;
        if (pass_time >= wait_time) {
            // 判断是否可以射击，以及是否已经射击过
            bool can_shot = (!one_shot || (one_shot && !shotted));
            shotted = true;
            // 如果可以射击且有超时回调，则调用回调函数
            if (can_shot && on_timeout) {
                on_timeout();
            }
            // 重置经过的时间
            pass_time -= wait_time;
        }
    }

};