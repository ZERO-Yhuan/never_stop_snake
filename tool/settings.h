#pragma once

#include <string>
#include <fstream>
#include <iostream>

#include "util.h"

class Settings {
private:
    int volume = 30;
    int fps = 144;
    bool is_debug = false;
    const std::string settings_file = "settings.txt";

    static Settings* _instance;
private:
    Settings();
    ~Settings();

public:
    static Settings* instance();

    void load();

    void save();

    int get_setting_volume() const;

    void set_setting_volume(int volume);

    int get_fps() const;

    void set_fps(int fps);

    bool get_is_debug_on() const;

    void set_is_debug_on(bool flag);
};