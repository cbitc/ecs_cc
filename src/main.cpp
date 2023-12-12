#include<iostream>
#include"../include/world.hpp"
#include<string>

using namespace ECS;
using std::cout, std::cin;

struct Position final
{
    float x,y;
};

struct HP final
{
    float hp;
};


struct Time final
{
    float time;
};


void worldLog(const World& world) {
    printf("\n");
    printf("entitys:\n"),world.entitysLog();
    printf("componentsMap:\n"),world.componentsMapLog();
    printf("\n");
}

int main() {
    World world;
    Registry registry(world);
    registry.createResource<Time>(100.f);
    registry.createResource<std::string>("33");
    Time* time = registry.getResource<Time>();
    std::string* name = registry.getResource<std::string>();
    cout << *name;
    bool st = registry.hasResource<Time>();

    worldLog(world);
    return 0;
}