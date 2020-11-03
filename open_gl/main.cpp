// Include standard headers
#include "GL/glew.h"
#include "GL/glut.h"
#include <cstdlib>
#include <fstream>
#include <vector>
#include <random>
#include <iostream>
#include <map>

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

    static float number_x;
    static float number_y;

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
    //void traverse_and_draw(Quad*,float);
    //void colorPick(float,Quad*,float*,int);
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

void assign_data(float data_x,float data_y){

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(data_x/2,.0);
    glVertex2f(data_x/2,data_y);
    glEnd();
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(data_x,data_y/2);
    glVertex2f(.0,data_y/2);
    glEnd();
}

void draw(int region,float data){
    if (region == 1){
        // x:positive
        // y:positive
        assign_data(data,data);
    }else if (region == 2){
        // x:negative
        // y:positive
        assign_data(-data,data);
    }else if(region == 3){
        // x:negative
        // y:negative
        assign_data(-data,-data);
    }else if (region == 4){
        // x:positive
        // y:negative
        assign_data(data,-data);
    }
}

// Find a node in a quadtree
Node* Quad::search(Point p)
{
    if (!inBoundary(p))
        return nullptr;

    if (n != nullptr)
        return n;

    //    topleft   topright   botleft  botright
    //x:     -         +          -        +
    //y:     +         +          -        -

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
            //draw(4);
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

std::vector<Point> create_random_point(){
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<Point> point;

    Point point1[70];

    std::bernoulli_distribution bernoulli_x(0.5);
    std::bernoulli_distribution bernoulli_y(0.5);

    std::map<int,int> hist;

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

    Point point2[20];

    uniform_int_distribution<int> distribution_x(0,2048);
    uniform_int_distribution<int> distribution_y(0,2048);

    for (auto &vec : point2) {
        vec.x = distribution_x(gen);
        vec.y = distribution_y(gen);
        point.push_back(vec);
    }

    Point point3[10];

    std::poisson_distribution<int> poissonDistribution_x(1024);
    std::poisson_distribution<int> poissonDistribution_y(1024);

    for(auto & n : point3) {
        n.x = poissonDistribution_x(gen);
        n.y = poissonDistribution_y(gen);

        point.push_back(n);
    }

    return  point;
}

void write_file(Node *node,int range) {
    std::ofstream output_file("point.txt");
    for (int i = 0; i < range; ++i) {
        output_file << node[i].pos.x << " " << node[i].pos.y << std::endl;
    }
}

void display(){
    std::vector<Point> point = create_random_point();

    for (auto vec : point) {
        std::cout << vec.x << " " << vec.y << std::endl;
    }

    Node node[100];
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
    write_file(node,100);

    if(center.search(point.at(99)) != nullptr){
        std::cout << "the point has been found" << std::endl;
    }else{
        std::cout << "the point has not been found" << std::endl;
    }
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(-0.8,0.8);
    glVertex2f(-0.8,-0.8);
    glEnd();
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(-0.8,-0.8);
    glVertex2f(0.8,-0.8);
    glEnd();
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(0.8,0.8);
    glVertex2f(0.8,-0.8);
    glEnd();
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(0.8,0.8);
    glVertex2f(-0.8,0.8);
    glEnd();
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(.0,0.8);
    glVertex2f(.0,-0.8);
    glEnd();
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(-.8,.0);
    glVertex2f(.8,.0);
    glEnd();

    float data = .8;

    for (int i = 0; i < 4; ++i) {
        for (int j = 1; j < 5; ++j) {
            draw(j,data);
        }
        data /= 2;
    }


    glLineWidth(1.0f);
    glBegin(GL_LINES);

    for(float cy=-0.8;cy<0.8;cy+=0.4){
        glColor3f(0.0, 0.0, 0.0);
        glVertex2f(-0.82,cy);
        glVertex2f(-0.78,cy);
    }
    glEnd();

    glLineWidth(1.0f);
    glBegin(GL_LINES);

    for(float cx=-0.8;cx<0.8;cx+=0.4){
        glColor3f(0.0, 0.0, 0.0);
        glVertex2f(cx,-0.82);
        glVertex2f(cx,-0.78);
    }
    glEnd();

    glPointSize(4.0f);
    glBegin(GL_POINTS);

    double pos_x;
    double pos_y;

    for (auto & i : node) {
        pos_x = (double) i.pos.x/128*.1;
        pos_y = (double) i.pos.y/128*.1;
        glColor3f(0.0, 0.0, 1.0);
        glVertex2f(-.8+pos_x,-.8+pos_y);
    }

    glEnd();

    double x_pos_1 = -.81;
    double y_pos_1 = -.85;

    const char *a[5] = {"0","512","1024","1536","2048"};

    for (auto & i : a) {
        glColor3f (0.0, 0.0, 1.0);
        glRasterPos2f(x_pos_1, y_pos_1); //define position on the screen
        x_pos_1 += .4;

        while (*i){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i++);
        }
    }

    glFlush();
    glutSwapBuffers();

}

int main(int argc, char* argv[]){
    //  Initialize GLUT and process user parameters
    glutInit(&argc,argv);

    //  Request double buffered true color window with Z-buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Create window
    glutCreateWindow("Coordinate System");

    //  Enable Z-buffer depth test
    glEnable(GL_DEPTH_TEST);

    // Callback functions
    glutDisplayFunc(display);

    //  Pass control to GLUT for events
    glutMainLoop();

    return 0;
}