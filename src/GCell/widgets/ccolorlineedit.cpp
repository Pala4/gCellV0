#include "ccolorlineedit.h"

#include <QLineEdit>
#include <QColorDialog>
#include <QRegExpValidator>

/*!
 * \class CColorLineEdit
 */
CColorLineEdit::CColorLineEdit(QWidget *parent) : CLineEditButton(parent)
{
	setObjectName(QStringLiteral("CColorLineEdit"));

	connect(this, SIGNAL(buttonClicked()), this, SLOT(onButtonClicked()));

	QRegExpValidator *regExpValid = new QRegExpValidator(QRegExp("^#([0-9A-Fa-f]){3,6}$"));
	if(lineEdit())
	{
		lineEdit()->setInputMask("\\#HHHHHH");
		lineEdit()->setValidator(regExpValid);
	}
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