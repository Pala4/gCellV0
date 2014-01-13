#ifndef IVIEW_H
#define IVIEW_H

class QWidget;

namespace gcell {
namespace mainwindow {
namespace workspace {

class IView
{
public:
	explicit IView(void);
	virtual ~IView(void){}

	QWidget* widget(void);
};

}
}
}

#endif // IVIEW_H