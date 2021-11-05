#include "curveobject.h"

CurveObject::CurveObject()
{
    m_color = glm::vec3(0);
    static std::vector<glm::vec3> ctrlpoints = {
            { -4.0, -4.0,0.0}, { -2.0, 4.0,0.0},
            {2.0, -4.0,0.0}, {4.0, 4.0,0.0}
    };
    for(glm::vec3 v : ctrlpoints){
        m_knots.push_back(v);
    }
    getCurveControlPoints();
}
void CurveObject::renderFCP(bool hx, bool vx){
    glPointSize(5.0);
    glBegin(GL_POINTS);
    for(glm::vec3 v : m_FCP)
        glVertex3f(v.x,v.y,v.z);
    glEnd();
    if(hx && vx){
        glBegin(GL_LINE_STRIP);
                for(glm::vec3 v : m_FCP){
                    glVertex3f(-v.x,-v.y,v.z);
                }
        glEnd();
        glBegin(GL_LINE_STRIP);
                for(glm::vec3 v : m_FCP){
                    glVertex3f(v.x,-v.y,v.z);
                }
        glEnd();
        glBegin(GL_LINE_STRIP);
                for(glm::vec3 v : m_FCP){
                    glVertex3f(-v.x,v.y,v.z);
                }
        glEnd();
    }
    else if(!hx && vx){
        glBegin(GL_LINE_STRIP);
            for(glm::vec3 v : m_FCP){
                glVertex3f(-v.x,v.y,v.z);
            }
        glEnd();

    }
    else if(hx && !vx){
        glBegin(GL_LINE_STRIP);
            for(glm::vec3 v : m_FCP){
                glVertex3f(-v.x,v.y,v.z);
            }
        glEnd();

    }

}
void CurveObject::renderSCP(bool hx, bool vx){
    glPointSize(5.0);
    glBegin(GL_POINTS);
    for(glm::vec3 v : m_SCP)
        glVertex3f(v.x,v.y,v.z);
    glEnd();
    if(hx && vx){
        glBegin(GL_LINE_STRIP);
                for(glm::vec3 v : m_SCP){
                    glVertex3f(-v.x,-v.y,v.z);
                }
        glEnd();
        glBegin(GL_LINE_STRIP);
                for(glm::vec3 v : m_SCP){
                    glVertex3f(v.x,-v.y,v.z);
                }
        glEnd();
        glBegin(GL_LINE_STRIP);
                for(glm::vec3 v : m_SCP){
                    glVertex3f(-v.x,v.y,v.z);
                }
        glEnd();
    }
    else if(!hx && vx){
        glBegin(GL_LINE_STRIP);
            for(glm::vec3 v : m_SCP){
                glVertex3f(-v.x,v.y,v.z);
            }
        glEnd();

    }
    else if(hx && !vx){
        glBegin(GL_LINE_STRIP);
            for(glm::vec3 v : m_SCP){
                glVertex3f(-v.x,v.y,v.z);
            }
        glEnd();

    }
}
void CurveObject::renderKnots(bool hx, bool vx){
    glPointSize(6.0);
    glBegin(GL_POINTS);
    for(glm::vec3 v : m_knots)
        glVertex3f(v.x,v.y,v.z);
    glEnd();
    if(hx && vx){
        glBegin(GL_LINE_STRIP);
                for(glm::vec3 v : m_knots){
                    glVertex3f(-v.x,-v.y,v.z);
                }
        glEnd();
        glBegin(GL_LINE_STRIP);
                for(glm::vec3 v : m_knots){
                    glVertex3f(v.x,-v.y,v.z);
                }
        glEnd();
        glBegin(GL_LINE_STRIP);
                for(glm::vec3 v : m_knots){
                    glVertex3f(-v.x,v.y,v.z);
                }
        glEnd();
    }
    else if(!hx && vx){
        glBegin(GL_LINE_STRIP);
            for(glm::vec3 v : m_knots){
                glVertex3f(-v.x,v.y,v.z);
            }
        glEnd();

    }
    else if(hx && !vx){
        glBegin(GL_LINE_STRIP);
            for(glm::vec3 v : m_knots){
                glVertex3f(-v.x,v.y,v.z);
            }
        glEnd();

    }
}
void CurveObject::renderCurve(bool hx,bool vx){
    m_feedback.clear();
    m_feedback = std::vector<std::vector<glm::vec3>>(m_FCP.size(),
                                                     std::vector<glm::vec3>(1000,glm::vec3(0,0,0)));
    std::vector<glm::vec3> curve(4);
    for(size_t i=0; i< m_FCP.size() ; i++){
        curve[0]=m_knots[i];
        curve[1]=(m_FCP[i]);
        curve[2]=(m_SCP[i]);
        curve[3]=(m_knots[i+1]);
        //glColor3f( 1.0, 1.0, 0.0 );
        glBegin(GL_LINE_STRIP);
            for(int j=0; j < 1000; j++){
                m_feedback[i][j] = glm::vec3(getBezier(j/1000.0,curve[0].x,curve[1].x,curve[2].x,curve[3].x),
                                        getBezier(j/1000.0,curve[0].y,curve[1].y,curve[2].y,curve[3].y),
                                        0);
                glVertex3fv(&m_feedback[i][j][0]);
            }
        glEnd();

        if(hx && vx){
            glBegin(GL_LINE_STRIP);
                    for(int j=0; j < 1000 ; j++){
                        GLfloat x,y;
                        x = -m_feedback[i][j][0];
                        y = m_feedback[i][j][1];
                        glVertex3f(x,y,0);
                    }
            glEnd();
            glBegin(GL_LINE_STRIP);
                    for(int j=0; j < 1000 ; j++){
                        GLfloat x,y;
                        x = m_feedback[i][j][0];
                        y = -m_feedback[i][j][1];
                        glVertex3f(x,y,0);
                    }
            glEnd();
            glBegin(GL_LINE_STRIP);
                    for(int j=0; j < 1000 ; j++){
                        GLfloat x,y;
                        x = -m_feedback[i][j][0];
                        y = -m_feedback[i][j][1];
                        glVertex3f(x,y,0);
                    }
            glEnd();
        }
        else if(!hx && vx){
            glBegin(GL_LINE_STRIP);
                    for(int j=0; j < 1000 ; j++){
                        GLfloat x,y;
                        x = -m_feedback[i][j][0];
                        y = m_feedback[i][j][1];
                        glVertex3f(x,y,0);
                    }
            glEnd();

        }
        else if(hx && !vx){
            glBegin(GL_LINE_STRIP);
                    for(int j=0; j < 1000 ; j++){
                        GLfloat x,y;
                        x = m_feedback[i][j][0];
                        y = -m_feedback[i][j][1];
                        glVertex3f(x,y,0);
                    }
            glEnd();

        }
        //glDisable(GL_MAP1_VERTEX_3);


    }//Control Points 1 in green
}


void CurveObject::getCurveControlPoints(){
    if(!m_knots.size()){
        qDebug()<<"knots is empty"; return;
    }
    if(m_knots.size() < 2){
        qDebug() << "knots needs at least 2 points"; return;
    }
    if(m_knots.size() == 2){
        m_FCP = std::vector<glm::vec3>(1);
        m_SCP = std::vector<glm::vec3>(1);

        m_FCP[0]=(glm::vec3{
                      (2 *m_knots[0].x + m_knots[1].x )/3,
                      (2 *m_knots[0].y + m_knots[1].y )/3,
                       0.0
                  });
        m_SCP[0]=(glm::vec3{
                      (2*m_FCP[0].x - m_knots[0].x),
                      (2*m_FCP[0].y - m_knots[0].y),
                       0.0
                                           });
        return;
    }
    size_t n=m_knots.size()-1;
    std::vector<glm::vec3> rhs(m_knots.size()-1);
    if(m_FCP.size() != n)
           m_FCP = std::vector<glm::vec3> (n);
    if(m_SCP.size() != n)
            m_SCP = std::vector<glm::vec3> (n);// Set right hand side X values
    for (size_t i = 1; i <n-1; ++i){
        rhs[i].x = 4 * m_knots[i].x + 2 * m_knots[i + 1].x;
        rhs[i].y = 4 * m_knots[i].y + 2 * m_knots[i + 1].y;
        rhs[i].z=0;

    }
    rhs[0].y = m_knots[0].y + 2 * m_knots[1].y;
    rhs[n - 1].y = (8 * m_knots[n - 1].y + m_knots[n].y) / 2.0;
    rhs[0].x = m_knots[0].x + 2 * m_knots[1].x;
    rhs[n - 1].x = (8 * m_knots[n - 1].x + m_knots[n].x) / 2.0;
    // Get first control points X-values
    std::vector<glm::vec3> ctrl = updateControlPoints(rhs);

    // Set right hand side Y values

    // Get first control points Y-values


    // Fill output arrays.
    for (size_t i = 0; i < n; ++i)
    {
        // First control point
        m_FCP[i]=(glm::vec3(ctrl[i].x, ctrl[i].y,0));
        // Second control point
        if (i < n - 1)
            m_SCP[i]=(glm::vec3(2 * m_knots[i + 1].x - ctrl[i + 1].x,
                                                         2 *m_knots[i + 1].y - ctrl[i + 1].y,
                                                         0));
        else
            m_SCP[i]=(glm::vec3((m_knots[n].x + ctrl[n - 1].x) / 2,
                                                         (m_knots[n].y + ctrl[n - 1].y) / 2,
                                                          0));
    }

}

std::vector<glm::vec3> CurveObject::updateControlPoints(std::vector<glm::vec3> & rhs){
    size_t n = rhs.size();
    std::vector<glm::vec3> solution(n); // Solution vector.
    std::vector<glm::vec3> tmp(n); // Solution vector.

    glm::vec3 b={2.0,2.0,0};
    solution[0] = rhs[0] / b;
    for (size_t i = 1; i < n; i++) // Decomposition and forward substitution.
    {
        tmp[i].x = 1 / b.x;
        tmp[i].y = 1/ b.y;
        b.x = (i < n - 1 ? 4.0 : 3.5) - tmp[i].x;
        solution[i].x = (rhs[i].x - solution[i - 1].x) / b.x;
        solution[i].y = (rhs[i].y - solution[i - 1].y) / b.y;

    }
    for (size_t i = 1; i < n; i++)
        solution[n - i - 1] -= tmp[n - i] * solution[n - i]; // Backsubstitution.

    return solution;
}


GLfloat CurveObject::getBezier(GLfloat x, GLfloat k1, GLfloat c1, GLfloat c2, GLfloat k2){
    GLfloat s=1-x;
    GLfloat AB= k1*s + c1*x;
    GLfloat BC= c1*s + c2*x;
    GLfloat CD= c2*s + k2*x;
    GLfloat ABC= AB*s +BC*x;
    GLfloat BCD= BC*s +CD*x;

    return ABC*s+BCD*x;
}
