#include "cschemedocument.h"

#include "cscheme.h"
#include "cschemeeditor.h"

using namespace gcell;
using namespace mainwindow;
using namespace workspace;

CSchemeDocument::CSchemeDocument(QObject *parent) : CDocument(parent)
{
	setObjectName(QStringLiteral("CSchemeDocument"));

	m_scheme = new CScheme(this);
	m_scheme->setObjectName(QStringLiteral("scheme"));
	m_scheme->setSceneRect(0.0, 0.0, 2000.0, 2000.0);
	m_scheme->setNewScheme(true);
	connect(m_scheme, SIGNAL(destroyed()), this, SLOT(deleteLater()));

//	m_scheme->setAlgorithmProtoMng(m_algorithmProtoMng);

	m_schemeEditor = new scheme::CSchemeEditor();
	m_schemeEditor->setObjectName(QStringLiteral("schemeEditor"));
	m_schemeEditor->setScheme(m_scheme);
	connect(m_schemeEditor, SIGNAL(destroyed()), this, SLOT(deleteLater()));
	connect(this, SIGNAL(destroyed()), m_schemeEditor, SLOT(deleteLater()));

//	m_schemeEditor->setupGrid(gridColor(), gridBkGndColor(), gridStep(), gridPointSize(), isGridAlign());
//	if(m_schemeEditorContextMenu) m_schemeEditor->setContextMenu(m_schemeEditorContextMenu);
//	connect(m_schemeEditor, SIGNAL(addAlgorithmModeFinished()), this, SLOT(onSchemeEditorAddAlgorithmModeFinished()));
//	connect(m_schemeEditor, SIGNAL(windowTitleChanged()), this, SLOT(onSchemeEditorWindowTitleChanged()));
//	connect(m_schemeEditor, SIGNAL(elementsSelected(QList<CElement*>)), this, SLOT(onSchemeEditorElementsSelected(QList<CElement*>)));
//	if(m_workSpaceTabWgt) m_workSpaceTabWgt->addTab(m_schemeEditor, m_schemeEditor->windowTitle());

}

IView* CSchemeDocument::view(void)
{
	return m_schemeEditor;
}