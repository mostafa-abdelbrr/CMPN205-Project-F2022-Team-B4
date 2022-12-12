#include "shader.hpp"

#include <cassert>
#include <iostream>
#include <fstream>
#include <string>

//Forward definition for error checking functions
std::string checkForShaderCompilationErrors(GLuint shader);
std::string checkForLinkingErrors(GLuint program);

bool our::ShaderProgram::attach(const std::string &filename, GLenum type) const {
    // Here, we open the file and read a string from it containing the GLSL code of our shader
    std::ifstream file(filename);
    if(!file){
        std::cerr << "ERROR: Couldn't open shader file: " << filename << std::endl;
        return false;
    }
    std::string sourceString = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    const char* sourceCStr = sourceString.c_str();
    file.close();

    //TODO: Complete this function
    GLuint shader = glCreateShader(type); 
    // assigning the source code to the shader and compiling it
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);

    //Note: The function "checkForShaderCompilationErrors" checks if there is
    // an error in the given shader. You should use it to check if there is a
    // compilation error and print it so that you can know what is wrong with
    // the shader. The returned string will be empty if there is no errors.
    std::string Error= checkForShaderCompilationErrors(shader);
    //We return true if the compilation succeeded
    if(Error==std::string())
    {
        glAttachShader(this->program, shader);// we only attach the shader if there was no errors in compilation
        glDeleteShader(shader);//onec we attach the shader to the program we ne longer need it

        // returning true to indicate success in attaching shader
        return true;
    }
    else
    {
        std::cerr<<"ERROR IN COMPILATION: "<< Error<< std::endl;// outputing the error message
        glDeleteShader(shader); 
        // returning false to indicate failure in attaching shader
        return false;
    }
}



bool our::ShaderProgram::link() const {
    //TODO: Complete this function
    glLinkProgram(this->program);
  
    //Note: The function "checkForLinkingErrors" checks if there is
    // an error in the given program. You should use it to check if there is a
    // linking error and print it so that you can know what is wrong with the
    // program. The returned string will be empty if there is no errors.

    std::string Error= checkForLinkingErrors(program);
    if(Error==std::string())
        {
            // returning true to indicate success in linking the program
            return true;
        }
        else
        {
            std::cerr<<"ERROR IN LINKING: "<<Error<< std::endl;// outputing the error message
            // returning false to indicate failure in linking the program
            return false;
        }
}

////////////////////////////////////////////////////////////////////
// Function to check for compilation and linking error in shaders //
////////////////////////////////////////////////////////////////////

std::string checkForShaderCompilationErrors(GLuint shader){
     //Check and return any error in the compilation process
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, logStr);
        std::string errorLog(logStr);
        delete[] logStr;
        return errorLog;
    }
    return std::string();
}

std::string checkForLinkingErrors(GLuint program){
     //Check and return any error in the linking process
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char *logStr = new char[length];
        glGetProgramInfoLog(program, length, nullptr, logStr);
        std::string error(logStr);
        delete[] logStr;
        return error;
    }
    return std::string();
}