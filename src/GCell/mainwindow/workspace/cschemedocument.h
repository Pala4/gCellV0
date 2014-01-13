#ifndef CSCHEMEDOCUMENT_H
#define CSCHEMEDOCUMENT_H

#include "cdocument.h"

class CScheme;

namespace gcell {

namespace scheme {
class CSchemeEditor;
}

namespace mainwindow {
namespace workspace {

class CSchemeDocument : public CDocument
{
	Q_OBJECT
private:
	CScheme *m_scheme;
	scheme::CSchemeEditor *m_schemeEditor;
public:
	explicit CSchemeDocument(QObject *parent = 0);

	virtual IView* view(void);

	CScheme* scheme(void){return m_scheme;}
	scheme::CSchemeEditor* schemeEditor(void){return m_schemeEditor;}
};

}
}
}

#endif // CSCHEMEDOCUMENT_H