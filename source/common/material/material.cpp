#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        //TODO: (Req 7) Write this function
        pipelineState.setup();
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        if(data.contains("pipelineState")){
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint 
    void TintedMaterial::setup() const {
        //TODO: (Req 7) Write this function
        Material::setup();
        shader->set("tint", tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
    void TexturedMaterial::setup() const {
        //TODO: (Req 7) Write this function
        TintedMaterial::setup();
        shader->set("alphaThreshold", alphaThreshold);
        if (texture) {
        texture->bind();
        }
        // sampler->bind(texture->getOpenGLName());
        if (sampler) {
            sampler->bind(0);
        }
        shader->set("tex", 0);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){
        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

    void LitMaterial::setup()const {
    
        // This function calls the setup of its parent function 
        Material::setup();
        // setting the alpha threshold
        shader->set("alphaThreshold", alphaThreshold);

        // making all the seting up for the sampler and all the textures
        // bing the sampler an texture for the albedo
        glActiveTexture(GL_TEXTURE0);
        albedo->bind();
        // binding the Texture material sampler to the texture at the texture unit 0 
        sampler->bind(0);
        // send the unit number to the uniform variable "material.albedo"
        shader->set("material.albedo", 0);

        // bing the sampler an texture for the specular
        glActiveTexture(GL_TEXTURE1);
        specular->bind();
        // binding the Texture material sampler to the texture at the texture unit 1 
        sampler->bind(1);
        // send the unit number to the uniform variable "material.specular"
        shader->set("material.specular", 1);

        // bing the sampler an texture for the roughness
        glActiveTexture(GL_TEXTURE2);
        roughness->bind();
        // binding the Texture material sampler to the texture at the texture unit 2 
        sampler->bind(2);
         // send the unit number to the uniform variable "material.roughness"
        shader->set("material.roughness", 2);

        // bing the sampler an texture for the ambient_occlusion
        glActiveTexture(GL_TEXTURE3);
        ambient_occlusion->bind();
        // binding the Texture material sampler to the texture at the texture unit 2 
        sampler->bind(3);
         // send the unit number to the uniform variable "material.ambient_oclusion"
        shader->set("material.ambient_occlusion", 3);

        // bing the sampler an texture for the emission
        glActiveTexture(GL_TEXTURE4);
        emission->bind();
        // binding the Texture material sampler to the texture at the texture unit 2 
        sampler->bind(4);
         // send the unit number to the uniform variable "material.emission"
        shader->set("material.emission", 4); 
    }

    // This function read the material data from a json object
    void LitMaterial::deserialize(const nlohmann::json& data)
    {
        // deserializing the data of the material itself->parent class of the lit material
         Material::deserialize(data);
        std::printf("lit material deserialization---------------------------------------\n");
        if(!data.is_object())
         return;
        // getting the data from the json file
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
        std::printf("albedo=\n");
        albedo = AssetLoader<Texture2D>::get(data.value("albedo", ""));
        std::printf("specular\n");
        specular = AssetLoader<Texture2D>::get(data.value("specular", ""));
        roughness = AssetLoader<Texture2D>::get(data.value("roughness", ""));
        ambient_occlusion = AssetLoader<Texture2D>::get(data.value("ambient_occlusion", ""));
        emission = AssetLoader<Texture2D>::get(data.value("emission", ""));

    }

}