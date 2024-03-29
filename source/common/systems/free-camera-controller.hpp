#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"
#include <iostream>
using namespace std;

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeCameraControllerSystem
    {
        Application *app;          // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked
        double margin = 0.2;       // margin between player/camera and other entities.

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        // Check for AABB collision before allowing the camera to move.
        // AABB collision simply checks if the player position is within the boundaries of the other entity.
        // If it is within the boundaries, return true (collision occured), otherwise return false.
        // In the collision check, check if the other entity has a win data member (flag), if yes then change the state to win state.
        bool check_collision(glm::vec3 player, glm::vec3 entity, glm::vec3 scale, double margin = 1, bool win = false)
        {
            if (
                player[0] + margin > entity[0] - scale[0] &&
                player[0] - margin < entity[0] + scale[0] &&
                player[2] + margin > entity[2] - scale[2] &&
                player[2] - margin < entity[2] + scale[2])
            {
                return true;
            }
            return false;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        void update(World *world, float deltaTime)
        {
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CameraComponent *camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            for (auto entity : world->getEntities())
            {
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if (camera && controller)
                    break;
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if (!(camera && controller))
                return;
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity *entity = camera->getOwner();

            // If the left mouse button is pressed, we lock and hide the mouse. This common in First Person Games.
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked)
            {
                app->getMouse().lockMouse(app->getWindow());
                mouse_locked = true;
                // If the left mouse button is released, we unlock and unhide the mouse.
            }
            else if (!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked)
            {
                app->getMouse().unlockMouse(app->getWindow());
                mouse_locked = false;
            }

            // We get a reference to the entity's position and rotation
            glm::vec3 &position = entity->localTransform.position;
            glm::vec3 &rotation = entity->localTransform.rotation;

            // If the left mouse button is pressed, we get the change in the mouse location
            // and use it to update the camera rotation
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1))
            {
                glm::vec2 delta = app->getMouse().getMouseDelta();
                // Disable pitch because this is a 2D maze, hence no movement up and down is allowed by the player.
                // rotation.x -= delta.y * controller->rotationSensitivity; // The y-axis controls the pitch
                rotation.y -= delta.x * controller->rotationSensitivity; // The x-axis controls the yaw
            }

            // We prevent the pitch from exceeding a certain angle from the XZ plane to prevent gimbal locks
            if (rotation.x < -glm::half_pi<float>() * 0.99f)
                rotation.x = -glm::half_pi<float>() * 0.99f;
            if (rotation.x > glm::half_pi<float>() * 0.99f)
                rotation.x = glm::half_pi<float>() * 0.99f;
            // This is not necessary, but whenever the rotation goes outside the 0 to 2*PI range, we wrap it back inside.
            // This could prevent floating point error if the player rotates in single direction for an extremely long time.
            rotation.y = glm::wrapAngle(rotation.y);

            // We update the camera fov based on the mouse wheel scrolling amount
            float fov = camera->fovY + app->getMouse().getScrollOffset().y * controller->fovSensitivity;
            fov = glm::clamp(fov, glm::pi<float>() * 0.01f, glm::pi<float>() * 0.99f); // We keep the fov in the range 0.01*PI to 0.99*PI
            camera->fovY = fov;

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                      up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

            glm::vec3 current_sensitivity = controller->positionSensitivity;
            // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
            if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT))
                current_sensitivity *= controller->speedupFactor;

            // We change the camera position based on the keys WASD/QE
            // S & W moves the player back and forth
            if (app->getKeyboard().isPressed(GLFW_KEY_W))
            {
                // Save the new position, then for each entity check if the other entity's collision data member is true
                // which means that this object is supposed to be checked for collision, otherwise ignore. The only thing
                // with collision set to false is the ground because otherwise it will always collide since it doesn't check
                // in 3D, hence in 2D the object always collides.
                // Only if no collision occurs with any entity, update the position.
                // Pass the win data member to the collision check function to enable checking for win condition if the item is
                // signifies the goal.
                // Also check for collectable flag, if it is true then despawn the object, and do related game logic (update counter).
                // If there is a collectable entity, then set the foundCollectable to true to indicate that the player hasn't won yet.
                glm::vec3 new_position = position + front * (deltaTime * current_sensitivity.z);
                bool collision = false;
                bool foundCollectable = false;
                auto it = world->getEntities().begin();
                while (it != world->getEntities().end())
                {
                    Entity *other_entity = *it;
                    if (other_entity != entity && other_entity->collision)
                    {
                        if (other_entity->collectable) {
                                foundCollectable = true;
                            }
                        if (check_collision(new_position, other_entity->localTransform.position, other_entity->localTransform.scale, margin, other_entity->win))
                        {
                            collision = true;
                            if (other_entity->collectable)
                            {

                                world->markForRemoval(other_entity);
                                world->deleteMarkedEntities();
                                it = world->getEntities().begin();
                            }
                        }
                    }
                    std::advance(it, 1);
                }
                // Update position if no collision or win state occured.
                if (!collision)
                {
                    position = new_position;
                }
                // If no collectable was found, trigger the win state.
                if (!foundCollectable)
                {
                    app->changeState("win");
                }
            }
            if (app->getKeyboard().isPressed(GLFW_KEY_S))
            {
                // Save the new position, then for each entity check if the other entity's collision data member is true
                // which means that this object is supposed to be checked for collision, otherwise ignore. The only thing
                // with collision set to false is the ground because otherwise it will always collide since it doesn't check
                // in 3D, hence in 2D the object always collides.
                // Only if no collision occurs with any entity, update the position.
                // Pass the win data member to the collision check function to enable checking for win condition if the item is
                // signifies the goal.
                // Also check for collectable flag, if it is true then despawn the object, and do related game logic (update counter).
                // If there is a collectable entity, then set the foundCollectable to true to indicate that the player hasn't won yet.
                glm::vec3 new_position = position - front * (deltaTime * current_sensitivity.z);
                bool collision = false;
                bool foundCollectable = false;
                auto it = world->getEntities().begin();
                while (it != world->getEntities().end())
                {
                    Entity *other_entity = *it;
                    if (other_entity != entity && other_entity->collision)
                    {
                        if (other_entity->collectable) {
                                foundCollectable = true;
                            }
                        if (check_collision(new_position, other_entity->localTransform.position, other_entity->localTransform.scale, margin, other_entity->win))
                        {
                            collision = true;
                            if (other_entity->collectable)
                            {

                                world->markForRemoval(other_entity);
                                world->deleteMarkedEntities();
                                it = world->getEntities().begin();
                            }
                        }
                    }
                    std::advance(it, 1);
                }
                // Update position if no collision or win state occured.
                if (!collision)
                {
                    position = new_position;
                }
                // If no collectable was found, trigger the win state.
                if (!foundCollectable)
                {
                    app->changeState("win");
                }
            }
            // Q & E are now disabled because this is a 2D maze, no up and down movement allowed.
            // Q & E moves the player up and down
            // if(app->getKeyboard().isPressed(GLFW_KEY_Q)) position += up * (deltaTime * current_sensitivity.y);
            // if(app->getKeyboard().isPressed(GLFW_KEY_E)) position -= up * (deltaTime * current_sensitivity.y);
            // A & D moves the player left or right
            if (app->getKeyboard().isPressed(GLFW_KEY_D))
            {
                // Save the new position, then for each entity check if the other entity's collision data member is true
                // which means that this object is supposed to be checked for collision, otherwise ignore. The only thing
                // with collision set to false is the ground because otherwise it will always collide since it doesn't check
                // in 3D, hence in 2D the object always collides.
                // Only if no collision occurs with any entity, update the position.
                // Pass the win data member to the collision check function to enable checking for win condition if the item is
                // signifies the goal.
                // Also check for collectable flag, if it is true then despawn the object, and do related game logic (update counter).
                // If there is a collectable entity, then set the foundCollectable to true to indicate that the player hasn't won yet.
                glm::vec3 new_position = position + right * (deltaTime * current_sensitivity.x);
                bool collision = false;
                bool foundCollectable = false;
                auto it = world->getEntities().begin();
                while (it != world->getEntities().end())
                {
                    Entity *other_entity = *it;
                    if (other_entity != entity && other_entity->collision)
                    {
                        if (other_entity->collectable) {
                                foundCollectable = true;
                            }
                        if (check_collision(new_position, other_entity->localTransform.position, other_entity->localTransform.scale, margin, other_entity->win))
                        {
                            collision = true;
                            if (other_entity->collectable)
                            {

                                world->markForRemoval(other_entity);
                                world->deleteMarkedEntities();
                                it = world->getEntities().begin();
                            }
                        }
                    }
                    std::advance(it, 1);
                }
                // Update position if no collision or win state occured.
                if (!collision)
                {
                    position = new_position;
                }
                // If no collectable was found, trigger the win state.
                if (!foundCollectable)
                {
                    app->changeState("win");
                }
            }
            if (app->getKeyboard().isPressed(GLFW_KEY_A))
            {
                // Save the new position, then for each entity check if the other entity's collision data member is true
                // which means that this object is supposed to be checked for collision, otherwise ignore. The only thing
                // with collision set to false is the ground because otherwise it will always collide since it doesn't check
                // in 3D, hence in 2D the object always collides.
                // Only if no collision occurs with any entity, update the position.
                // Pass the win data member to the collision check function to enable checking for win condition if the item is
                // signifies the goal.
                // Also check for collectable flag, if it is true then despawn the object, and do related game logic (update counter).
                // If there is a collectable entity, then set the foundCollectable to true to indicate that the player hasn't won yet.
                glm::vec3 new_position = position - right * (deltaTime * current_sensitivity.x);
                bool collision = false;
                bool foundCollectable = false;
                auto it = world->getEntities().begin();
                while (it != world->getEntities().end())
                {
                    Entity *other_entity = *it;
                    if (other_entity != entity && other_entity->collision)
                    {
                        if (other_entity->collectable) {
                                foundCollectable = true;
                            }
                        if (check_collision(new_position, other_entity->localTransform.position, other_entity->localTransform.scale, margin, other_entity->win))
                        {
                            collision = true;
                            if (other_entity->collectable)
                            {

                                world->markForRemoval(other_entity);
                                world->deleteMarkedEntities();
                                it = world->getEntities().begin();
                            }
                        }
                    }
                    std::advance(it, 1);
                }
                // Update position if no collision or win state occured.
                if (!collision)
                {
                    position = new_position;
                }
                // If no collectable was found, trigger the win state.
                if (!foundCollectable)
                {
                    app->changeState("win");
                }
            }
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit()
        {
            if (mouse_locked)
            {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }
    };

}
