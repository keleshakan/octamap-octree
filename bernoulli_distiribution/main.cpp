// C++ program to illustrate
// the use of operator()
// in uniform_int_distribution
#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include "quad.h"

using namespace std;

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
