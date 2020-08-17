#include "../../headers/OpenGlViewer/openglviewer.h"
#include "ui_openglviewer.h"


const GLchar* vertexShaderSource = "#version 330 core\n"
                                   "layout (location = 0) in vec3 position;\n"
                                   "layout(location = 1) in vec3 normal;\n"
                                   "uniform mat4 projMatrix;\n"
                                   "uniform mat4 transMatrix;\n"
        // "out vec3 ourColor;\n"
        "out vec3 Normal;\n"
        "out vec3 FragPos;\n"

        "void main()\n"
        "{\n"
        "vec4 tempPos = vec4(position, 1.0);\n"
        "Normal =normal;\n"
        "FragPos =vec3(transMatrix*vec4(position,1.0f));\n"
        "gl_Position = projMatrix * transMatrix*  tempPos;\n"
        "}\0";


// "ourColor = color;\n"

const GLchar* fragmentShaderSource = "#version 330 core\n"
                                     "out vec4 color;\n"
                                     "in vec3 FragPos;\n"
                                     "in vec3 Normal;\n"

                                     "uniform vec3 LightPosition;\n"
                                     "uniform vec3 ourColor;\n"
                                     "uniform vec3 lightColor;\n"
                                     "void main()\n"
                                     "{\n"

                                     "vec3 ambient = 0.1f *lightColor;\n"

                                     "vec3 norm =normalize(Normal);\n"
        //"vec3 lightDir =normalize(LightPosition-FragPos);\n"
        "vec3 lightDir =normalize(vec3(0.5,0.5,0.5));\n"

        "float diff=max(dot(norm,lightDir),0.0);\n"
        "vec3 diffuse=diff*lightColor;\n"
        //  "color = vec4 (lightColor*ourColor, 1.0f);\n"
        "vec3 result=(ambient+diffuse)*ourColor;\n"
        "color=vec4(result,1.0f);\n"
        //"color =ourColor;\n"

        "}\n\0";

//   "color = ourColor;\n"

OpenGlViewer::OpenGlViewer( QWidget *parent)
    : QGLWidget(parent) {

    ui->setupUi(this);

    rotation.setVector(0,0,0);
    // rotationAxis=QVector3D(0,0,0);

    sizeDrawVertex=0;
    distanceInfo.clear();
    drawVertex=nullptr;

    translateX=0;
    translateY=0;

    translateSpeed=5;


    ratioWidthHeight=1;

    setFormat(QGLFormat(QGL::DoubleBuffer));  // double buff

    light_diffuse[0]=0.5;
    light_diffuse[1]=0.5;
    light_diffuse[2]=0.5;
    light_diffuse[2]=0.5;

    light_ambient[0]=0.4;
    light_ambient[1]=0.4;
    light_ambient[2]=0.4;
    light_ambient[3]=0.4;

    minMaxXYZ[0]=minMaxXYZ[2]=minMaxXYZ[4]=100000000;   //min X Y Z
    minMaxXYZ[1]=minMaxXYZ[3]=minMaxXYZ[5]=-100000000;  //max X Y Z

    scaleSpeed = 0.001;
    //  scaleWheel = orthoCoefficient * maxOrigin*0.1;
    //    scaleSpeed = 0.001;
    scaleWheel = 1;

    light_position[0]=0;
    light_position[1]=0;
    light_position[2]=1;
    light_position[3]=0;


    light_position2[0]=0;
    light_position2[1]=0;
    light_position2[2]=-1;
    light_position2[3]=0;

    rotate_x=0;
    rotate_y=0;

    translateX=0;
    translateY=0;


    sizeDrawVertex=0;



}

OpenGlViewer::~OpenGlViewer() {
    if(drawVertex!=nullptr)
        delete [] drawVertex;
    delete ui;
}

void OpenGlViewer::initializeGL() {

    initializeOpenGLFunctions();

    f = QOpenGLContext::currentContext()->extraFunctions();

    GLuint vertexShader=f->glCreateShader(GL_VERTEX_SHADER);
    f->glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    f->glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    f->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        f->glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
    }
    // Fragment shader
    GLuint fragmentShader = f->glCreateShader(GL_FRAGMENT_SHADER);
    f->glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    f->glCompileShader(fragmentShader);
    // Check for compile time errors
    f->glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        f->glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        qDebug()  << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
    }
    // Link shaders
    shaderProgram = f->glCreateProgram();
    f->glAttachShader(shaderProgram, vertexShader);
    f->glAttachShader(shaderProgram, fragmentShader);
    f->glLinkProgram(shaderProgram);
    // Check for linking errors
    f->glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        f->glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
    }
    f->glDeleteShader(vertexShader);
    f->glDeleteShader(fragmentShader);


    // Set up vertex data (and buffer(s)) and attribute pointers


    f->glGenVertexArrays(1, &VAO);


    f->glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    f->glBindVertexArray(VAO);

    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //  f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                             reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);

    f->glBindVertexArray(0); // Unbind VAO


    f->glDepthFunc(GL_LEQUAL);   // buff deep
    f->glEnable(GL_DEPTH_TEST);  // line that we can't see - become invisible

    f->glEnable(GL_COLOR_MATERIAL);
    f->glEnable(GL_NORMALIZE);
    // Game loop
    f->glUseProgram(shaderProgram);
    // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions

    GPUobjectColor =  f->glGetUniformLocation(shaderProgram, "ourColor");
    GPUtransformMatrix=f->glGetUniformLocation(shaderProgram, "transMatrix");
    GPUprojectionMatrix=f->glGetUniformLocation(shaderProgram, "projMatrix");
    GPUlightPosition=f->glGetUniformLocation(shaderProgram, "LightPosition");
    GPUlightColor=f->glGetUniformLocation(shaderProgram, "lightColor");


}
void OpenGlViewer::resizeGL(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // qglClearColor(BACKGROUND_COLOR);
    glViewport(0, 0, (GLint)w, (GLint)h);
    aspect = qreal(w) / qreal(h);
}

void OpenGlViewer::paintGL() {
    f->glClearColor(BACKGROUND_COLOR[0],BACKGROUND_COLOR[1],BACKGROUND_COLOR[2],1.0f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_transform.setToIdentity();
    m_projection.setToIdentity();

    m_transform.rotate(rotation);
    m_transform.translate(-(minMaxXYZ[1] + minMaxXYZ[0])/2.0f,-(minMaxXYZ[3] + minMaxXYZ[2])/2.0f,-(minMaxXYZ[4] + minMaxXYZ[5])/2.0f);

    m_projection.ortho(-maxOrigin*scaleWheel*aspect,maxOrigin*scaleWheel*aspect,
                       -maxOrigin*scaleWheel,maxOrigin*scaleWheel,
                       -maxOrigin*scaleWheel*3,maxOrigin*scaleWheel*3);

    //set transform matrix in geometry shader
    f->glUniformMatrix4fv(GPUtransformMatrix,1,GL_FALSE,m_transform.constData());
    //set projection matrix in geometry shader
    f->glUniformMatrix4fv(GPUprojectionMatrix,1,GL_FALSE,m_projection.constData());
    //set light position in geometry shader
    f->glUniform3f(GPUlightPosition,light_position[0],light_position[1],light_position[2]);
    //set color light in geometry shader (white)
    f->glUniform3f(GPUlightColor,1,1,1);

    f->glBindVertexArray(VAO);

    if(isDrawFaces)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        //draw faces first mesh
        f->glUniform3f(GPUobjectColor, 0.5f, 0.5f, 0.5f);   //color of faces first mesh
        f->glDrawArrays(GL_TRIANGLES , 0, sizeDrawVertexFirstObject/6);

        //draw faces second mesh
        f->glUniform3f(GPUobjectColor, 0.0f, 0.5f, 1.0f);   //color of faces second mesh
        f->glDrawArrays(GL_TRIANGLES , sizeDrawVertexFirstObject/6, sizeDrawVertex/6);
    }

    if(isDrawGrid)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        //draw grid first mesh
        f->glUniform3f(GPUobjectColor, 0.0f, 0.0f, 0.7f);   //color of grid first mesh
        f->glDrawArrays(GL_TRIANGLES , 0, sizeDrawVertexFirstObject/6);

        //draw grid second mesh
        f->glUniform3f(GPUobjectColor, 0.0f, 1.0f, 0.0f);//color of grid second mesh
        f->glDrawArrays(GL_TRIANGLES , sizeDrawVertexFirstObject/6, sizeDrawVertex/6);
    }

    f->glBindVertexArray(0);
}



void OpenGlViewer::mouseMoveEvent(QMouseEvent *e) {

    //    if (e->buttons() & Qt::LeftButton && cameraMove) {

    //        QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;
    //        QVector2D n = QVector2D(diff.x(),diff.y()).normalized();

    //       // x_pos = e->x();
    //      //  y_pos = e->y();
    //        translateX -= n.x()* translateSpeed;
    //        translateY -= n.y() * translateSpeed;
    //        qDebug()<<"transX="<<translateX<<" transY="<<translateY;

    //        mousePressPosition=QVector2D(e->localPos());
    //       // prevRotation_x = x_pos;
    //       // prevRotation_y = y_pos;
    //        update();  // update Form that display Object
    //        qDebug() << "Shift";
    //        return;
    //    }
    //    else
    if (e->buttons() & Qt::LeftButton) {
        QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

        //QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
        // qreal acc = diff.length() / 100.0;

        // Calculate new rotation axis as weighted sum
        //rotationAxis = ( QVector3D(diff.y(), diff.x(), 0.0).normalized() * diff.length() / 100.0).normalized();

        rotation = QQuaternion::fromAxisAndAngle((QVector3D(diff.y(), diff.x(), 0.0).normalized() * diff.length() / 100.0).normalized(), rotationSpeed) * rotation;

        mousePressPosition=QVector2D(e->localPos());
        // Request an update
        update();
    }
}
void OpenGlViewer::mousePressEvent(QMouseEvent *e) {
    mousePressPosition = QVector2D(e->localPos());
}

void OpenGlViewer::mouseReleaseEvent(QMouseEvent *e) {

}

void OpenGlViewer::wheelEvent(QWheelEvent *event) {
    if(scaleWheel<0.1)
    {
        scaleWheel=0.1;
        return;
    }
    scaleWheel +=
            event->angleDelta().y()*scaleSpeed;  // change scale when scroll wheel

    //  qDebug()<<"scale="<<scaleWheel;
    update();
}

void OpenGlViewer::keyPressEvent(QKeyEvent *event)
{
    //    if (event->key() == Qt::Key_Shift) {
    //        cameraMove = true;
    //    }
}

void OpenGlViewer::keyReleaseEvent(QKeyEvent *event)
{
    //    if (event->key() == Qt::Key_Shift) {
    //        cameraMove = false;
    //    }
}

void OpenGlViewer::findMinMaxForStl()
{
    for(int i=0;i<sizeDrawVertex;i+=3)
    {
        if(minMaxXYZ[0]>drawVertex[i])
            minMaxXYZ[0]=drawVertex[i];
        if(minMaxXYZ[1]<drawVertex[i])
            minMaxXYZ[1]=drawVertex[i];

        if(minMaxXYZ[2]>drawVertex[i+1])
            minMaxXYZ[2]=drawVertex[i+1];
        if(minMaxXYZ[3]<drawVertex[i+1])
            minMaxXYZ[3]=drawVertex[i+1];

        if(minMaxXYZ[4]>drawVertex[i+2])
            minMaxXYZ[4]=drawVertex[i+2];
        if(minMaxXYZ[5]<drawVertex[i+2])
            minMaxXYZ[5]=drawVertex[i+2];
    }
}

int OpenGlViewer::getSizeTriangleFractal(int n, int Depth)
{
    int result=5;
    --Depth;
    for(int i=0;i<Depth;++i)
        result=result*4+1;
    return n*result;
}





void OpenGlViewer::InitMaxOrigin()
{

    minMaxXYZ[0]=minMaxXYZ[2]=minMaxXYZ[4]=100000000;
    minMaxXYZ[1]=minMaxXYZ[3]=minMaxXYZ[5]=-100000000;
    findMinMaxForStl();





    double distanceX=abs((minMaxXYZ[1]-minMaxXYZ[0]));
    double distanceY=abs((minMaxXYZ[3]-minMaxXYZ[2]));
    double distanceZ=abs((minMaxXYZ[5]-minMaxXYZ[4]));


    maxOrigin=distanceX;

    if(maxOrigin<distanceY)
        maxOrigin=distanceY;
    if(maxOrigin<distanceZ)
        maxOrigin=distanceZ;


}

void OpenGlViewer::updateDrawVertex()
{

    //    if(drawVertex!=nullptr)
    //        delete[] drawVertex;

    //    int n=18;

    //    sizeDrawVertex=n*(drawFirstObject->face.size()+drawSecondObject->face.size());


    //    drawVertex=new GLfloat[sizeDrawVertex];


    //    sizeDrawVertexFirstObject=drawFirstObject->face.size();

    //    for(int i=0;i<sizeDrawVertexFirstObject;++i)
    //    {
    //        drawVertex[n*i  ]=(*drawFirstObject).face[i].P0(0).X();
    //        drawVertex[n*i+1]=(*drawFirstObject).face[i].P0(0).Y();
    //        drawVertex[n*i+2]=(*drawFirstObject).face[i].P0(0).Z();

    //        drawVertex[n*i+3]=(*drawFirstObject).face[i].N().X();
    //        drawVertex[n*i+4]=(*drawFirstObject).face[i].N().Y();
    //        drawVertex[n*i+5]=(*drawFirstObject).face[i].N().Z();

    //        drawVertex[n*i+6]=(*drawFirstObject).face[i].P0(1).X();
    //        drawVertex[n*i+7]=(*drawFirstObject).face[i].P0(1).Y();
    //        drawVertex[n*i+8]=(*drawFirstObject).face[i].P0(1).Z();

    //        drawVertex[n*i+9]=(*drawFirstObject).face[i].N().X();
    //        drawVertex[n*i+10]=(*drawFirstObject).face[i].N().Y();
    //        drawVertex[n*i+11]=(*drawFirstObject).face[i].N().Z();

    //        drawVertex[n*i+12]=(*drawFirstObject).face[i].P0(2).X();
    //        drawVertex[n*i+13]=(*drawFirstObject).face[i].P0(2).Y();
    //        drawVertex[n*i+14]=(*drawFirstObject).face[i].P0(2).Z();

    //        drawVertex[n*i+15]=(*drawFirstObject).face[i].N().X();
    //        drawVertex[n*i+16]=(*drawFirstObject).face[i].N().Y();
    //        drawVertex[n*i+17]=(*drawFirstObject).face[i].N().Z();
    //    }
    //    sizeDrawVertexFirstObject*=n;

    //    int  size2=drawSecondObject->face.size();

    //    for(int i=0;i<size2;++i)
    //    {
    //        drawVertex[sizeDrawVertexFirstObject + n*i  ]=(*drawSecondObject).face[i].P0(0).X();
    //        drawVertex[sizeDrawVertexFirstObject +n*i+1]=(*drawSecondObject).face[i].P0(0).Y();
    //        drawVertex[sizeDrawVertexFirstObject +n*i+2]=(*drawSecondObject).face[i].P0(0).Z();

    //        drawVertex[sizeDrawVertexFirstObject +n*i+3]=(*drawSecondObject).face[i].N().X();
    //        drawVertex[sizeDrawVertexFirstObject +n*i+4]=(*drawSecondObject).face[i].N().Y();
    //        drawVertex[sizeDrawVertexFirstObject +n*i+5]=(*drawSecondObject).face[i].N().Z();

    //        drawVertex[sizeDrawVertexFirstObject +n*i+6]=(*drawSecondObject).face[i].P0(1).X();
    //        drawVertex[sizeDrawVertexFirstObject +n*i+7]=(*drawSecondObject).face[i].P0(1).Y();
    //        drawVertex[sizeDrawVertexFirstObject +n*i+8]=(*drawSecondObject).face[i].P0(1).Z();

    //        drawVertex[sizeDrawVertexFirstObject +n*i+9]=(*drawSecondObject).face[i].N().X();
    //        drawVertex[sizeDrawVertexFirstObject +n*i+10]=(*drawSecondObject).face[i].N().Y();
    //        drawVertex[sizeDrawVertexFirstObject +n*i+11]=(*drawSecondObject).face[i].N().Z();

    //        drawVertex[sizeDrawVertexFirstObject +n*i+12]=(*drawSecondObject).face[i].P0(2).X();
    //        drawVertex[sizeDrawVertexFirstObject +n*i+13]=(*drawSecondObject).face[i].P0(2).Y();
    //        drawVertex[sizeDrawVertexFirstObject +n*i+14]=(*drawSecondObject).face[i].P0(2).Z();

    //        drawVertex[sizeDrawVertexFirstObject +n*i+15]=(*drawSecondObject).face[i].N().X();
    //        drawVertex[sizeDrawVertexFirstObject +n*i+16]=(*drawSecondObject).face[i].N().Y();
    //        drawVertex[sizeDrawVertexFirstObject +n*i+17]=(*drawSecondObject).face[i].N().Z();
    //    }

    for(int i=0;i<sizeDrawVertex;++i)
        qDebug()<<drawVertex[i];
    //  f->glBufferData(GL_ARRAY_BUFFER,  (sizeDrawVertex) * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
    f->glBufferData(GL_ARRAY_BUFFER,  (sizeDrawVertex) * sizeof(GLfloat), drawVertex, GL_STATIC_DRAW);
    update();
}

void OpenGlViewer::drawFractalTriangle(std::vector<float> Point1, std::vector<float> Point2, std::vector<float> Point3, int Depth)
{


    if(Depth<1)
        return;

    std::vector<float> normalToTriangle{0,0,0};
    UtilsOperation::vectorCrossProd(Point1,Point2,Point3,normalToTriangle);

    drawVertex[sizeDrawVertex++]=Point1[0];
    drawVertex[sizeDrawVertex++]=Point1[1];
    drawVertex[sizeDrawVertex++]=Point1[2];

    drawVertex[sizeDrawVertex++]=normalToTriangle[0];
    drawVertex[sizeDrawVertex++]=normalToTriangle[1];
    drawVertex[sizeDrawVertex++]=normalToTriangle[2];

    drawVertex[sizeDrawVertex++]=Point2[0];
    drawVertex[sizeDrawVertex++]=Point2[1];
    drawVertex[sizeDrawVertex++]=Point2[2];

    drawVertex[sizeDrawVertex++]=normalToTriangle[0];
    drawVertex[sizeDrawVertex++]=normalToTriangle[1];
    drawVertex[sizeDrawVertex++]=normalToTriangle[2];

    drawVertex[sizeDrawVertex++]=Point3[0];
    drawVertex[sizeDrawVertex++]=Point3[1];
    drawVertex[sizeDrawVertex++]=Point3[2];

    drawVertex[sizeDrawVertex++]=normalToTriangle[0];
    drawVertex[sizeDrawVertex++]=normalToTriangle[1];
    drawVertex[sizeDrawVertex++]=normalToTriangle[2];



    float x12=(Point1[0]+Point2[0])/2;
    float y12=(Point1[1]+Point2[1])/2;
    float z12=(Point1[2]+Point2[2])/2;

    float x23=(Point2[0]+Point3[0])/2;
    float y23=(Point2[1]+Point3[1])/2;
    float z23=(Point2[2]+Point3[2])/2;

    float x31=(Point3[0]+Point1[0])/2;
    float y31=(Point3[1]+Point1[1])/2;
    float z31=(Point3[2]+Point1[2])/2;



    drawFractalTriangle({x12,y12,z12},{x23,y23,z23},{x31,y31,z31},Depth-1);

    drawFractalTriangle({Point1[0],Point1[1],Point1[2]},{x12,y12,z12},{x31,y31,z31},Depth-1);
    drawFractalTriangle({Point2[0],Point2[1],Point2[2]},{x12,y12,z12},{x23,y23,z23},Depth-1);
    drawFractalTriangle({Point3[0],Point3[1],Point3[2]},{x31,y31,z31},{x23,y23,z23},Depth-1);




}



//void OpenGlViewer::drawFirstMesh()
//{
//    uint size=drawFirstObject->face.size();
//    glBegin(GL_TRIANGLES);  // START TRIANGLES DRAWING
//    for (uint i = 0; i < size; ++i) {
//        glNormal3f((*drawFirstObject).face[i].N().X(),(*drawFirstObject).face[i].N().Y(),(*drawFirstObject).face[i].N().Z());
//        glVertex3f((*drawFirstObject).face[i].P0(0).X(),(*drawFirstObject).face[i].P0(0).Y(),(*drawFirstObject).face[i].P0(0).Z());
//        glVertex3f((*drawFirstObject).face[i].P0(1).X(),(*drawFirstObject).face[i].P0(1).Y(),(*drawFirstObject).face[i].P0(1).Z());
//        glVertex3f((*drawFirstObject).face[i].P0(2).X(),(*drawFirstObject).face[i].P0(2).Y(),(*drawFirstObject).face[i].P0(2).Z());
//    }
//    glEnd();  // END TRIANGLES DRAWING
//}





void OpenGlViewer::setLight(bool value)
{
    isLight=value;
    update();

    int Depth=10;
    int n=18;
    if(drawVertex!=nullptr)
        delete [] drawVertex;

    drawVertex=new GLfloat[getSizeTriangleFractal(n,Depth)];
    //qDebug()<<"Size="<<GLfloat(n*pow(3,Depth));
    drawFractalTriangle({-100,0,0},{100,0,0}, {0,100,0},Depth);

    InitMaxOrigin();
    updateDrawVertex();
    update();
}




void OpenGlViewer::setShowGrid(bool value)
{
    isDrawGrid=value;
    update();
}

void OpenGlViewer::setShowFaces(bool value)
{
    isDrawFaces=value;
    update();
}



