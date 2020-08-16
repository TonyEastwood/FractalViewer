#ifndef MAINAPPWINDOW_H
#define MAINAPPWINDOW_H

#include <QMainWindow>
#include "OpenGlViewer/openglviewer.h"
#include "Design.h"
#include <QVBoxLayout>
#include <QMenuBar>
#include <QLabel>
#include <QMenu>
#include <QDialog>
#include <QFileDialog>
#include <QPushButton>
#include <QCheckBox>
#include <QStyleFactory>
namespace Ui {
class MainAppWindow;
}

class MainAppWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainAppWindow(  QWidget *parent = nullptr);
    ~MainAppWindow();
public slots:

    void setDrawGrid(bool value);
    void setDrawFaces(bool value);
    void setLight(bool value);

private:
    Ui::MainAppWindow *ui;
    OpenGlViewer *openGlViewer; // openGl viewer
    QHBoxLayout *mainLayout;

};

#endif // MAINAPPWINDOW_H
