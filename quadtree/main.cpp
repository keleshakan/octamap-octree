#include <iostream>
#include "point.h"
#include "Rectangle.h"
#include "quadtree.h"

int main() {
    Rectangle *rectangle = new Rectangle(200,200,200,200);
    QuadTree *quadTree = new QuadTree(rectangle);

    std::cout << quadTree->rectangle << std::endl;
    return 0;
}
