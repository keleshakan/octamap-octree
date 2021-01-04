//
// Created by hakan on 26/10/2020.
//

#ifndef QUADTREE_QUADTREE_H
#define QUADTREE_QUADTREE_H

#include "Rectangle.h"
#include "vec.h"

class QuadTree{
    Vec3 vect;
public:
    Rectangle rectangle;
    QuadTree(Rectangle *rectangle);
};

#endif //QUADTREE_QUADTREE_H
