
#include "BSplineWindow.h"

void BSplineWindow::vxToggle(){
    m_hvx[m_sel].second = !m_hvx[m_sel].second;
}
void BSplineWindow::hxToggle(){
    m_hvx[m_sel].first= !m_hvx[m_sel].first;
}
void BSplineWindow::mousePressEvent(QMouseEvent *e){
        glm::vec2 nmc(e->pos().x(),e->pos().y());
        win2glcoord(nmc);
        std::pair<int,int> cindex  = closestKnot(nmc);
    if(e->button() == Qt::RightButton){
        if(cindex.second !=0){
            return;
        }
        else{
            if(m_curves[m_sel].m_knots.size() >2){
                m_curves[m_sel].m_knots.erase(m_curves[m_sel].m_knots.begin() + cindex.first);
                m_curves[m_sel].getCurveControlPoints();
                renderNow();
            }
        }
        return;
    }else
   if(e->button() == Qt::LeftButton && !m_isknotselected){

        if (cindex != std::make_pair(-1,-1))
            m_isknotselected = true;
        return;

    }else
    if(e->button() == Qt::MiddleButton && !m_isknotselected){
        m_curves[m_sel].m_knots.push_back(glm::vec3(nmc.x,nmc.y,0));//add new control point if no close one exists
        //std::sort(m_knots.begin(), m_knots.end(), [] (const glm::vec3& a,const glm::vec3& b){return a.x < b.x;});
        m_curves[m_sel].getCurveControlPoints();
        renderNow();
        return;
    }
    m_isknotselected = false;
}

void BSplineWindow::mouseReleaseEvent(QMouseEvent *e){
    if(e->button() == Qt::LeftButton){
       m_isknotselected = false;
    }
}

void BSplineWindow::mouseMoveEvent(QMouseEvent *e){

   if( m_isknotselected  ){
        glm::vec2 nmc(e->pos().x(),e->pos().y());
        win2glcoord(nmc);
        std::pair<int,int> cindex  = closestKnot(nmc);

        if(cindex.second == 0 ){
            dragMouse(cindex.first, nmc);
            m_curves[m_sel].getCurveControlPoints();
            renderNow();
        }else if (cindex.second== 1){
            m_curves[m_sel].m_FCP[cindex.first] = glm::vec3(nmc,0);
            renderNow();
        }else if (cindex.second== 2) {
            m_curves[m_sel].m_SCP[cindex.first] = glm::vec3(nmc,0);
            renderNow();
        }
        else if (cindex.first < 0){
            //m_knots.push_back(glm::vec3(nmc.x,nmc.y,0));//add new control point if no close one exists
            //getCurveControlPoints();
            //renderNow();
        }

        //std::cout << nmc.x << " :: " << nmc.y << " \n";
    }
}

void BSplineWindow::mouseDoubleClickEvent(QMouseEvent *e){
    if( e->button() == Qt::LeftButton)    {
        m_curves[m_sel].getCurveControlPoints();
        renderNow();
    }
}

void BSplineWindow::initImgui(){
}
void BSplineWindow::initialize()
{
    m_isknotselected = false;
    m_program = new QOpenGLShaderProgram(this);
    QFile vertexShaderSource(":/shaders/shaders/vertex.vert"),
          fragmentShaderSource(":/shaders/shaders/fragment.frag");
    if(!vertexShaderSource.open(QFile::ReadOnly |
                     QFile::Text))
       {
           qDebug() << " Could not open Vertex shader for reading";
           return;
       }
    if(!fragmentShaderSource.open(QFile::ReadOnly |
                     QFile::Text))
       {
           qDebug() << " Could not open Fragment shader for reading";
           return;
       }
    QByteArray vs = vertexShaderSource.readAll(),
               fs = fragmentShaderSource.readAll();
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vs);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fs);
    m_program->link();

    //checking uniforms, primitive check as things are fairly simple

    //m_posAttr = m_program->attributeLocation("posAttr");
    //Q_ASSERT(m_posAttr != -1);

    m_colAttr = m_program->attributeLocation("colAttr");
    Q_ASSERT(m_colAttr != -1);
    m_matrixUniform = m_program->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);
    m_coord2d = m_program->attributeLocation("coord2d");
    Q_ASSERT(m_coord2d != -1);

    QtImGui::initialize(this);
    for(int i=0; i < 32 ; i++){
        m_curves.push_back(CurveObject());
        m_curves[i].getCurveControlPoints();
    }
    m_hvx = std::vector<std::pair<bool,bool>>(32,{0,0});

}

void BSplineWindow::renderCurveObjects(){
    for(int i=0; i < 32 ; i++){
        glm::vec3 color = glm::vec3(0,1,0) ;
        bool hx = m_hvx[i].first;
        bool vx = m_hvx[i].second;
        if(m_sel == i){
            m_program->setAttributeValue(m_colAttr,color.x,color.y,color.z);
            m_curves[i].renderFCP(0,0);
            color = {0,0,1};
            m_program->setAttributeValue(m_colAttr,color.x,color.y,color.z);
            m_curves[i].renderSCP(0,0);
            color = {1,1,1};
            m_program->setAttributeValue(m_colAttr,color.x,color.y,color.z);
            m_curves[i].renderKnots(0,0);
        }
        color = m_curves[i].m_color;
        m_program->setAttributeValue(m_colAttr,color.x,color.y,color.z);
        m_curves[i].renderCurve(hx,vx);
    }
}
void BSplineWindow::renderTools(){
    QtImGui::newFrame();
    ImGui::Begin("Tools");
        //ImGui::SetWindowPos(ImVec2(0,0),0);
        if(ImGui::Button("Hx"))
            hxToggle();
        if(ImGui::Button("Vx"))
            vxToggle();
        ImGui::Text("Layer:");
        ImGui::SliderInt("",&m_sel, 0, 31);
        ImGui::ColorPicker3("Layer:Color,",glm::value_ptr( m_curves[m_sel].m_color));
    ImGui::End();
}
void BSplineWindow::renderAxes(){
    m_program->setAttributeValue(m_colAttr,1,1,1);
    glLineStipple(1,0xebAA);
    glEnable(GL_LINE_STIPPLE);
    if(m_hvx[m_sel].second){
        glBegin(GL_LINES);
        glVertex3f(0,-10,0);
        glVertex3f(0,10,0);
        glEnd();
    }
    if(m_hvx[m_sel].first){
        glBegin(GL_LINES);
        glVertex3f(-10,0,0);
        glVertex3f(10,0,0);
        glEnd();
    }
    glDisable(GL_LINE_STIPPLE);


}
void BSplineWindow::render()
{

    glm::vec3 clear_color(0,0,0);

    renderTools();

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);
    glClear(GL_COLOR_BUFFER_BIT);

    //std::cout << "rendering\n";
    m_program->bind();

    QMatrix4x4 matrix;
    matrix.ortho(-10.0,10,-10,10,0.1f,100.0f);
    //matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -100);
   // matrix.rotate(100.0f * m_frame / screen()->refreshRate(), 0, 1, 0);
    m_program->setUniformValue(m_matrixUniform, matrix);

    glEnable(GL_BLEND) ;
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


    m_program->setAttributeBuffer(m_colAttr, GL_FLOAT, 0,3,0);
    m_program->enableAttributeArray(m_colAttr);

    renderCurveObjects();

    renderAxes();
    m_program->disableAttributeArray(m_colAttr);
    //bezBuffer.release();
    m_program->release();


    ImGui::Render();
    QtImGui::render();
    glDisable(GL_BLEND);
    ++m_frame;
}

void BSplineWindow::dragMouse(int indx, glm::vec2 &nmc){
        m_curves[m_sel].m_knots[indx] = glm::vec3(nmc, 0);
        //std::sort(m_knots.begin(), m_knots.end(), [] (const glm::vec3& a,const glm::vec3& b)
        //{ return a.x < b.x; });

}

std::pair<int,int> BSplineWindow::closestKnot(glm::vec2 &v){
    GLfloat min= 3.402823466E38;
    uint32_t indx=-1;
    int id=-1;
   for(size_t i =0 ; i< m_curves[m_sel].m_knots.size(); i++){
        GLfloat x=glm::distance(m_curves[m_sel].m_knots[i],glm::vec3(v,0));
        if(x < min){
            min=x;
            indx=i;
            id=0;
        }
   }
   for(size_t i=0; i < m_curves[m_sel].m_FCP.size() ;i++){
      GLfloat x = glm::distance(m_curves[m_sel].m_FCP[i],glm::vec3(v,0));
      if ( x < min){
          min=x;
          indx=i;
          id=1;
      }
   }
   for(size_t i=0; i < m_curves[m_sel].m_SCP.size() ;i++){
      GLfloat x = glm::distance(m_curves[m_sel].m_SCP[i],glm::vec3(v,0));
      if ( x < min){
          min=x;
          indx=i;
          id=2;
      }
   }
   if(min <= 0.6f)
       return {indx,id};
   else
       return {-1,-1};
}

void BSplineWindow::win2glcoord(glm::vec2 & v){
//translate according to pre defined size, TODO make this better
        v.x =( v.x / width()) * 20;
        v.y =( v.y / height())* 20;
        v.x -= 10;
        v.y -= 10;
        v.y *= -1;
}

