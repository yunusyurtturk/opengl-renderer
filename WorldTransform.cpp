#include "WorldTransform.h"


void WorldTrans::SetScale(float scale)
{
    m_scale = scale;
}


void WorldTrans::SetRotation(float x, float y, float z)
{
    m_rotation.x = x;
    m_rotation.y = y;
    m_rotation.z = z;
}


void WorldTrans::SetPosition(float x, float y, float z)
{
    m_pos.x = x;
    m_pos.y = y;
    m_pos.z = z;
}


void WorldTrans::Rotate(float x, float y, float z)
{
    m_rotation.x += x;
    m_rotation.y += y;
    m_rotation.z += z;
}
glm::vec3& WorldTrans::GetPosition()
{
    return m_pos;
}


glm::mat4x4 WorldTrans::GetMatrix()
{
    glm::vec3 scaleVector(m_scale, m_scale, m_scale); // Scale along x, y, and z axes
    glm::vec3 rotationAngles(m_rotation.x, m_rotation.y, m_rotation.z); // Rotation angles in radians for x, y, and z axes
    glm::vec3 translationVector(-m_pos.x, -m_pos.y, -m_pos.z); // Translation along x, y, and z axes

    glm::mat4 Scale = glm::scale(glm::mat4x4(1.0f), scaleVector);
    glm::mat4 rotationMatrix =    glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngles.x), glm::vec3(1.0f, 0.0f, 0.0f))
                                * glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngles.y), glm::vec3(0.0f, 1.0f, 0.0f))
                                * glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngles.z), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 translationMatrix = glm::translate(glm::mat4x4(1.0f), translationVector);

    glm::mat4x4 WorldTransformation = translationMatrix * rotationMatrix * Scale; // (Scale * rotationMatrix)* translationMatrix; ;

    return WorldTransformation;
}