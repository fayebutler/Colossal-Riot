#ifndef CROWDENGINE_H
#define CROWDENGINE_H

#include <QMainWindow>
#include <cstdlib>
#include <set>


namespace Ui {
class CrowdEngine;
}

class CrowdEngine : public QMainWindow
{
    Q_OBJECT

public:
    explicit CrowdEngine(QWidget *parent = 0);
    ~CrowdEngine();
    void loadBrain(std::string _brain);
    void addAgent(Agent * _agent);
    void addAgents(std::vector<Agent*> _agents);
    void update();
    void clear();
    void restart();


private:
    Ui::CrowdEngine *ui;
    std::string s_brainsPath;
    std::vector<std::string> s_loadedBrains;




};

#endif // CROWDENGINE_H
