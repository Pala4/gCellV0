#ifndef CTHREADENGINE_H
#define CTHREADENGINE_H

#include <QObject>

class CScheme;

class CThreadEngine : public QObject
{
    Q_OBJECT
private:
    int m_nActiveTasksCount;
public:
    explicit CThreadEngine(QObject *parent = 0);
private slots:
    void onTaskStarted();
    void onTaskFinished();
public slots:
    void calcScheme(CScheme *scheme);
    void calcSchemes(const QList<CScheme*> &schemes);
signals:
    void calcStarted();
    void calcFinished();
};

#endif // CTHREADENGINE_H