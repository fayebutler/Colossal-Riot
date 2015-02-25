#ifndef CROWDENGINE_H
#define CROWDENGINE_H

#include <QMainWindow>

namespace Ui {
class CrowdEngine;
}

class CrowdEngine : public QMainWindow
{
    Q_OBJECT

public:
    explicit CrowdEngine(QWidget *parent = 0);
    ~CrowdEngine();

private:
    Ui::CrowdEngine *ui;
};

#endif // CROWDENGINE_H
