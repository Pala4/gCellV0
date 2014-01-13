#ifndef CDOCUMENT_H
#define CDOCUMENT_H

#include <QObject>

namespace gcell {
namespace mainwindow {
namespace workspace {

class IView;

class CDocument : public QObject
{
	Q_OBJECT
public:
	explicit CDocument(QObject *parent = 0);

	virtual IView* view(void) = 0;
};

}
}
}

#endif // CDOCUMENT_H