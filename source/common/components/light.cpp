#include "light.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    // function should read all light data from given json object
    // -----------------TO BE CHECKED--------------------- (dont know how exactly do we read from json files)
    void LightComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object())
            return;
        color = data.value("color",  glm::vec3 (1.0f, 1.0f, 1.0f));
        inner_angle = data.value("inner_angle", glm::vec2(glm::radians(15.0f)));
        outer_angle= data.value("outer_angle", glm::vec2(glm::radians(15.0f)));
        
        attenuation_constant= data.value("attenuation_constant", 0.0f);
        attenuation_linear= data.value("attenuation_linear", 0.0f);
        attenuation_quadratic= data.value("attenuation_quadratic", 1.0f);

        int lightValue;
        lightValue = data.value("lightType", 0);
         
        if(lightValue==0)
         {
            lightType=LightType::DIRECTIONAL;
         }
         else if(lightValue==1)
         {   
            lightType=LightType::POINT;

         }
         else if(lightValue==2)
         {
            lightType=LightType::SPOT;
         }
         
        

    }
}