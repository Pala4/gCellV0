#include "cworkspace.h"

#include "cschemedocument.h"
#include "iview.h"

using namespace gcell;
using namespace mainwindow;
using namespace workspace;

CWorkSpace::CWorkSpace(QWidget *parent) : QTabWidget(parent)
{
	setObjectName(QStringLiteral("CWorkSpace"));
}

void CWorkSpace::onDocumentDestroyed(QObject *objDocument)
{
	if(!m_documents.contains((CDocument*)objDocument)) return;
	m_documents.removeOne((CDocument*)objDocument);
}

void CWorkSpace::newDocument(void)
{
	CDocument *document = new CSchemeDocument(this);
	connect(document, SIGNAL(destroyed(QObject*)), this, SLOT(onDocumentDestroyed(QObject*)));
	m_documents << document;

	QWidget *viewWidget = document->view()->widget();
	if(viewWidget) addTab(viewWidget, viewWidget->windowTitle());
}