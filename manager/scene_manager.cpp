#include "scene_manager.h"

SceneManager* SceneManager::_instance = nullptr;

SceneManager* SceneManager::instance() {
    if (_instance == nullptr) {
        _instance = new SceneManager();
    }
    return _instance;
}

SceneManager::SceneManager() = default;
SceneManager::~SceneManager() = default;


void SceneManager::set_current_scene(Scene* scene) {
    current_scene = scene;
    current_scene->on_enter();
}

void SceneManager::switch_scene_to(SceneType type, int state) {
    current_scene->on_exit();
    switch (type) {
        case SceneType::Start:
            current_scene = start_scene;
            break;
        case SceneType::Menu:
            current_scene = menu_scene;
            break;
        case SceneType::Game:
            current_scene = game_scene;
            switch (state) {
            case 0:
                game_scene->set_scene_state(0);
                break;
            case 1:
                game_scene->set_scene_state(1);
                break;

            }
            break;
        case SceneType::DoublePlayerGame:
            current_scene = double_player_game_scene;
            break;
        case SceneType::Settings:
            current_scene = settings_scene;
            break;
    }

    current_scene->on_enter();
}

void SceneManager::on_update(float delta) {
    current_scene->on_update(delta);
}

void SceneManager::on_render(const Camera& camera) {
    current_scene->on_render(camera);
}

void SceneManager::on_input(const ExMessage& msg) {
    current_scene->on_input(msg);
}

