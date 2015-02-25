#include "crowdengine.h"
#include "ui_crowdengine.h"

CrowdEngine::CrowdEngine(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CrowdEngine)
{
    ui->setupUi(this);
}

CrowdEngine::~CrowdEngine()
{
    delete ui;
}
