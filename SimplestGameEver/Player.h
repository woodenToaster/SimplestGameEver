#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <irrKlang.h>

#include "Entity.h"

class Player : public Entity {
public:
    GLuint lives;
    bool alive;
    GLfloat velocity;
    glm::vec2 startingPos;
    irrklang::ISoundEngine* soundEngine;

    Player(glm::vec2 playerPos, glm::vec2 playerSize, irrklang::ISoundEngine* soundEngine);
    void moveUp(GLfloat dt, std::vector<Entity> walls);
    void moveDown(GLfloat dt, GLuint screenHeight, std::vector<Entity> walls);
    void moveLeft(GLfloat dt, std::vector<Entity> walls);
    void moveRight(GLfloat dt, GLuint screenWidth, std::vector<Entity> walls);
    void onCollision(Entity& other);

private:

};

#endif