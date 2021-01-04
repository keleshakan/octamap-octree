//
// Created by hakan on 26/10/2020.
//

#ifndef QUADTREE_RECTANGLE_H
#define QUADTREE_RECTANGLE_H

class Rectangle{
    double x,y,width,height;
public:
    Rectangle();
    Rectangle(double x,double y,double width,double height);
};

Rectangle::Rectangle(double x, double y, double width, double height) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

#endif //QUADTREE_RECTANGLE_H
