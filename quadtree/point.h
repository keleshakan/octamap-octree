//
// Created by hakan on 26/10/2020.
//

#ifndef QUADTREE_POINT_H
#define QUADTREE_POINT_H

#include "vec.h"

class Point{
    Vec3 position;
public:
    Point(){};
    Point(const Vec3& position):position(position){}
    inline const Vec3& getPosition() const { return position; }
    inline void setPosition(const Vec3& p) { position = p; }
};

#endif //QUADTREE_POINT_H
