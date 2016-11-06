#include "Game.h"
#include "ResourceManager.h"
#include "EntityRenderer.h"
#include "Player.h"

EntityRenderer *renderer;
Player *player;

Game::Game(GLuint width, GLuint height) :
    state(GAME_ACTIVE),
    keys(),
    doorOpen(true),
    width(width),
    height(height) {

}

Game::~Game() {
    delete renderer;
    delete player;
}

void Game::init() {
    ResourceManager::loadShader("SimpleVertexShader.vert", "SimpleFragmentShader.frag", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);
    // ResourceManager::getShader("sprite").use().setInteger("image", 0);
    ResourceManager::getShader("sprite").use().setMatrix4("projection", projection);

    // ResourceManager::loadTexture("textures/awesomeface.png", GL_TRUE, "face");
    renderer = new EntityRenderer(ResourceManager::getShader("sprite"));

    glm::vec2 playerPos = glm::vec2(0.0f, 400.0f);
    glm::vec2 playerSize = glm::vec2(40, 40);
    player = new Player(playerPos, playerSize);

    levelNames.push_back("level1.txt");
    levelNames.push_back("level2.txt");
    levelNames.push_back("level3.txt");
    levelNames.push_back("level4.txt");
    levelNames.push_back("level5.txt");
    levelNames.push_back("level6.txt");
    levelNames.push_back("level7.txt");
    levelNames.push_back("level8.txt");
    levelNames.push_back("level9.txt");
    levelNames.push_back("level10.txt");

    for(std::string& levelName : levelNames) {
        GameLevel levelToLoad;
        levelToLoad.init(levelName);
        levels.push_back(levelToLoad);
    }

    level = 0;
    enemiesActive = false;
}

void Game::updateEnemies(GLfloat dt) {
    for(auto& enemy : levels[level].enemies) {
        enemy.update(dt, width, height, levels[level].arena);
        enemy.rotation += 5.0f;
    }
}

void Game::checkCollisions() {

    // Check player collisions with enemies
    for(Enemy& enemy : levels[level].enemies) {
        if(player->checkCollision(enemy)) {
            player->onCollision(enemy);
            openDoor();
            closeExit();
        }
    }

    // Check enemy collisions
    for(Enemy& enemy : levels[level].enemies) {

        // With other enemies
        for(Enemy& otherEnemy : levels[level].enemies) {
            if(&otherEnemy == &enemy) {
                continue;
            }
            if(enemy.checkCollision(otherEnemy)) {
                enemy.onCollision(otherEnemy);
            }
        }

        // With arena walls
        for(Entity& wall : levels[level].arena) {
            if(enemy.checkCollision(wall)) {
                enemy.onCollision(wall);
                return;
            }
        }
    }
}

void Game::closeDoor() {
    glm::vec2 doorPos = levels[level].doorPosition;
    glm::vec2 doorSize = levels[level].doorSize;
    glm::vec3 doorColor = levels[level].doorColor;
    Entity door(doorPos, doorSize, doorColor);
    levels[level].arena.push_back(door);
    doorOpen = false;
    enemiesActive = true;

    if(false) {
        for(Enemy& enemy : levels[level].enemies) {
            enemy.direction = enemy.direction == Enemy::HORIZONTAL ? Enemy::VERTICAL : Enemy::HORIZONTAL;
            enemy.movingRight = !enemy.movingRight;
            enemy.movingUp = !enemy.movingUp;
        }
    }
}

void Game::openDoor() {
    levels[level].arena.pop_back();
    doorOpen = true;
}

void Game::closeExit() {
    Entity exit(levels[level].exitPosition, levels[level].exitSize, glm::vec3(1.0f, 1.0f, 1.0f));
    levels[level].arena.insert(levels[level].arena.begin(), exit);
}

void Game::openExit() {
    levels[level].arena.erase(levels[level].arena.begin());
}

void Game::checkDoor() {
    if(player->position.x > levels[level].doorPosition.x + levels[level].doorSize.x) {
        closeDoor();
        openExit();
    }
}

void Game::checkExit() {
    glm::vec2 exitPos = levels[level].exitPosition;
    glm::vec2 exitSize = levels[level].exitSize;
    bool xIsExited = player->position.x > exitPos.x + exitSize.x;
    bool yIsExitedTop = player->position.y < exitPos.y + exitSize.y;
    bool yIsExitedBottom = player->position.y + player->size.y > exitPos.y;
    bool yIsExited = yIsExitedTop && yIsExitedBottom;
    bool playerExited = xIsExited && yIsExited;

    if(playerExited) {
        doNextLevel();
    }
}

void Game::doNextLevel() {
    level++;
    enemiesActive = false;
    doorOpen = true;
    player->position = glm::vec2(0.0f, 400.0f);
}

void Game::update(GLfloat dt) {
    if(state == GAME_ACTIVE) {
        if(enemiesActive)
            updateEnemies(dt);
        checkCollisions();
        if(doorOpen) {
            checkDoor();
        }
        checkExit();
        if(player->lives == 0) {
            state = GAME_MENU;
        }
    }
}

void Game::processInput(GLfloat dt) {
    if(state == GAME_ACTIVE)
    {
        if(keys[GLFW_KEY_LEFT]) {
            player->moveLeft(dt, levels[level].arena);
        }
        if(keys[GLFW_KEY_RIGHT]) {
            player->moveRight(dt, width, levels[level].arena);
        }
        if(keys[GLFW_KEY_UP]) {
            player->moveUp(dt, levels[level].arena);
        }
        if(keys[GLFW_KEY_DOWN]) {
            player->moveDown(dt, height, levels[level].arena);
        }
    }
}

void Game::render() {
    if(state == GAME_ACTIVE) {
        player->draw(*renderer);
        levels[level].draw(*renderer);
    }
}