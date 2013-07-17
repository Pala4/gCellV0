#include "cschememodel.h"

#include "cgrelement.h"
#include "../../core/scheme/cscheme.h"

CSchemeModel::CSchemeModel(QObject *parent) : QGraphicsScene(parent)
{
    setObjectName(QStringLiteral("CSchemeModel"));

    m_scheme = 0;
}

void CSchemeModel::setScheme(CScheme *scheme)
{
    if(m_scheme && (m_scheme == scheme)) return;

    if(m_scheme)
    {
        clear();
        disconnect(m_scheme, SIGNAL(elementAdded(CElement*)), this, SLOT(addElement(CElement*)));
        disconnect(m_scheme, SIGNAL(destroyed()), this, SLOT(onSchemeDestroyed()));
    }

    m_scheme = scheme;

    if(m_scheme)
    {
        foreach(CElement *element, m_scheme->elements())
        {
            if(!element) continue;
            addElement(element);
        }
        connect(m_scheme, SIGNAL(elementAdded(CElement*)), this, SLOT(addElement(CElement*)));
        connect(m_scheme, SIGNAL(destroyed()), this, SLOT(onSchemeDestroyed()));
    }
}

void CSchemeModel::onSchemeDestroyed(void)
{
    clear();
    m_scheme = 0;
}

void CSchemeModel::addElement(CElement *element)
{
    if(!element) return;

    CGrElement *grElement = new CGrElement(element);
	addItem(grElement);
}