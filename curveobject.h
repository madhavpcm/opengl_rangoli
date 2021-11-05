#ifndef CURVEOBJECT_H
#define CURVEOBJECT_H


#include<qopenglfunctions.h>
#include<glm/gtc/type_ptr.hpp>
#include<glm/glm.hpp>
#include<glm/vec3.hpp>
#include<vector>
#include<QDebug>

class CurveObject
{
public:
    CurveObject();
    void getFirstControlPoints();
    void getCurveControlPoints();
    std::vector<glm::vec3> updateControlPoints(std::vector<glm::vec3> &rhs);
    GLfloat getBezier(GLfloat x, GLfloat k1, GLfloat c1, GLfloat c2, GLfloat k2) ;

    void renderFCP(bool hx, bool vx);
    void renderSCP(bool hx, bool vx);
    void renderKnots(bool hx, bool vx);
    void renderCurve(bool hx, bool vx);

    glm::vec3 m_color = glm::vec3(1,1,1);
    std::vector<glm::vec3> m_knots;
    std::vector<glm::vec3> m_FCP;
    std::vector<glm::vec3> m_SCP;
    std::vector<std::vector<glm::vec3>> m_feedback;
};

#endif // CURVEOBJECT_H
