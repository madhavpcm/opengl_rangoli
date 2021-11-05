#ifndef BSPLINEWINDOW_H
#define BSPLINEWINDOW_H

#include "openglwindow.h"
#include <qopenglfunctions_4_5_core.h>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QScreen>
#include <QtMath>
#include <QFile>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include <Qt>
#include <QDebug>

#include <glm/vec2.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <atomic>
#include <imgui.h>
#include <QtImGui.h>

#include "curveobject.h"
//! [1]


class BSplineWindow : public OpenGLWindow
{
public:
    using OpenGLWindow::OpenGLWindow;
    //explicit BSplineWindow(QWindow *parent = nullptr);

    void initImgui();
    void initialize() override;

    void render() override;
    void renderTools();
    void renderAxes();
    void renderCurveObjects();
    std::pair<int,int> closestKnot(glm::vec2 &v);
    void win2glcoord(glm::vec2 & v);
    void dragMouse(int indx,glm::vec2 &nmc);
    void nearestPoint(glm::vec3 point, std::vector<glm::vec3>& list);
    void vxToggle();
    void hxToggle();
private:
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;
    GLint m_coord2d = 0;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e)	override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;

    std::atomic_bool m_isknotselected;
    std::atomic_bool m_isVxOn =0;
    std::atomic_bool m_isHxOn =0;
    //void mouseDoubleClickEvent(QMouseEvent *e) override;

    std::vector<CurveObject> m_curves;
    std::vector<std::pair<bool,bool>>m_hvx;
    QOpenGLShaderProgram *m_program = nullptr;

    int m_sel = 0;
    int m_frame = 0;
};
//! [1]
#endif // BSPLINEWINDOW_H
