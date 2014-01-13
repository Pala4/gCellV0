#include "iview.h"

#include <QWidget>

using namespace gcell;
using namespace mainwindow;
using namespace workspace;

IView::IView(void)
{
}

QWidget* IView::widget(void)
{
	return dynamic_cast<QWidget*>(this);
}