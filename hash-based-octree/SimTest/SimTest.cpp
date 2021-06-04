// SimTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <bitset>
#include <vector>
#include <iomanip>

#include "Octree.h"
#include "Stopwatch.h"

#include "b0RemoteApi.h"
#include <Bits.h>
#include <random>


bool doNextStep = true;
bool runInSynchronousMode = true;
int objHand;
int objHand_Octree;
float _vecData[3] = {};
std::string wordofStream;

bool flag = true;
static bool secondFlag = true;
static int number = 0;
static int control = 0;
static int numberofPoints = 0;

using namespace std;
using namespace ocspace;

// Used for testing
std::vector<Vec3> points;
Octree* octree;
OctreePoint* octreePoints;
Vec3 qmin, qmax; 

float rand11() // Random number between [-1,1]
{
    //std::cout << -1.f + (2.f * rand()) * (1.f / RAND_MAX) << std::endl;
    //return 256.0f * rand() / (RAND_MAX + 1.0f);
    return -1.f + (2.f * rand()) * (1.f / RAND_MAX);
} 

Vec3 randVec3() // Random vector with components in the range [-1,1]
{
    return Vec3(rand11(), rand11(), rand11());
}

// Determine if 'point' is within the bounding box [bmin, bmax]
bool naivePointInBox(const Vec3& point, const Vec3& bmin, const Vec3& bmax) {
    return
        point.x >= bmin.x &&
        point.y >= bmin.y &&
        point.z >= bmin.z &&
        point.x <= bmax.x &&
        point.y <= bmax.y &&
        point.z <= bmax.z;
}

void init() {
    // Create a new Octree centered at the origin
    // with physical dimension 2x2x2
    /*octree = new Octree(Vec3(0, 0, 0), Vec3(1, 1, 1));

    // Create a bunch of random points
    const int nPoints = 1 * 1000 * 1000;
    for (int i = 0; i < nPoints; ++i) {
        points.push_back(randVec3());
    }
    std::cout << "Created " << points.size()  << "points " <<std::endl;
    fflush(stdout);

    // Insert the points into the octree
    octreePoints = new OctreePoint[nPoints];
    for (int i = 0; i < nPoints; ++i) {
        octreePoints[i].setPosition(points[i]);
        octree->insert(octreePoints + i);
    }
    printf("Inserted points to octree\n"); 
    fflush(stdout);*/

    // Create a very small query box. The smaller this box is
    // the less work the octree will need to do. This may seem
    // like it is exaggerating the benefits, but often, we only
    // need to know very nearby objects.*/
    qmin = Vec3(-.5, -.5, -.5);
    qmax = Vec3(.5, .5, .5);

    // Remember: In the case where the query is relatively close
    // to the size of the whole octree space, the octree will
    // actually be a good bit slower than brute forcing every point!
}

// Query using brute-force
void testNaive() {
    double start = stopwatch();

    std::vector<int> results;
    for (int i = 0; i < points.size(); ++i) {
        if (naivePointInBox(points[i], qmin, qmax)) {
            results.push_back(i);
        }
    }

    double T = stopwatch() - start;
    std::cout << "testNaive found " << results.size() << " points in" << T << "sec.\n" ;
}

// Query using Octree
void testOctree() {
    double start = stopwatch();

    std::vector<OctreePoint*> results;
    octree->getPointsInsideBox(qmin, qmax, results);

    double T = stopwatch() - start;
    std::cout << "testNaive found " << results.size() << " points in" << T << "sec.\n";
}

b0RemoteApi* cl = NULL;
b0RemoteApi client("b0RemoteApi_V-REP", "b0RemoteApi");

void simulationStepDone_CB(std::vector<msgpack::object>* msg)
{
    float simTime = 0.0;
    std::map<std::string, msgpack::object> data = msg->at(1).as<std::map<std::string, msgpack::object>>();
    std::map<std::string, msgpack::object>::iterator it = data.find("simulationTime");
    if (it != data.end())
        simTime = it->second.as<float>();
    
    std::vector<msgpack::object>* reply = client.simxGetObjectPosition(objHand, -1, client.simxServiceCall());

    std::vector<float> objPose;
    b0RemoteApi::readFloatArray(reply, objPose, 1);

    if (objPose[2] < .5)
    {
        flag = false;
    }
    doNextStep = true;
}

void stepSimulation()
{
    if (runInSynchronousMode)
    {
        while (!doNextStep)
            cl->simxSpinOnce();
        doNextStep = false;
        cl->simxSynchronousTrigger();
    }
    else
        cl->simxSpinOnce();
}

void getvectorsData(std::string str) {
    istringstream ss(str);
    int i = 0;

    while (ss >> wordofStream)
    {
        _vecData[i++] = std::stof(wordofStream);
    }
    Vec3 _vector;
    
    _vector.x = _vecData[0];
    _vector.y = _vecData[1];
    _vector.z = _vecData[2];

    points.push_back(_vector);
}

//Create a Vector that contains random float x,y,z value
std::vector<Vec3> create_random_point() {

    std::mt19937 eng;  // a core engine class
    std::normal_distribution<float> _distXval(0,1.f);
    std::normal_distribution<float> _distYval(0,1.f);

    std::vector<Vec3> _points;
    Vec3 _vector;

    for (int i = 0; i < 10000; ++i) {
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        
        _vector.x = _distXval(eng);
        _vector.y = _distYval(eng);
        _vector.z = r;

        _points.push_back(_vector);
    }

    return  _points;
}

void write_file(std::vector<Vec3>& pt) {
    std::ofstream output_file("C://Program Files/CoppeliaRobotics/CoppeliaSimEdu/points.txt");
    for (auto& i : pt) {
        output_file << i.x << " " << i.y << " " << i.z <<std::endl;
    }

    output_file.close();
}

int main()
{
    /*std::vector<Vec3> _fileVector = create_random_point();

    //save generated points into the file.
    write_file(_fileVector);

    //Saving points to the file, then wait some time for the next step.
    std::this_thread::sleep_for(std::chrono::seconds(2));*/

    cl = &client;

    std::vector<msgpack::object>* reply = client.simxGetObjectHandle("Cuboid", client.simxServiceCall());
    objHand = b0RemoteApi::readInt(reply, 1);

    if (runInSynchronousMode)
        client.simxSynchronous(true);

    client.simxGetSimulationStepDone(client.simxDefaultSubscriber(simulationStepDone_CB));
    client.simxStartSimulation(client.simxDefaultPublisher());

    while (flag)
        stepSimulation();

    client.simxStopSimulation(client.simxDefaultPublisher());

    std::cout << "Simulation Stopped!" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(3));

    ifstream _inputFile;
    _inputFile.open("C:/Program Files/CoppeliaRobotics/CoppeliaSimEdu/octreePoints.txt",ios::in);

    int _numberfPoints = 0;

    if (!_inputFile.is_open()) {
        std::cout << " Failed to open" << endl;
    }
    else {
        std::cout << "Opened OK" << endl;
        std::string str;

        while (!_inputFile.eof()) {
            getline(_inputFile, str);
            _numberfPoints++;
        }
    }
    std::cout << "Number of points in file : " << _numberfPoints << std::endl;

    _inputFile.close();

    _inputFile.open("C:/Program Files/CoppeliaRobotics/CoppeliaSimEdu/octreePoints.txt", ios::in);

    //octreePoints = new OctreePoint[_numberfPoints-1];
    //octree = new Octree(Vec3(0, 0, 0), Vec3(1, 1, 1));

    if (!_inputFile.is_open()) {
        std::cout << " Failed to open" << endl;
    }
    else {
        std::cout << "Opened OK" << endl;
        std::string str;

        while (!_inputFile.eof()) {
            getline(_inputFile, str);
            getvectorsData(str);
        }
    }
    _inputFile.close();

    //init();
    int len =(int) _numberfPoints;

    octree = new Octree(Vec3(0, 0, 0), Vec3(2.5, 2.5, 2.5));

    OctreePoint *_octreePoints;
    //_octreePoints = (OctreePoint*)malloc( len * sizeof(OctreePoint));

    //Vec3 _ctrVec;
    
    std::ofstream output_file("C://Program Files/CoppeliaRobotics/CoppeliaSimEdu/hashing.txt");

    octreePoints = new OctreePoint[len];
    for (int i = 0; i < len-1; ++i) {
        //stackoverflow
        octreePoints[i].setPosition(points.at(i));
        //_ctrVec = octreePoints[i].getPosition();
        ocspace::p = "1";
        octree->insert(octreePoints + i);
        if (!ocspace::p._Equal("1")) {
            output_file << "Point is: " << points.at(i).x << " " << points.at(i).y << " " << points.at(i).z  << " hash is: " << ocspace::p << std::endl;
        }
        

        //cause violation rule exception.
        //free(&octreePoints[i]);
    }

    output_file.close();
    //std::cout << points.size << 

    /*Vec3 _vector;
    for (int i = _numberfPoints-1; i > 0; i /= 10) {
        _vector = octreePoints[i].getPosition();
        //std::cout << "x: " << _vector.x << ", y: " << _vector.y << ", z: " << _vector.z << std::endl;
    }*/
    Vec3 newVec(2,- 2,0);
    int result = octree->getOctantContainingPoint(newVec);

    std::cout << "Octant is: " << result << std::endl;

    octree->isLeafNode();
    qmin = Vec3(-2.5, -2.5, -2.5);
    qmax = Vec3(0, 0, 0);

    testNaive();
    testOctree();
        
    //std::vector<OctreePoint*> ocTREE;

    //octree->getPointsInsideBox(Vec3(.5,.5,.5), Vec3(1, 1, 1), ocTREE);

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
