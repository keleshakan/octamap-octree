#ifndef Vec3_h_
#define Vec3_h_

#include <cmath>

struct Vec3;
Vec3 operator*(float r, const Vec3& v);

struct Vec3 {
    union {
        struct {
            float x,y;
        };
        float D[2];
    };

    Vec3() { }
    Vec3(float _x, float _y)
            :x(_x), y(_y)
    { }

    float& operator[](unsigned int i) {
        return D[i];
    }

    const float& operator[](unsigned int i) const {
        return D[i];
    }

    float maxComponent() const {
        float r = x;
        if(y>r) r = y;
        return r;
    }

    float minComponent() const {
        float r = x;
        if(y<r) r = y;
        return r;
    }

    Vec3 operator+(const Vec3& r) const {
        return Vec3(x+r.x, y+r.y);
    }

    Vec3 operator-(const Vec3& r) const {
        return Vec3(x-r.x, y-r.y);
    }

    Vec3 cmul(const Vec3& r) const {
        return Vec3(x*r.x, y*r.y);
    }

    Vec3 cdiv(const Vec3& r) const {
        return Vec3(x/r.x, y/r.y);
    }

    Vec3 operator*(float r) const {
        return Vec3(x*r,y*r);
    }


    Vec3 operator/(float r) const {
        return Vec3(x/r, y/r);
    }

    Vec3& operator+=(const Vec3& r) {
        x+=r.x;
        y+=r.y;
        return *this;
    }

    Vec3& operator-=(const Vec3& r) {
        x-=r.x;
        y-=r.y;
        return *this;
    }

    Vec3& operator*=(float r) {
        x*=r; y*=r;
        return *this;
    }

    // Inner/dot product
    float operator*(const Vec3& r) const {
        return x*r.x + y*r.y ;
    }

    float norm() const {
        return sqrtf(x*x+y*y);
    }

    float normSquared() const {
        return x*x + y*y ;
    }

    /*Cross product
    Vec3 operator^(const Vec3& r) const {
        return Vec3(
                x * r.y - y * r.x
        );
    }*/

    Vec3 normalized() const {
        return *this / norm();
    }
};

inline Vec3 operator*(float r, const Vec3& v) {
    return Vec3(v.x*r, v.y*r);
}

#endif