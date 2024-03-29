#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/matrix.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera
{
public:

    Camera(int WindowWidth, int WindowHeight);
    Camera(int WindowWidth, int WindowHeight, const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up);

    void SetPosition(float x, float y, float z);
    void OnMouse(int deltaX, int deltaY);
    void OnKeyboard(unsigned char key);
    void OnRender();


    glm::mat4x4 GetMatrix();

private:

    void Init();
    void Update();

    glm::vec3 m_pos;
    glm::vec3 m_target;
    glm::vec3 m_up;
    float m_speed = 0.25f;

    int m_windowWidth;
    int m_windowHeight;

    float m_AngleH;
    float m_AngleV;

    bool m_OnUpperEdge;
    bool m_OnLowerEdge;
    bool m_OnLeftEdge;
    bool m_OnRightEdge;

    glm::ivec2 m_mousePos;
};

#endif  /* CAMERA_H */