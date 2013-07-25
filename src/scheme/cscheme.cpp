#include "cscheme.h"

#include <QAction>

#include "calgorithmproto.h"
#include "calgorithmprotomng.h"
#include "calgorithm.h"
#include "cportal.h"
#include "clink.h"
#include "elementlistutil.h"

CScheme::CScheme(QObject *parent) : QGraphicsScene(parent)
{
	setObjectName(QStringLiteral("CScheme"));

	m_acDeleteSelected = 0;
    m_algProtoMng = 0;

	connect(this, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));

	m_acDeleteSelected = new QAction(tr("Delete selected"), this);
	m_acDeleteSelected->setObjectName(QStringLiteral("acDeleteSelected"));
	m_acDeleteSelected->setDisabled(true);
	connect(m_acDeleteSelected, SIGNAL(triggered()), this, SLOT(deleteSelected()));
    addAction(m_acDeleteSelected);
}

void CScheme::addAction(QAction *action)
{
	if(!action) return;
	if(m_actions.contains(action)) return;

    m_actions << action;
}

CElement* CScheme::element(const QString &id)
{
    QList<CElement*> elements = getElements<CElement*, QGraphicsItem*>(items());
    foreach(CElement *element, elements)
    {
        if(element && element->id() == id) return element;
    }
    return 0;
}

void CScheme::onSelectionChanged(void)
{
    if(m_acDeleteSelected) m_acDeleteSelected->setDisabled(selectedItems().isEmpty());
}

void CScheme::createAlgorithm(const QPointF &pos)
{
    if(!m_algProtoMng) return;
    addAlgorithm(m_algProtoMng->selectedAlgorithmProto(), pos);
}

void CScheme::addAlgorithm(CAlgorithmProto *algorithmProto, const QPointF &pos)
{
	if(!algorithmProto) return;

	CAlgorithm *algorithm = algorithmProto->createAlgorithm();
	addAlgorithm(algorithm, pos);
}

void CScheme::addAlgorithm(CAlgorithm *algorithm, const QPointF &pos)
{
	if(!algorithm) return;

	algorithm->setNomber(generateNomber<CAlgorithm*, CAlgorithm*>(getElements<CAlgorithm*, QGraphicsItem*>(items(), algorithm->typeID())));
	algorithm->setPos(pos);
	addItem(algorithm);
}

void CScheme::addLink(CPortal *firstPortal, CPortal *secondPortal)
{
	if(!firstPortal || !secondPortal) return;

	CLink *link = new CLink(0);
	firstPortal->addLink(link);
	secondPortal->addLink(link);
	link->setResult(firstPortal);
	link->setArgument(secondPortal);
	link->setNomber(generateNomber<CLink*, CLink*>(getElements<CLink*, QGraphicsItem*>(items())));
	addItem(link);
	link->updateGeometry();
}

void CScheme::deleteSelected(void)
{
	QList<QGraphicsItem*> selItems = selectedItems();
	foreach(QGraphicsItem *selItem, selItems)
	{
		CElement *element = dynamic_cast<CElement*>(selItem);
		if(element)
		{
			element->deleteLater();
		}
		else
		{
			delete selItem;
		}
	}
}