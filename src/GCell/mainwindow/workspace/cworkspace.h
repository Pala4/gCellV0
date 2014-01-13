#ifndef CWORKSPACE_H
#define CWORKSPACE_H

#include <QTabWidget>

namespace gcell {
namespace mainwindow {
namespace workspace {

class CDocument;

class CWorkSpace : public QTabWidget
{
	Q_OBJECT
private:
	QList<CDocument*> m_documents;
public:
	explicit CWorkSpace(QWidget *parent = 0);

	const QList<CDocument*>& documents(void) const{return m_documents;}
private slots:
	void onDocumentDestroyed(QObject *objDocument);
public slots:
	void newDocument(void);
};

}
}
}

#endif // CWORKSPACE_H