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






void CrowdEngine::loadBrain(std::string _brain)
{

}

void CrowdEngine::addAgent(Agent *_agent)
{

}

void CrowdEngine::addAgents(std::vector<Agent *> _agents)
{

}

void CrowdEngine::update()
{

}

void CrowdEngine::clear()
{

}

void CrowdEngine::restart()
{

}
