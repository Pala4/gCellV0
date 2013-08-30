#include "clineeditbutton.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

CLineEditButton::CLineEditButton(QWidget *parent) : QWidget(parent)
{
	setObjectName(QStringLiteral("CLineEditButton"));

	m_lineEdit = 0;
	m_pushButton = 0;

	QHBoxLayout *hbl = new QHBoxLayout();
	hbl->setSpacing(0);
	hbl->setMargin(0);
	hbl->setContentsMargins(0, 0, 0, 0);
	setLayout(hbl);

	m_lineEdit = new QLineEdit();
	m_lineEdit->setObjectName(QStringLiteral("lineEdit"));
	hbl->addWidget(m_lineEdit, 1);

	m_pushButton = new QPushButton(QStringLiteral("..."));
	m_pushButton->setObjectName(QStringLiteral("pushButton"));
	m_pushButton->setMinimumSize(m_lineEdit->sizeHint().height(), m_lineEdit->sizeHint().height());
	m_pushButton->setMaximumSize(m_pushButton->minimumSize());
	connect(m_pushButton, SIGNAL(clicked()), this, SIGNAL(buttonClicked()));
	hbl->addWidget(m_pushButton);
}

QString CLineEditButton::text(void) const
{
	if(m_lineEdit) return m_lineEdit->text();
	return QString();
}

void CLineEditButton::setText(const QString &text)
{
	if(m_lineEdit) m_lineEdit->setText(text);
}