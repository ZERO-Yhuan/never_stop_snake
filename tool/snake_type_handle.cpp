#include "snake_type_handle.h"

SnakeTypeHandle* SnakeTypeHandle::_instance = nullptr;

SnakeTypeHandle::SnakeTypeHandle() = default;
SnakeTypeHandle::~SnakeTypeHandle() = default;

SnakeTypeHandle* SnakeTypeHandle::instance() {
    if (_instance == nullptr) {
        _instance = new SnakeTypeHandle();
    }
    return _instance;
}