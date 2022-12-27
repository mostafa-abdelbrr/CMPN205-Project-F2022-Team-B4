#pragma once

#include "../ecs/component.hpp"

#include <glm/mat4x4.hpp>

namespace our {

 // An enum that defines the type of the light (DIRECTIONAL, POINT, SPOT)
    enum class LightType {
        DIRECTIONAL,
        POINT,
        SPOT
    };

    class LightComponent : public Component {
    public:
        // No data members for the position and direction as they are given in the Entity itself
        LightType lightType; //the type of light (directional, point, spotlight)

        //-------------------TO BE CHECKED--------------------(mesh 3arfa ek mafroud ne7otohom walla la2)
        // // defining the diffuse ,specular, and ambient for teh light
        // glm::vec3 diffuse = {0, 0, 0}; 
        // glm::vec3 specular = {0, 0, 0};
        // glm::vec3 ambient = {0, 0, 0};
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