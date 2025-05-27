#pragma once

#include <cmath>

// 二维向量类
class Vector2 {
public:
    float x;
    float y;

public:
    Vector2(float x = 0, float y = 0) : x(x), y(y) {}
    ~Vector2() {}

    // 运算符重载
    Vector2 operator+ (const Vector2& v) const {
        return Vector2(x + v.x, y + v.y);
    }

    void operator+= (const Vector2& v) {
        x += v.x, y += v.y;
    }

    Vector2 operator- (const Vector2& v) const {
        return Vector2(x - v.x, y - v.y);
    }

    void operator-= (const Vector2& v) {
        x -= v.x, y -= v.y;
    }

    float operator* (const Vector2& v) const {
        return x * v.x + y * v.y;
    }

    Vector2 operator* (float val) const {
        return Vector2(x * val, y * val);
    }

    void operator*= (float val) {
        x *= val;
        y *= val;
    }

    float length() const{
        return sqrt(x * x + y * y);
    }

    // 单位化
    Vector2 normalize() {
        float len = length();

        if (len == 0) {
            return Vector2(0, 0);
        }

        return Vector2(x / len, y / len);
    }

};