#include "include/MainWindow.h"
#include "ui_Mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), m_ui(new Ui::MainWindow)

{
    m_ui->setupUi(this);
    // create an openGL format and pass to the new GLWidget
    QGLFormat format;
    format.setVersion(4,1);
    format.setProfile( QGLFormat::CoreProfile);

    m_gl=new GLWindow(format,this);
    //add our GLWindow to the Qt grid layout and set the size.
    m_ui->s_mainWindowGridLayout->addWidget(m_gl,0,0,2,1);

    //Connections from UI and NGL viewport
    connect(m_ui->m_wireframe,SIGNAL(toggled(bool)),m_gl,SLOT(toggleWireframe(bool)));
    connect(m_ui->m_objectSelection,SIGNAL(currentIndexChanged(int)),m_gl,SLOT(setObjectMode(int)));



}

MainWindow::~MainWindow()
{
    delete m_ui;
}
