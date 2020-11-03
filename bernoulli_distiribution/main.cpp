// C++ program to illustrate
// the use of operator()
// in uniform_int_distribution
#include <iostream>
#include <random>
#include <vector>
#include <fstream>

using namespace std;

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

std::vector<Point> create_bernoulli_random_point(){
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<Point> point;

    Point point1[10];

    std::bernoulli_distribution bernoulli_x(0.15);
    std::bernoulli_distribution bernoulli_y(0.25);

    static double total_x = 0.0;
    static double total_y = 0.0;

    for (auto & i : point1) {
        for (int j=0;j<2048;++j) {
            total_x += bernoulli_x(gen)*2048;
            total_y += bernoulli_y(gen)*2048;
        }
        i.x = (int)total_x / 2048;
        i.y = (int)total_y / 2048;
        point.push_back(i);
        total_x = 0.0;
        total_y = 0.0;
    }

    Point point2[10];

    uniform_int_distribution<int> distribution_x(0,2048);
    uniform_int_distribution<int> distribution_y(0,2048);

    for (auto &vec : point2) {
        vec.x = distribution_x(gen);
        vec.y = distribution_y(gen);
        point.push_back(vec);
    }

    /*Point point3[10];

    std::poisson_distribution<int> poissonDistribution_x(7);
    std::poisson_distribution<int> poissonDistribution_y(7);

    static double total_x_poison = 0.0;
    static double total_y_poison = 0.0;

    for (auto & i : point3) {
        for (int j=0;j<2048;++j) {
            total_x += poissonDistribution_x(gen)*2048;
            total_y += poissonDistribution_y(gen)*2048;
        }
        i.x = (int)total_x_poison / 2048;
        i.y = (int)total_y_poison / 2048;
        point.push_back(i);
        total_x_poison = 0.0;
        total_y_poison = 0.0;
    }*/

    return  point;
}

void write_file(Node *node,int range) {
    std::ofstream output_file("point.txt");
    for (int i = 0; i < range; ++i) {
        output_file << node[i].pos.x << " " << node[i].pos.y << std::endl;
    }
}

// Driver program
int main()
{
    std::vector<Point> point = create_bernoulli_random_point();

    for (auto vec : point) {
        std::cout << vec.x << " " << vec.y << std::endl;
    }

    Node node[20];
    int count = 0;

    Quad center(Point(0, 0), Point(2048, 2048));

    for (auto vec : point) {
        node[count].pos = vec;
        node[count].data = count+1;
        count ++;
    }

    for (auto nodes: node) {
        center.insert(&nodes);
    }
    write_file(node,20);

    if(center.search(point.at(19)) != nullptr){
        std::cout << "find the point!" << std::endl;
    }else{
        std::cout << "the point does not exist!" << std::endl;
    }

    return 0;
}
