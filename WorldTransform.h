#ifndef WORLD_TRANSFORM_H
#define WORLD_TRANSFORM_H

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/matrix.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

class WorldTrans {
public:
    WorldTrans() {}

    void SetScale(float scale);
    void SetRotation(float x, float y, float z);
    void SetPosition(float x, float y, float z);

    void Rotate(float x, float y, float z);

    glm::mat4x4 GetMatrix();



private:
    float    m_scale = 1.0f;
    glm::vec3 m_rotation = glm::vec3{ 0.0f, 0.0f, 0.0f };
    glm::vec3 m_pos = glm::vec3{ 0.0f, 0.0f, 0.0f };
};


#endif