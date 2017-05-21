#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
    public:
        GLuint Program; //Program ID

        Shader(const GLchar* vertexPath,const GLchar* geometryPath, const GLchar* fragmentPath)
        {
            std::string vertexCode;
            std::string geometryCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream gShaderFile;
            std::ifstream fShaderFile;

            //ensures ifstream objects can throw exceptions
            vShaderFile.exceptions(std::ifstream::badbit);
            gShaderFile.exceptions(std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::badbit);

            try
            {
                //Open files
                vShaderFile.open(vertexPath);
                gShaderFile.open(geometryPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream, gShaderStream;
                //Read file's buffer contents into streams
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                gShaderStream << gShaderFile.rdbuf();
                //Close file handlers
                vShaderFile.close();
                gShaderFile.close();
                fShaderFile.close();
                //Convert stream into GLchar array
                vertexCode = vShaderStream.str();
                geometryCode = gShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch(std::ifstream::failure e)
            {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
            }
            const GLchar* vShaderCode = vertexCode.c_str();
            const GLchar* gShaderCode = geometryCode.c_str();
            const GLchar* fShaderCode = fragmentCode.c_str();
            GLuint vertex, geometry, fragment;
            GLint success;
            GLchar infoLog[512];

            //Vertex Shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            }

            //Fragment Shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            }

            //Geometry Shader
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(geometry, 1, &gShaderCode, NULL);
            glCompileShader(geometry);
            glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
            if(!success){
                glGetShaderInfoLog(geometry, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
            }
            //Shader Program
            this->Program = glCreateProgram();
            glAttachShader(this->Program, vertex);
            glAttachShader(this->Program, geometry);
            glAttachShader(this->Program, fragment);
            glLinkProgram(this->Program);
            glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            }

            glDeleteShader(vertex);
            glDeleteShader(geometry);
            glDeleteShader(fragment);
        }

        void Use(){glUseProgram(this->Program);};
};

#endif // SHADER_H
