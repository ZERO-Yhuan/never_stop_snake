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
    static const int Max_Snake_Type = 2; // �������̬����
    LPCTSTR snake_type_names[Max_Snake_Type] = { _T("��ͨ��"), _T("������") };
    int current_snake_type = 0; // ��ǰ����̬



};