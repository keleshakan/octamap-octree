//
// Created by hakan on 26/10/2020.
//

#ifndef QUADTREE_DATA_SET_NODECLASS_H
#define QUADTREE_DATA_SET_NODECLASS_H

class NodeClass{
public:
    NodeClass();
    static NodeClass KD;
    static NodeClass KB;
    static NodeClass GD;
    static NodeClass GB;

    int X;
    int Y;
    int CenterX;
    int CenterY;
    int Range;

    static void insertNode(NodeClass param,int X,int Y);
    static void AddQuadTree();
};

#endif //QUADTREE_DATA_SET_NODECLASS_H
