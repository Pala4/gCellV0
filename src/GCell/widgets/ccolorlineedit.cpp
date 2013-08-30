#include "ccolorlineedit.h"

#include <QColorDialog>

/*!
 * \class CColorLineEdit
 */
CColorLineEdit::CColorLineEdit(QWidget *parent) : CLineEditButton(parent)
{
	setObjectName(QStringLiteral("CColorLineEdit"));

	connect(this, SIGNAL(buttonClicked()), this, SLOT(onButtonClicked()));
}

QColor CColorLineEdit::color(void) const
{
	return QColor(text());
}

void CColorLineEdit::setColor(const QColor &color)
{
	setText(color.name());
}

void CColorLineEdit::onButtonClicked(void)
{
	QColorDialog colorDlg(color(), this);
	int dlgResult = colorDlg.exec();
	switch(dlgResult)
	{
		case QDialog::Accepted:
			setColor(colorDlg.currentColor());
		break;
		case QDialog::Rejected:break;
	}
}