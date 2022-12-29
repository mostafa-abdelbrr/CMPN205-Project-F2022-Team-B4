#include "light.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    // function should read all light data from given json object 
    void LightComponent::deserialize(const nlohmann::json& data){

      if(!data.is_object())
         return;
      // light type is given in the json file as an int so we read it and convert it to a LightType
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
      //  reading the light color, direction, attenuation, and angles
      color = data.value("color",  glm::vec3 (1.0f, 1.0f, 1.0f));
      std::printf("light color\n");
      direction = data.value("direction",  glm::vec3 (-1.0f, 0.0f, 0.0f));
      std::printf("light direction\n");
      inner_angle = data.value("inner_angle", glm::radians(15.0f));
      std::printf("light inner_angle\n");
      outer_angle= data.value("outer_angle", glm::radians(30.0f));
      std::printf("light outer_angle\n");

      attenuation_constant= data.value("attenuation_constant", 0.0f);
      std::printf("light attenuation_constant\n");
      attenuation_linear= data.value("attenuation_linear", 0.0f);
      std::printf("light attenuation_linear\n");
      attenuation_quadratic= data.value("attenuation_quadratic", 1.0f);
      std::printf("light attenuation_quadratic\n");
    }
}