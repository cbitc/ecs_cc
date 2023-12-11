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





void worldLog(const World& world) {
    printf("\n");
    printf("entitys:\n"),world.entitysLog();
    printf("componentsMap:\n"),world.componentsMapLog();
    printf("\n");
}

int main() {
    World world;
    Registry registry(world);

    Entity entity = registry.create();
    registry.emplace<std::string>(entity,"hcc");
    registry.emplace<int>(entity,18);

    auto [name,age] = registry.get_or_emplace<std::string,double>(entity);
    registry.replace<std::string>(entity,"hzz");

    if (name) {
        cout << *name;
    }
    if (age) {
        cout << *age;
    } else {
        cout << "null";
    }
    cout << '\n';

    worldLog(world);
    return 0;
}