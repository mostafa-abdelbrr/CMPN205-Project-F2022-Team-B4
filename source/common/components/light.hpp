#pragma once

#include "../ecs/component.hpp"

#include <glm/mat4x4.hpp>

/* TODO:Create a light component. The component will identify that the entity is a light and will contain all the
    data related to the light (e.g. color, type, cone angles) except the position and the direction which
    should be calculated from the entity component.*/
namespace our {

 // An enum that defines the type of the light (DIRECTIONAL, POINT, SPOT)
    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    class LightComponent : public Component {
    public:

        LightType lightType; //the type of light (directional, point, spotlight)
        glm::vec3 color={0.0f,0.0f,0.0f};
        // No data members for the position it's given in the Entity itself
        glm::vec3 direction = {0.0f, 0.0f, 0.0f}; //this direction is used in addition to the owner's entity's direction to get the overall direction

        // attenuation constants for the point and spotlight
        float attenuation_constant;
        float attenuation_linear;
        float attenuation_quadratic;
        float inner_angle, outer_angle; //cone angles

        // The ID of this component type is "light"
        static std::string getID() { return "light"; }

        // Reads light parameters from the given json object
        void deserialize(const nlohmann::json& data) override;

    };

}