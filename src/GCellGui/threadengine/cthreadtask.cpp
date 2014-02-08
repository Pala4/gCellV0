#include "cthreadtask.h"

#include "cscheme.h"
#include "cengine.h"

//Until this class is not thread safety!!!

CThreadTask::CThreadTask(CScheme *scheme) : QObject(nullptr), QRunnable()
{
    m_pScheme = scheme;
}

void CThreadTask::setScheme(CScheme *scheme)
{
    if ((m_pScheme != nullptr) && (m_pScheme == scheme))
        return;

    m_pScheme = scheme;
}

void CThreadTask::run()
{
    emit taskStarted();

    CEngine *engine = new CEngine();
    engine->calc(m_pScheme);
    delete engine;

    emit taskFunished();
}