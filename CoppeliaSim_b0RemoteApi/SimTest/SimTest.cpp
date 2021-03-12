// SimTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>
#include <thread>

#include "b0RemoteApi.h"

bool doNextStep = true;
bool runInSynchronousMode = true;
int objHand;
bool flag = true;

using namespace std;

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

    if (objPose[2] < .1)
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

int main()
{
    thread(chrono::milliseconds(20000));
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
