#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram(){
            //TODO: (Req 1) Create A shader program
            program=glCreateProgram();
        }
        ~ShaderProgram(){
            //TODO: (Req 1) Delete a shader program
            if(program != 0)
            glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name) {
            //TODO: (Req 1) Return the location of the uniform with the given name
           const char* nameStr = name.c_str();
           return  glGetUniformLocation(program,nameStr);

        }

        void set(const std::string &uniform, GLfloat value) {
            //TODO: (Req 1) Send the given float value to the given uniform
             GLuint u=getUniformLocation(uniform);
             glUniform1f(u,value);
        }

        void set(const std::string &uniform, GLuint value) {
            //TODO: (Req 1) Send the given unsigned integer value to the given uniform
             GLuint u=getUniformLocation(uniform);
             glUniform1ui(u,value);
        }

        void set(const std::string &uniform, GLint value) {
            //TODO: (Req 1) Send the given integer value to the given uniform
            GLuint u=getUniformLocation(uniform);
            glUniform1i(u,value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            //TODO: (Req 1) Send the given 2D vector value to the given uniform
            GLuint u=getUniformLocation(uniform);
            glUniform2f(u,value.x,value.y);
             
        }

        void set(const std::string &uniform, glm::vec3 value) {
            //TODO: (Req 1) Send the given 3D vector value to the given uniform
            GLuint u=getUniformLocation(uniform);
            glUniform3f(u,value.x,value.y,value.z);
        }

        void set(const std::string &uniform, glm::vec4 value) {
            //TODO: (Req 1) Send the given 4D vector value to the given uniform
           GLuint u=getUniformLocation(uniform);
            glUniform4f(u,value.x,value.y,value.z,value.w);   
        }

        void set(const std::string &uniform, glm::mat4 matrix) {
            //TODO: (Req 1) Send the given matrix 4x4 value to the given uniform
            GLuint u=getUniformLocation(uniform);
            glUniformMatrix4fv(u,1,GL_FALSE,glm::value_ptr(matrix));
        }

        //TODO: (Req 1) Delete the copy constructor and assignment operator.
        ShaderProgram (const ShaderProgram&) = delete;
        ShaderProgram& operator= (const ShaderProgram&) = delete;
        //Question: Why do we delete the copy constructor and assignment operator?
        //since we are following the Resource Acquisition Is Initialization (RAII) we need to release all resources on destruction
        // we delete it since the shader program must preserve its uniqueness so we should avoid having multiple compilations
    };  

}

#endif