#include "camera.h"
#include <SDL_keycode.h>

static int MARGIN = 10;
static float EDGE_STEP = 1.0f;

Camera::Camera(int WindowWidth, int WindowHeight)
{
    m_windowWidth = WindowWidth;
    m_windowHeight = WindowHeight;
    m_pos = glm::vec3(0.0f, 0.0f, -2.0f);
    m_target = glm::vec3(0.0f, 0.0f, 1.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);

    Init();
}

Camera::Camera(int WindowWidth, int WindowHeight, const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up)
{
    m_windowWidth = WindowWidth;
    m_windowHeight = WindowHeight;
    m_pos = Pos;

    m_target = Target;
    m_target = glm::normalize(m_target);

    m_up = Up;
    m_up = glm::normalize(m_up);

    Init();
}

void Camera::Init()
{
    glm::vec3 HTarget(m_target.x, 0.0, m_target.z);
    HTarget = glm::normalize(HTarget);

    float Angle = glm::degrees(asin(abs(HTarget.z)));

    if (HTarget.z >= 0.0f)
    {
        if (HTarget.x >= 0.0f)
        {
            m_AngleH = 360.0f - Angle;
        }
        else
        {
            m_AngleH = 180.0f + Angle;
        }
    }
    else
    {
        if (HTarget.x >= 0.0f)
        {
            m_AngleH = Angle;
        }
        else
        {
            m_AngleH = 180.0f - Angle;
        }
    }

    m_AngleV = -glm::degrees(asin(m_target.y));

    m_OnUpperEdge = false;
    m_OnLowerEdge = false;
    m_OnLeftEdge = false;
    m_OnRightEdge = false;
    m_mousePos.x = m_windowWidth / 2;
    m_mousePos.y = m_windowHeight / 2;
}


void Camera::SetPosition(float x, float y, float z)
{
    m_pos.x = x;
    m_pos.y = y;
    m_pos.z = z;
}
const glm::vec3& Camera::GetPosition() const
{
    return m_pos;
}

const glm::vec3& Camera::GetFront() const
{
    return m_target;
}

void Camera::OnMouse(int x, int y)
{
    Update();
    return;
    int DeltaX = x * 10 ;
    int DeltaY = y * 10 ;

    m_mousePos.x = x;
    m_mousePos.y = y;

    m_AngleH += (float)DeltaX / 20.0f;
    m_AngleV += (float)DeltaY / 50.0f;

    if (DeltaX == 0) {
        if (x <= MARGIN) {
            m_OnLeftEdge = true;
        }
        else if (x >= (m_windowWidth - MARGIN)) {
            m_OnRightEdge = true;
        }
    }
    else {
        m_OnLeftEdge = false;
        m_OnRightEdge = false;
    }

    if (DeltaY == 0) {
        if (y <= MARGIN) {
            m_OnUpperEdge = true;
        }
        else if (y >= (m_windowHeight - MARGIN)) {
            m_OnLowerEdge = true;
        }
    }
    else {
        m_OnUpperEdge = false;
        m_OnLowerEdge = false;
    }
    Update();
    
}

void Camera::OnKeyboard(unsigned char Key)
{
    switch (Key) {

    case SDLK_w:
        m_pos += (m_speed * m_target);
        break;
    case SDLK_a:
        glm::vec3 Left = glm::cross(m_target, m_up);
        Left = glm::normalize(Left);
        Left *= m_speed;
        m_pos += Left;
        break;
    case SDLK_s:
        m_pos -= (m_speed * m_target);
        break;
    case SDLK_d:
        glm::vec3 Right = glm::cross(m_up, m_target);
        Right = glm::normalize(Right);
        Right *= m_speed;
        m_pos += Right;
        break;
    case SDLK_t:
        m_pos.y += m_speed;
        break;
    case SDLK_g:
        m_pos.y -= m_speed;
        break;
    case '+':
        m_speed += 0.1f;
        printf("Speed changed to %f\n", m_speed);
        break;
    case '-':
        m_speed -= 0.1f;
        if (m_speed < 0.1f) {
            m_speed = 0.1f;
        }
        printf("Speed changed to %f\n", m_speed);
        break;
    }
}

void Camera::Update()
{
#if 0
    glm::vec3 Yaxis(0.0f, 1.0f, 0.0f);
    glm::mat4 rotM = glm::mat4(1.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    glm::vec3 View(1.0f, 0.0f, 0.0f);
    rotM = glm::rotate(rotM, glm::radians(m_AngleH), Yaxis);
    glm::vec4 transformedView = rotM * glm::vec4(View, 1.0f);
    View = glm::vec3(transformedView.x, transformedView.y, transformedView.z);

    // Rotate the view vector by the vertical angle around the horizontal axis
    glm::vec3 U = glm::cross(Yaxis, View);
    U = glm::normalize(U);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_AngleV), U);
    glm::vec4 rotatedVector = rotationMatrix * glm::vec4(View, 1.0f);
    View = glm::vec3(rotatedVector.x, rotatedVector.y, rotatedVector.z);;

    m_target = View;
    m_target = glm::normalize(m_target);

    m_up = glm::normalize(glm::cross(m_target, U)); 
#endif

}

void Camera::OnRender()
{
    bool ShouldUpdate = true;

    if (m_OnLeftEdge) {
        m_AngleH -= EDGE_STEP;
        ShouldUpdate = true;
    }
    else if (m_OnRightEdge) {
        m_AngleH += EDGE_STEP;
        ShouldUpdate = true;
    }

    if (m_OnUpperEdge) {
        if (m_AngleV > -90.0f) {
            m_AngleV -= EDGE_STEP;
            ShouldUpdate = true;
        }
    }
    else if (m_OnLowerEdge) {
        if (m_AngleV < 90.0f) {
            m_AngleV += EDGE_STEP;
            ShouldUpdate = true;
        }
    }

    if (ShouldUpdate) {
        Update();
    }
}

glm::mat4x4 Camera::GetMatrix()
{
    glm::mat4x4 CameraTransformation = glm::lookAt(m_pos, m_target, m_up);
    return CameraTransformation;
}