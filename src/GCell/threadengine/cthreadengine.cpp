#include "cthreadengine.h"

#include <QThreadPool>

#include "cthreadtask.h"

CThreadEngine::CThreadEngine(QObject *parent) : QObject(parent)
{
    setObjectName(QStringLiteral("CThreadEngine"));

    m_nActiveTasksCount = 0;
}

void CThreadEngine::onTaskStarted()
{
    if (m_nActiveTasksCount == 0)
        emit calcStarted();

    ++m_nActiveTasksCount;
}

void CThreadEngine::onTaskFinished()
{
    --m_nActiveTasksCount;

    if (m_nActiveTasksCount == 0)
        emit calcFinished();
}

void CThreadEngine::calcScheme(CScheme *scheme)
{
    if (scheme == nullptr)
        return;

    CThreadTask *task = new CThreadTask(scheme);
    connect(task, SIGNAL(taskStarted()), this, SLOT(onTaskStarted()));
    connect(task, SIGNAL(taskFunished()), this, SLOT(onTaskFinished()));
    QThreadPool::globalInstance()->start(task);
}

void CThreadEngine::calcSchemes(const QList<CScheme*> &schemes)
{
    foreach (CScheme *scheme, schemes)
        calcScheme(scheme);
}