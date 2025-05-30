#pragma once

#include <functional>

class Timer {
private:
    float pass_time = 0; // ����ʱ��
    float wait_time = 0; // ��Ҫ�ȴ�ʱ��
    bool paused = false; // ��ͣ
    bool shotted = false; // ��ʱ���Ѿ�����
    bool one_shot = false; // ֻ����һ��
    std::function<void()> on_timeout; // Ŀ�꺯��

public:
    Timer(){}
    ~Timer(){}

    // ��ʱ������
    void restart() {
        pass_time = 0;
        shotted = false;
    }
    // ������Ҫ�ȴ�ʱ��
    void set_wait_time(float val) {
        wait_time = val;
    }
    // ����ֻ����һ��
    void set_one_shot(bool flag) {
        one_shot = flag;
    }
    // ����Ŀ�꺯��
    void set_on_timeout(std::function<void()> on_timeout) {
        this->on_timeout = on_timeout;
    }
    // ��ͣ
    void pause() {
        paused = true;
    }
    // ����
    void resume() {
        paused = false;
    }

    // ���º���
    /*
    * ��ʱ�������߼�������֡��֮֡���ʱ�������ۼƾ���ʱ�䵽��pass_time������⡾pass_time���Ƿ񵽴�ȴ�ʱ��
    * ����������ж��Ƿ��ܴ���������ܴ�������Ŀ�꺯����Ϊ���򴥷���������󽫡�pass_time����ԭ
    */
    void on_update(float delta) {
        if (paused) return;

        pass_time += delta;
        if (pass_time >= wait_time) {
            bool can_shot = (!one_shot || (one_shot && !shotted));
            shotted = true;
            if (can_shot && on_timeout) {
                on_timeout();
            }
            pass_time -= wait_time;
        }

    }
};