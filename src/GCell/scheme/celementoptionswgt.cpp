#include "celementoptionswgt.h"

#include <QVBoxLayout>
#include <QTabWidget>
#include <QTabBar>
#include <QGridLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>

#include "celement.h"

CElementOptionsWgt::CElementOptionsWgt(CElement *element, QWidget *parent) : QDialog(parent)
{
	setObjectName("CElementOptionsWgt");

	m_tabWidget = 0;
	m_generalLayout = 0;
	m_captionEdit = 0;
	m_element = 0;

	m_element = element;

	QVBoxLayout *mainLayout = new QVBoxLayout();
	mainLayout->setObjectName(QStringLiteral("mainLayout"));
	setLayout(mainLayout);

	m_tabWidget = new QTabWidget(this);
	m_tabWidget->setObjectName(QStringLiteral("tabWidget"));
	m_tabWidget->tabBar()->setVisible(false);
	mainLayout->addWidget(m_tabWidget);

	m_generalLayout = new QGridLayout();
	m_generalLayout->setObjectName(QStringLiteral("generalLayout"));

	m_generalFormLayout = new QFormLayout();
	m_generalFormLayout->setObjectName(QStringLiteral("generalFormLayout"));
	m_generalLayout->addLayout(m_generalFormLayout, 0, 0, Qt::AlignCenter);

	m_captionEdit = new QLineEdit(this);
	m_captionEdit->setObjectName(QStringLiteral("captionEdit"));
	m_generalFormLayout->addRow(tr("&Caption:"), m_captionEdit);

	QWidget *generalTab = new QWidget(this);
	generalTab->setObjectName(QStringLiteral("generalTab"));
	generalTab->setWindowTitle(tr("General"));
	generalTab->setLayout(m_generalLayout);
	m_tabWidget->addTab(generalTab, generalTab->windowTitle());

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
													   QDialogButtonBox::Cancel,
													   Qt::Horizontal);
	buttonBox->setObjectName(QStringLiteral("buttonBox"));
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	mainLayout->addWidget(buttonBox);

	QString title = "options";
	if(m_element)
	{
		title = m_element->caption() + " " + title;
		m_captionEdit->setText(m_element->caption());
	}
	setWindowTitle(title);
}

QString CElementOptionsWgt::elementCaption(void)
{
	return m_captionEdit ? m_captionEdit->text() : QString();
}