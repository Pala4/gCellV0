#include "cargument.h"

CArgument::CArgument(QGraphicsItem *parent) : CPortal(parent)
{
	setObjectName(QStringLiteral("CArgument"));
	setDefaultName(tr("argument"));
}