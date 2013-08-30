#ifndef CCOLORLINEEDIT_H
#define CCOLORLINEEDIT_H

#include "clineeditbutton.h"

class CColorLineEdit : public CLineEditButton
{
	Q_OBJECT
public:
	explicit CColorLineEdit(QWidget *parent = 0);

	QColor color(void) const;
	void setColor(const QColor &color);
private slots:
	void onButtonClicked(void);
};

#endif // CCOLORLINEEDIT_H