#include "Player.h"

#include "Camera.h"

const glm::vec3 Player::defaultSpawnPoint = {0.0f, 20.0f, 0.0f};

Camera& Player::getCamera() {
    return this->camera;
}