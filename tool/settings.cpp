#include "settings.h"

Settings::Settings() = default;

Settings::~Settings() = default;

Settings* Settings::_instance = nullptr;

Settings* Settings::instance() {
    if (_instance == nullptr) {
        _instance = new Settings();
    }
    return _instance;
}

void Settings::load() {
    std::wifstream file(settings_file);
    if (file.is_open()) {
        std::wstring line;
        while (std::getline(file, line)) {
            if (line.find(L"volume=") == 0) {
                volume = std::stoi(line.substr(7));
            }
            else if (line.find(L"frame_rate=") == 0) {
                fps = std::stoi(line.substr(11));
            }
        }
        file.close();
    }
    else {
        std::cerr << "ÅäÖÃÎÄ¼þ¼ÓÔØÊ§°Ü£¬Ê¹ÓÃÄ¬ÈÏÅäÖÃ¡£" << std::endl;
    }

    set_volume(_T("start_menu_bgm"), volume);
}

void Settings::save() {
    std::wofstream file(settings_file);
    if (file.is_open()) {
        file << L"volume=" << volume << L"\n";
        file << L"frame_rate=" << fps << L"\n";
        file.close();
    }
    else {
        std::cerr << "ÅäÖÃÎÄ¼þ¼ÓÔØÊ§°Ü£¬±£´æÊ§°Ü¡£" << std::endl;
    }
}

int Settings::get_setting_volume() const {
    return volume;
}

void Settings::set_setting_volume(int volume) {
    this->volume = volume;
}

int Settings::get_fps() const {
    return fps;
}

void Settings::set_fps(int fps) {
    this->fps = fps;
}

bool Settings::get_is_debug_on() const {
    return this->is_debug;
}

void Settings::set_is_debug_on(bool flag) {
    this->is_debug = flag;
}