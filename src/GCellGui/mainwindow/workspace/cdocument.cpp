#include "cdocument.h"

using namespace gcell;
using namespace mainwindow;
using namespace workspace;

CDocument::CDocument(QObject *parent) : QObject(parent)
{
	setObjectName(QStringLiteral("CDocument"));
}