#include "SE_transform.h"

#include <math.h>

void SimpleEngine::Transform::RotateAroundPoint(const cppvec::Vec2<float>& p_point, const float p_radians)
{
    float s = sin(p_radians);
    float c = cos(p_radians);

    cppvec::Vec2<float> ot = m_position - p_point;

    m_position.x = ot.x * c - ot.y * s;
    m_position.y = ot.x * s + ot.y * c;
    m_position += p_point;
}