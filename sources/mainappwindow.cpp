#include "headers/mainappwindow.h"
#include "ui_mainappwindow.h"

#include <QDebug>
MainAppWindow::MainAppWindow(  QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainAppWindow)
{
    QFont font;
    font.setPointSize(10);


    QLabel * drawGrid=new QLabel("Enable Grid");
    QLabel * drawFace=new QLabel("Enable Faces");
    QLabel * lightLabel=new QLabel("Enable Light");

    drawGrid->setFont(font);
    drawFace->setFont(font);
    lightLabel->setFont(font);

    QCheckBox * isDrawGrid=new QCheckBox();
    QCheckBox * isFaceGrid=new QCheckBox();
    QCheckBox * isLightOn=new QCheckBox();

    isDrawGrid->setChecked(false);
    isFaceGrid->setChecked(true);
    isLightOn->setChecked(false);


    QHBoxLayout * horizLay=new QHBoxLayout();
    QHBoxLayout * horizLay2=new QHBoxLayout();
    QHBoxLayout * horizLay3=new QHBoxLayout();

    horizLay->addWidget(drawGrid);
    horizLay->addWidget(isDrawGrid);


    horizLay2->addWidget(drawFace);
    horizLay2->addWidget(isFaceGrid);

    horizLay3->addWidget(lightLabel);
    horizLay3->addWidget(isLightOn);

    horizLay->setContentsMargins(0,0,5,0);
    horizLay->setSpacing(5);
    horizLay->setMargin(0);
    horizLay->setStretch(0,0);

    horizLay2->setContentsMargins(0,0,5,0);
    horizLay2->setSpacing(5);
    horizLay2->setMargin(0);
    horizLay2->setStretch(0,0);

    horizLay3->setContentsMargins(0,0,5,0);
    horizLay3->setSpacing(5);
    horizLay3->setMargin(0);
    horizLay3->setStretch(0,0);

    drawGrid->setAlignment(Qt::AlignTop);
    drawFace->setAlignment(Qt::AlignBottom);
    lightLabel->setAlignment(Qt::AlignTop);



    QVBoxLayout * Vbox=new QVBoxLayout(this);

    Vbox->setContentsMargins(0,0,0,0);
    Vbox->setSpacing(0);
    Vbox->setMargin(0);
    Vbox->setStretch(0,0);

    horizLay->setAlignment(Qt::AlignTop);
    horizLay2->setAlignment(Qt::AlignTop);
    horizLay3->setAlignment(Qt::AlignTop);

    Vbox->addLayout(horizLay,1);
    Vbox->addLayout(horizLay2,7);
    Vbox->addLayout(horizLay3,400);






    ui->setupUi(this);
    openGlViewer = new OpenGlViewer();
    openGlViewer->setFocusPolicy(Qt::FocusPolicy::StrongFocus);


    QMenuBar * menuBar = new QMenuBar(this);


    //menuBar->addMenu(functions);



    mainLayout = new QHBoxLayout();

    mainLayout->setMenuBar(menuBar);
    mainLayout->addWidget(openGlViewer,90);
    mainLayout->addLayout(Vbox,10);

    ui->centralwidget->setLayout(mainLayout);
    ui->centralwidget->show();

    connect(isFaceGrid, &QCheckBox::toggled, this, &MainAppWindow::setDrawFaces);
    connect(isDrawGrid, &QCheckBox::toggled, this, &MainAppWindow::setDrawGrid);
    connect(isLightOn, &QCheckBox::toggled, this, &MainAppWindow::setLight);

}

MainAppWindow::~MainAppWindow()
{
    delete mainLayout;
    delete openGlViewer;
    delete ui;
}




void MainAppWindow::setDrawGrid(bool value)
{
    openGlViewer->setShowGrid(value);
}

void MainAppWindow::setDrawFaces(bool value)
{
    openGlViewer->setShowFaces(value);
}

void MainAppWindow::setLight(bool value)
{
    openGlViewer->setLight(value);
}

