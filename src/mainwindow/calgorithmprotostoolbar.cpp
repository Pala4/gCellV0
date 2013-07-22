#include "calgorithmprotostoolbar.h"

#include <QActionGroup>

#include "../scheme/calgorithmproto.h"
#include "../scheme/calgorithmprotomng.h"

/*!
 * \class CElementProtosToolBar
 */
void CAlgorithmProtosToolBar::addActionToGroup(QAction *action)
{
    if(!action) return;

    action->setCheckable(true);
    action->setActionGroup(m_actionGroup);
}

CAlgorithmProtosToolBar::CAlgorithmProtosToolBar(CAlgorithmProtoMng *algorithmProtoMng, QWidget *parent) : QToolBar(parent)
{
	setObjectName(QStringLiteral("CAlgorithmProtosToolBar"));

    m_blockOnProtoSelected = false;
    m_blockOnManaginActionTriggered = false;

    m_actionGroup = 0;
	m_algorithmProtoMng = 0;
    m_managinSeparator = 0;
    m_defaultAction = 0;

    m_actionGroup = new QActionGroup(this);
    m_actionGroup->setObjectName(QStringLiteral("actionGroup"));
    m_actionGroup->setExclusive(true);

    m_managinSeparator = addSeparator();
    m_managinSeparator->setVisible(false);

	setAlgorithmProtoMng(algorithmProtoMng);
}

void CAlgorithmProtosToolBar::setAlgorithmProtoMng(CAlgorithmProtoMng *algorithmProtoMng)
{
	if(m_algorithmProtoMng && (m_algorithmProtoMng == algorithmProtoMng)) return;

	if(m_algorithmProtoMng)
    {
		disconnect(m_algorithmProtoMng, SIGNAL(algorithmProtoAdded(CAlgorithmProto*)), this, SLOT(addProto(CAlgorithmProto*)));
		disconnect(m_algorithmProtoMng, SIGNAL(algorithmProtoRemoved(CAlgorithmProto*)), this, SLOT(removeProto(CAlgorithmProto*)));
		disconnect(m_algorithmProtoMng, SIGNAL(algorithmProtoSelected(CAlgorithmProto*)), this, SLOT(onProtoSelected(CAlgorithmProto*)));
		foreach(CAlgorithmProto *algorithmProto, m_algorithmProtoMng->algorithmProtos())
        {
			if(!algorithmProto) continue;
			removeProto(algorithmProto);
        }
    }

	m_algorithmProtoMng = algorithmProtoMng;

	if(m_algorithmProtoMng)
    {
		connect(m_algorithmProtoMng, SIGNAL(algorithmProtoAdded(CAlgorithmProto*)), this, SLOT(addProto(CAlgorithmProto*)));
		connect(m_algorithmProtoMng, SIGNAL(algorithmProtoRemoved(CAlgorithmProto*)), this, SLOT(removeProto(CAlgorithmProto*)));
		connect(m_algorithmProtoMng, SIGNAL(algorithmProtoSelected(CAlgorithmProto*)), this, SLOT(onProtoSelected(CAlgorithmProto*)));
		foreach(CAlgorithmProto *algorithmProto, m_algorithmProtoMng->algorithmProtos())
		{
			if(!algorithmProto) continue;
			addProto(algorithmProto);
		}
	}
}

void CAlgorithmProtosToolBar::addManaginAction(QAction *action)
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

void CAlgorithmProtosToolBar::onManaginActionTriggered(const bool &checked)
{
    if(m_blockOnManaginActionTriggered)
    {
        m_blockOnManaginActionTriggered = false;
        return;
    }

	if(m_algorithmProtoMng)
    {
        m_blockOnProtoSelected = true;
		if(checked) m_algorithmProtoMng->setSelectedAlgorithmProto(0);
    }
}

void CAlgorithmProtosToolBar::onProtoActionTriggered(const bool &checked)
{
    QAction *action = qobject_cast<QAction*>(sender());
    if(!action) return;
    if(!m_protoActions.contains(action)) return;
	if(m_algorithmProtoMng)
    {
        m_blockOnProtoSelected = true;
		m_algorithmProtoMng->setSelectedAlgorithmProto(checked ? m_protoActions[action] : 0);
    }
}

void CAlgorithmProtosToolBar::onProtoSelected(CAlgorithmProto *algorithmProto)
{
    if(m_blockOnProtoSelected)
    {
        m_blockOnProtoSelected = false;
        return;
    }

	if(!algorithmProto)
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
		QAction *protoAction = m_protoActions.key(algorithmProto, 0);
        if(protoAction) protoAction->setChecked(true);
    }
}

void CAlgorithmProtosToolBar::addProto(CAlgorithmProto *algorithmProto)
{
	if(!algorithmProto) return;
	if(m_protoActions.values().contains(algorithmProto)) return;

	QAction *acProto = new QAction(algorithmProto->name(), this);
	acProto->setObjectName(QStringLiteral("acProto_%1").arg(algorithmProto->name()));
    connect(acProto, SIGNAL(triggered(bool)), this, SLOT(onProtoActionTriggered(bool)));
    addActionToGroup(acProto);
    addAction(acProto);
	m_protoActions[acProto] = algorithmProto;
}

void CAlgorithmProtosToolBar::removeProto(CAlgorithmProto *algorithmProto)
{
	if(!algorithmProto) return;
	if(!m_protoActions.values().contains(algorithmProto)) return;

	QAction *acProto = m_protoActions.key(algorithmProto, 0);
    if(!acProto) return;
    m_protoActions.remove(acProto);
    m_actionGroup->removeAction(acProto);
    removeAction(acProto);
    acProto->deleteLater();
}