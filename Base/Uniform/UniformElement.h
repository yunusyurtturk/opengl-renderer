#pragma once
#include <memory>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "UniformSlot.h"

class Texture;

template<typename T>
class UniformElement : public UniformSlot {
private:
    std::string name;
    GLuint uniformLocation = -1;
    GLuint program;
    T* pBindedValue = nullptr;
    T valHolder;

public:
    UniformElement(GLuint program, std::string uniform_name) : program(program), name(std::move(uniform_name)) {
        uniformLocation = glGetUniformLocation(program, name.c_str());
    }

    void bind(T *p_val) {
        pBindedValue = p_val;
    }

    virtual void Update() override
    {
        if constexpr (std::is_same_v<T, float>) {
            glUniform1f(uniformLocation, *pBindedValue);
        }
        else if constexpr (std::is_same_v<T, int>) {
            glUniform1i(uniformLocation, *pBindedValue);
        }
        else if constexpr (std::is_same_v<T, glm::vec2>) {
            glUniform2fv(uniformLocation, 1, glm::value_ptr(pBindedValue));
        }
        else if constexpr (std::is_same_v<T, glm::vec3>) {
            glUniform3fv(uniformLocation, 1, (GLfloat *)pBindedValue);
        }
        else if constexpr (std::is_same_v<T, glm::vec4>) {
            glUniform4fv(uniformLocation, 1, glm::value_ptr(pBindedValue));
        }
        else if constexpr (std::is_same_v<T, Texture>) {

            glBindTexture(pBindedValue->getTextureTarget(), pBindedValue->getTexture());
            glUniform1i(uniformLocation, 0);
        }
        else if constexpr (std::is_same_v<T, Texture*>) {
            unsigned int texture_index = (*pBindedValue)->getTextureIndex();
            GLuint texture = (*pBindedValue)->getTexture();
            GLenum target = (*pBindedValue)->getTextureTarget();
            glUniform1i(uniformLocation, texture_index);
            glActiveTexture(GL_TEXTURE0 + texture_index);
            glBindTexture(target, texture);
        }
        else if constexpr (std::is_same_v<T, std::shared_ptr<Texture>>) {
            unsigned int texture_index = pBindedValue->get()->getTextureIndex();
            GLuint texture = (*pBindedValue)->getTexture();
            GLenum target = (*pBindedValue)->getTextureTarget();
            glActiveTexture(GL_TEXTURE0 + texture_index);
            glBindTexture(target, texture);
            glUniform1i(uniformLocation, texture_index);
        }

        
    }

    void set(T value) const {
        valHolder = value;
        bind(&valHolder);
    }
};
