#ifndef CTHREADTASK_H
#define CTHREADTASK_H

#include <QObject>
#include <QRunnable>

class CScheme;

class CThreadTask : public QObject, public QRunnable
{
    Q_OBJECT
private:
    CScheme *m_pScheme;
public:
    explicit CThreadTask(CScheme *scheme = nullptr);

    CScheme* scheme() {return m_pScheme;}
    void setScheme(CScheme *scheme);

    void run();
signals:
    void taskStarted();
    void taskFunished();
};

#endif // CTHREADTASK_H
