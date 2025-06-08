#pragma once
#include <windows.h>
#include <tchar.h>

class SnakeTypeHandle {
private:
    SnakeTypeHandle();
    ~SnakeTypeHandle();
    static SnakeTypeHandle* _instance;

public:
    static SnakeTypeHandle* instance();

public:
    static const int Max_Snake_Type = 2; // 最大蛇形态数量
    LPCTSTR snake_type_names[Max_Snake_Type] = { _T("普通蛇"), _T("极速蛇") };
    int current_snake_type = 0; // 当前蛇形态



};