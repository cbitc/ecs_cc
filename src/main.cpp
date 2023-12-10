#include<iostream>
#include"../include/world.hpp"
#include<string>

using namespace ECS;
using std::cout, std::cin;

struct A
{
    int age;
    bool isMan;
    std::string name;
};

struct B
{
    int id;
    float score;
};

void worldLog(const World& world) {
    printf("entitys:\n"),world.entitysLog();
    printf("componentsMap:\n"),world.componentsMapLog();
}

int main() {
    World world;
    Registry registry(world);
    Entity e1 = registry.create();
    registry.emplace<A>(e1, 18, true, "hcc");
    registry.emplace<B>(e1, 1, 100.0f);

    Entity e2 = registry.create();
    registry.emplace<B>(e2, 2, 98.0f);

    worldLog(world);

    registry.erase<A,B,int>(e1);
    worldLog(world);
    return 0;
}