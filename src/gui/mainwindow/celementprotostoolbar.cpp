#include "celementprotostoolbar.h"

#include <QActionGroup>

#include "../schemeview/celementprotomng.h"
#include "../schemeview/celementproto.h"

/*!
 * \class CElementProtosToolBar
 */
void CElementProtosToolBar::addActionToGroup(QAction *action)
{
    if(!action) return;

    action->setCheckable(true);
    action->setActionGroup(m_actionGroup);
}

CElementProtosToolBar::CElementProtosToolBar(CElementProtoMng *elementProtoMng, QWidget *parent) : QToolBar(parent)
{
    setObjectName(QStringLiteral("CElementProtosToolBar"));

    m_blockOnProtoSelected = false;
    m_blockOnManaginActionTriggered = false;

    m_actionGroup = 0;
    m_elementProtoMng = 0;
    m_managinSeparator = 0;
    m_defaultAction = 0;

    m_actionGroup = new QActionGroup(this);
    m_actionGroup->setObjectName(QStringLiteral("actionGroup"));
    m_actionGroup->setExclusive(true);

    m_managinSeparator = addSeparator();
    m_managinSeparator->setVisible(false);

    setElementProtoMng(elementProtoMng);
}

void CElementProtosToolBar::setElementProtoMng(CElementProtoMng *elementProtoMng)
{
    if(m_elementProtoMng && (m_elementProtoMng == elementProtoMng)) return;

    if(m_elementProtoMng)
    {
        disconnect(m_elementProtoMng, SIGNAL(elementProtoAdded(CElementProto*)), this, SLOT(addProto(CElementProto*)));
        disconnect(m_elementProtoMng, SIGNAL(elementProtoRemoved(CElementProto*)), this, SLOT(removeProto(CElementProto*)));
        disconnect(m_elementProtoMng, SIGNAL(elementProtoSelected(CElementProto*)), this, SLOT(onProtoSelected(CElementProto*)));
        foreach(CElementProto *elementProto, m_elementProtoMng->elementProtos())
        {
            if(!elementProto) continue;
            removeProto(elementProto);
        }
    }

    m_elementProtoMng = elementProtoMng;

    if(m_elementProtoMng)
    {
        connect(m_elementProtoMng, SIGNAL(elementProtoAdded(CElementProto*)), this, SLOT(addProto(CElementProto*)));
        connect(m_elementProtoMng, SIGNAL(elementProtoRemoved(CElementProto*)), this, SLOT(removeProto(CElementProto*)));
        connect(m_elementProtoMng, SIGNAL(elementProtoSelected(CElementProto*)), this, SLOT(onProtoSelected(CElementProto*)));
        foreach(CElementProto *elementProto, m_elementProtoMng->elementProtos())
        {
            if(!elementProto) continue;
            addProto(elementProto);
        }
    }
}

void CElementProtosToolBar::addManaginAction(QAction *action)
{
    if(!action) return;
    addActionToGroup(action);
    if(m_managinSeparator)
    {
        insertAction(m_managinSeparator, action);
        m_managinSeparator->setVisible(true);
    }
    else
    {
        addAction(action);
    }
    connect(action, SIGNAL(triggered(bool)), this, SLOT(onManaginActionTriggered(bool)));
}

void CElementProtosToolBar::onManaginActionTriggered(const bool &checked)
{
    if(m_blockOnManaginActionTriggered)
    {
        m_blockOnManaginActionTriggered = false;
        return;
    }

    if(m_elementProtoMng)
    {
        m_blockOnProtoSelected = true;
        if(checked) m_elementProtoMng->setSelectedElementProto(0);
    }
}

void CElementProtosToolBar::onProtoActionTriggered(const bool &checked)
{
    QAction *action = qobject_cast<QAction*>(sender());
    if(!action) return;
    if(!m_protoActions.contains(action)) return;
    if(m_elementProtoMng)
    {
        m_blockOnProtoSelected = true;
        m_elementProtoMng->setSelectedElementProto(checked ? m_protoActions[action] : 0);
    }
}

void CElementProtosToolBar::onProtoSelected(CElementProto *elementProto)
{
    if(m_blockOnProtoSelected)
    {
        m_blockOnProtoSelected = false;
        return;
    }

    if(!elementProto)
    {
        if(m_actionGroup && !m_defaultAction)
        {
            QAction *checkedAction = m_actionGroup->checkedAction();
            if(checkedAction && m_protoActions.contains(checkedAction))
            {
                checkedAction->setChecked(false);
            }
        }
        if(m_defaultAction)
        {
            m_blockOnManaginActionTriggered = true;
            m_defaultAction->trigger();
        }
    }
    else
    {
        QAction *protoAction = m_protoActions.key(elementProto, 0);
        if(protoAction) protoAction->setChecked(true);
    }
}

void CElementProtosToolBar::addProto(CElementProto *elementProto)
{
    if(!elementProto) return;
    if(m_protoActions.values().contains(elementProto)) return;

    QAction *acProto = new QAction(elementProto->name(), this);
    acProto->setObjectName(QStringLiteral("acProto_%1").arg(elementProto->id()));
    connect(acProto, SIGNAL(triggered(bool)), this, SLOT(onProtoActionTriggered(bool)));
    addActionToGroup(acProto);
    addAction(acProto);
    m_protoActions[acProto] = elementProto;
}

void CElementProtosToolBar::removeProto(CElementProto *elementProto)
{
    if(!elementProto) return;
    if(!m_protoActions.values().contains(elementProto)) return;

    QAction *acProto = m_protoActions.key(elementProto, 0);
    if(!acProto) return;
    m_protoActions.remove(acProto);
    m_actionGroup->removeAction(acProto);
    removeAction(acProto);
    acProto->deleteLater();
}