#include "entity.hpp"
#include "../deserialize-utils.hpp"
#include "../components/component-deserializer.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function returns the transformation matrix from the entity's local space to the world space
    // Remember that you can get the transformation matrix from this entity to its parent from "localTransform"
    // To get the local to world matrix, you need to combine this entities matrix with its parent's matrix and
    // its parent's parent's matrix and so on till you reach the root.
    glm::mat4 Entity::getLocalToWorldMatrix() const {
        //TODO: (Req 8) Write this function
        Entity * entity_parent = parent;
        glm::mat4 combined_transform = localTransform.toMat4();
        while(entity_parent != nullptr) {
            combined_transform = entity_parent->localTransform.toMat4() * combined_transform;
            entity_parent = entity_parent->parent;
        }
        return combined_transform;
    }

    // Deserializes the entity data and components from a json object
    void Entity::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        name = data.value("name", name);
        localTransform.deserialize(data);
        // Check if entity has a collision flag to determine whether to check for it or not while moving. Default is true.
        if(data.contains("collision")){
            collision = data.value("collision", collision);
        }
        // Check if entity has a win flag to determine if this is the end point of the game to show the win state.
        if(data.contains("win")){
            win = data.value("win", win);
        }
        if(data.contains("components")){
            if(const auto& components = data["components"]; components.is_array()){
                for(auto& component: components){
                    deserializeComponent(component, this);
                }
            }
        }
    }

}