#pragma once

#include "Camera.h"

class Player {
public:
    Player() : camera(defaultSpawnPoint) {};
    ~Player() = default;

    Player(const Player& other) = delete;
    Player& operator=(const Player& other) = delete;

    Player(Player&& other) noexcept = delete;
    Player& operator=(Player&& other) noexcept = delete;

    Camera& getCamera();

private:
    Camera camera;

    static const glm::vec3 defaultSpawnPoint;
};