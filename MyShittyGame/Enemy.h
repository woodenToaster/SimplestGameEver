#ifndef ENEMY_H
#define ENEMY_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Entity.h"

class Enemy : public Entity {
public:
    enum Direction {HORIZONTAL, VERTICAL};
    Direction direction;
    bool movingRight;
    bool movingUp;
    GLfloat velocity;

    Enemy(glm::vec2 enemyPosition, glm::vec2 enemySize, glm::vec3 color, Direction dir);
    void update(GLfloat dt, GLuint width, GLuint height);
    void onCollision(Entity& other);

private:

};

#endif