#include <iostream>
#include <cstdlib>
#include <vector>
#include "nodeclass.h"

void insertNode(NodeClass param, int X, int Y)
{
    //kuzeybatı
    NodeClass *ptr = &param;
    try
    {
        if (X <= param.X && Y >= param.Y)
        {
            if (ptr != NULL)
            {
                //insertNode(param.KB, X, Y);
                insertNode(param.KB, param.KB.X, param.KB.Y);
                std::cout << "some data has been added" << std::endl;
            }
            else
            {
                param.KB.X = X;
                param.KB.Y = Y;
                param.KB.Range = param.Range / 2;
                param.KB.CenterX = param.CenterX - param.KB.Range;
                param.KB.CenterY = param.CenterY + param.KB.Range;
                std::cout << "some data has not been added" << std::endl;
            }

        }
            //güneybatı
        else if (X <= param.X && Y < param.Y)
        {
            if (ptr != NULL)
            {
                //insertNode(param.GB, X, Y);
                insertNode(param.GB, param.GB.X, param.GB.Y);
            }
            else
            {
                param.GB.X = X;
                param.GB.Y = Y;
                param.GB.Range = param.Range / 2;
                param.GB.CenterX = param.CenterX - param.GB.Range;
                param.GB.CenterY = param.CenterY - param.GB.Range;
            }
        }
            //kuzeydogu
        else if (X > param.X && Y > param.Y)
        {
            if (ptr != NULL)
            {
                //insertNode(param.KD, X, Y);
                insertNode(param.KD, param.KD.X, param.KD.Y);
            }
            else
            {
                param.KD.X = X;
                param.KD.Y = Y;
                param.KD.Range = param.Range / 2;
                param.KD.CenterX = param.CenterX + param.KD.Range;
                param.KD.CenterY = param.CenterY + param.KD.Range;
            }
        }
            //güneydogu
        else if (X > param.X && Y <= param.Y)
        {
            if (ptr != nullptr)
            {
                //insertNode(param.GD, X, Y);
                insertNode(param.GD, param.GD.X, param.GD.Y);
            }
            else
            {
                param.GD.X = X;
                param.GD.Y = Y;
                param.GD.Range = param.Range / 2;
                param.GD.CenterX = param.CenterX + param.GD.Range;
                param.GD.CenterY = param.CenterY - param.GD.Range;
            }
        }
    }
    catch (std::exception ex)
    {
        //
    }
}

void AddQuadTree()
{
    int totalx = 1200;
    int totaly = 1200;
    std::vector<NodeClass> ListCloud;
    NodeClass model1 ; model1.X = 1010; model1.Y = 1010; ListCloud.push_back(model1);
    NodeClass model2 ; model1.X = 980; model1.Y = 1010; ListCloud.push_back(model1);
    NodeClass model3 ; model1.X = 850; model1.Y = 880; ListCloud.push_back(model1);
    NodeClass model4 ; model1.X = 860; model1.Y = 810; ListCloud.push_back(model1);
    NodeClass model5 ; model1.X = 100; model1.Y = 150; ListCloud.push_back(model1);
    NodeClass model6 ; model1.X = 310; model1.Y = 400; ListCloud.push_back(model1);
    NodeClass model7 ; model1.X = 400; model1.Y = 800; ListCloud.push_back(model1);
    NodeClass model9 ; model1.X = 600; model1.Y = 620; ListCloud.push_back(model1);
    NodeClass model10 ; model1.X = 980; model1.Y = 1010; ListCloud.push_back(model1);

    NodeClass root ;

    NodeClass *ptr = &root;

    for (auto item: ListCloud) {
        if (ptr == nullptr) {
            root.CenterX = totalx / 2;
            root.CenterY = totaly / 2;
            root.Range = totalx / 2;
        } else {
            insertNode(root, item.X, item.Y);
        }
    }
}

