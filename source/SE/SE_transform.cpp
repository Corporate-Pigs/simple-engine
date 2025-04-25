#include "SE_transform.h"

#include <math.h>

void SimpleEngine::Transform::RotateAroundPoint(const cppvec::Vec2<float>& p_point, const float p_radians)
{
    float s = sin(p_radians);
    float c = cos(p_radians);

    m_position -= p_point;

    m_position.x = m_position.x * c - m_position.y * s;
    m_position.y = m_position.x * s + m_position.y * c;
    
    m_position += p_point;
}