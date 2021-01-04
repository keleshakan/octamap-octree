// C++ Implementation of Quad Tree
#include <iostream>
#include <cmath>
using namespace std;

// Used to hold details of a point
struct Point
{
    int x;
    int y;
    Point(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    Point()
    {
        x = 0;
        y = 0;
    }
};

// The objects that we want stored in the quadtree
struct Node
{
    Point pos;
    int data;
    Node(Point _pos, int _data)
    {
        pos = _pos;
        data = _data;
    }
    Node()
    {
        data = 0;
    }
};

// The main quadtree class
class Quad
{
    // Hold details of the boundary of this node
    Point topLeft;
    Point botRight;

    // Contains details of node
    Node *n;

    // Children of this tree
    Quad *topLeftTree;
    Quad *topRightTree;
    Quad *botLeftTree;
    Quad *botRightTree;

public:
    Quad()
    {
        topLeft = Point(0, 0);
        botRight = Point(1200, 1200);
        n = nullptr;
        topLeftTree = nullptr;
        topRightTree = nullptr;
        botLeftTree = nullptr;
        botRightTree = nullptr;
    }
    Quad(Point topL, Point botR)
    {
        n = nullptr;
        topLeftTree = nullptr;
        topRightTree = nullptr;
        botLeftTree = nullptr;
        botRightTree = nullptr;
        topLeft = topL;
        botRight = botR;
    }
    void insert(Node*);
    Node* search(Point);
    [[nodiscard]] bool inBoundary(Point) const;
};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-misleading-indentation"

// Insert a node into the quadtree
void Quad::insert(Node *node)
{
    if (node == nullptr)
        return;

    // Current quad cannot contain it
    if (!inBoundary(node->pos))
        return;

    if (abs(topLeft.x - botRight.x) <= 1 &&
        abs(topLeft.y - botRight.y) <= 1)
    {
        if (n == nullptr)
            n = node;
        return;
    }

    if ((topLeft.x + botRight.x) / 2 >= node->pos.x)
    {
        //topLeftTree
        if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
        {
            if (topLeftTree == nullptr)
                topLeftTree = new Quad(
                        Point(topLeft.x, topLeft.y),
                        Point((topLeft.x + botRight.x) / 2,
                              (topLeft.y + botRight.y) / 2));
                topLeftTree->insert(node);
        }

            //botLeftTree
        else
        {
            if (botLeftTree == nullptr)
                botLeftTree = new Quad(
                        Point(topLeft.x,
                              (topLeft.y + botRight.y) / 2),
                        Point((topLeft.x + botRight.x) / 2,
                              botRight.y));
            botLeftTree->insert(node);
        }
    }
    else
    {
        //topRightTree
        if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
        {
            if (topRightTree == nullptr)
                topRightTree = new Quad(
                        Point((topLeft.x + botRight.x) / 2,
                              topLeft.y),
                        Point(botRight.x,
                              (topLeft.y + botRight.y) / 2));
            topRightTree->insert(node);
        }

            //botRightTree
        else
        {
            if (botRightTree == nullptr)
                botRightTree = new Quad(
                        Point((topLeft.x + botRight.x) / 2,
                              (topLeft.y + botRight.y) / 2),
                        Point(botRight.x, botRight.y));
            botRightTree->insert(node);
        }
    }
}
#pragma clang diagnostic pop

// Find a node in a quadtree
Node* Quad::search(Point p)
{
    if (!inBoundary(p))
        return nullptr;

    if (n != nullptr)
        return n;

    if ((topLeft.x + botRight.x) / 2 >= p.x)
    {
        // Indicates topLeftTree
        if ((topLeft.y + botRight.y) / 2 >= p.y)
        {
            if (topLeftTree == nullptr){
                return nullptr;
            }
            return topLeftTree->search(p);
        }

            // Indicates botLeftTree
        else
        {
            if (botLeftTree == nullptr){
                return nullptr;
            }
            return botLeftTree->search(p);
        }
    }
    else
    {
        // Indicates topRightTree
        if ((topLeft.y + botRight.y) / 2 >= p.y)
        {
            if (topRightTree == nullptr){
                return nullptr;
            }
            return topRightTree->search(p);
        }

            // Indicates botRightTree
        else
        {
            if (botRightTree == nullptr){
                return nullptr;
            }
            return botRightTree->search(p);
        }
    }
}

// Check if current quadtree contains the point
bool Quad::inBoundary(Point p) const
{
    return (p.x >= topLeft.x &&
            p.x <= botRight.x &&
            p.y >= topLeft.y &&
            p.y <= botRight.y);
}

// Driver program
int main()
{
    Quad center(Point(0, 0), Point(1010, 1010));
    Node a(Point(1010, 1010), 1);
    Node b(Point(980, 1010), 2);
    Node c(Point(850, 880), 3);
    Node d(Point(860, 810), 4);
    Node e(Point(100, 150), 5);
    Node f(Point(310, 400), 6);
    Node g(Point(400, 800), 7);
    Node h(Point(600, 620), 8);
    Node j(Point(980, 1010), 9);

    center.insert(&a);
    center.insert(&b);
    center.insert(&c);
    center.insert(&d);
    center.insert(&e);
    center.insert(&f);
    center.insert(&g);
    center.insert(&h);
    center.insert(&j);


    if(center.search(Point(1010, 1010)) != nullptr){
        std::cout << "find the point!" << std::endl;
    }else{
        std::cout << "the point does not exist!" << std::endl;
    }
    return 0;
}
