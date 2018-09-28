#include <iostream>
#include "entityx/quick.h"


struct DebugSystem : public entityx::System<DebugSystem>
{
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override
    {
    }
};


struct MovementSystem : public entityx::System<DebugSystem>
{
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override
    {
    }
};


struct CollisionSystem : public entityx::System<DebugSystem>
{
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override
    {
    }
};


class Level : public entityx::EntityX
{
public:
    Level()
    {
        systems.add<DebugSystem>();
        systems.add<MovementSystem>();
        systems.add<CollisionSystem>();
        systems.configure();
    }


    void update(float dt)
    {
        systems.update<DebugSystem>(dt);
        systems.update<MovementSystem>(dt);
        systems.update<CollisionSystem>(dt);
    }
};


int main()
{
    Level level;
    while (true) {
        level.update(0.1f);
    }
    return 0;
}
