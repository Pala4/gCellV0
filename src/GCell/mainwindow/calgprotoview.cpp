#include "calgprotoview.h"

#include <QGridLayout>
#include <QToolButton>
#include <QButtonGroup>

#include "../scheme/algorithmproto/calgorithmproto.h"
#include "../scheme/algorithmproto/calgorithmprotomng.h"

/*!
 * \class CAlgProtoGroupWidget
 */
CAlgProtoGroupWidget::CAlgProtoGroupWidget(QWidget *parent) : QWidget(parent)
{
	setObjectName(QStringLiteral("CAlgProtoGroupWidget"));

	m_columnCount = 3;
	m_currentRow = 0;
	m_currentCol = 0;
	m_buttonsGrid = 0;

	m_buttonsGrid = new QGridLayout();
	m_buttonsGrid->setObjectName(QStringLiteral("actionsGrid"));
	m_buttonsGrid->setSpacing(0);
	m_buttonsGrid->setMargin(0);
	m_buttonsGrid->setContentsMargins(0, 0, 0, 0);
	setLayout(m_buttonsGrid);
}

void CAlgProtoGroupWidget::addAlgProtoButton(QToolButton *algProtoButton)
{
	if(!algProtoButton) return;

	if(m_currentCol < columnCount() - 1)
	{
		++m_currentCol;
	}
	else
	{
		m_currentCol = 0;
		++m_currentRow;
	}

	if(m_buttonsGrid) m_buttonsGrid->addWidget(algProtoButton,
											   m_currentRow, m_currentCol,
											   Qt::AlignLeft | Qt::AlignTop);
}

/*!
 * \class CAlgProtoView
 */
void CAlgProtoView::fill(void)
{
	if(!m_algProtoMng) return;

	foreach(CAlgorithmProto *algProto, m_algProtoMng->algorithmProtos())
	{
		if(!algProto) continue;

		QString groupName = algProto->groupName().isEmpty() ? defaultGroupName() : algProto->groupName();
		CAlgProtoGroupWidget *groupWidget = algProtoGroupWidget(groupName);
		if(!groupWidget)
		{
			groupWidget = new CAlgProtoGroupWidget();
			groupWidget->setObjectName(groupName + QStringLiteral("_groupWgt"));
			addItem(groupWidget, groupName);
			m_algProtoGroupWidgets[groupName] = groupWidget;
		}
		QToolButton *tbAlgProto = new QToolButton();
		tbAlgProto->setObjectName(algProto->name() + QStringLiteral("_tbAlg"));
		tbAlgProto->setText(algProto->name());
		tbAlgProto->setToolButtonStyle(Qt::ToolButtonFollowStyle);
		tbAlgProto->setCheckable(true);
		m_buttonAlgProtoMap[tbAlgProto] = algProto;
		groupWidget->addAlgProtoButton(tbAlgProto);
		if(m_buttonGroup) m_buttonGroup->addButton(tbAlgProto);
	}
}

CAlgProtoGroupWidget* CAlgProtoView::algProtoGroupWidget(const QString &groupName)
{
	if(!m_algProtoGroupWidgets.contains(groupName)) return 0;
	return m_algProtoGroupWidgets[groupName];
}

void CAlgProtoView::init(void)
{
	setObjectName(QStringLiteral("CAlgProtoView"));

	m_defaultGroupName = tr("General");
	m_buttonGroup = 0;
	m_algProtoMng = 0;

	m_buttonGroup = new QButtonGroup(this);
	m_buttonGroup->setObjectName(QStringLiteral("buttonGroup"));
	m_buttonGroup->setExclusive(false);
	connect(m_buttonGroup, SIGNAL(buttonPressed(QAbstractButton*)), this, SLOT(onButtonGroupButtonPressed(QAbstractButton*)));

	setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored));
}

CAlgProtoView::CAlgProtoView(QWidget *parent) : QToolBox(parent)
{
	init();
}

CAlgProtoView::CAlgProtoView(CAlgorithmProtoMng *algProtoMng, QWidget *parent) : QToolBox(parent)
{
	init();

	setAlgProtoMng(algProtoMng);
}

void CAlgProtoView::setAlgProtoMng(CAlgorithmProtoMng *algProtoMng)
{
	if(m_algProtoMng && (m_algProtoMng == algProtoMng)) return;

	if(m_algProtoMng)
	{
		disconnect(m_algProtoMng, SIGNAL(algorithmProtoAdded(CAlgorithmProto*)), this, SLOT(onAlgProtoAdded(CAlgorithmProto*)));
		disconnect(m_algProtoMng, SIGNAL(algorithmProtoRemoved(CAlgorithmProto*)), this, SLOT(onAlgProtoRemoved(CAlgorithmProto*)));
		disconnect(m_algProtoMng, SIGNAL(algorithmProtoSelected(CAlgorithmProto*)), this, SLOT(onAlgProtoProgrammSelected(CAlgorithmProto*)));
		disconnect(m_algProtoMng, SIGNAL(destroyed(QObject*)), this, SLOT(onAlgProtoDestroyed(QObject*)));
		clear();
	}
	m_algProtoMng = algProtoMng;
	if(m_algProtoMng)
	{
		connect(m_algProtoMng, SIGNAL(algorithmProtoAdded(CAlgorithmProto*)), this, SLOT(onAlgProtoAdded(CAlgorithmProto*)));
		connect(m_algProtoMng, SIGNAL(algorithmProtoRemoved(CAlgorithmProto*)), this, SLOT(onAlgProtoRemoved(CAlgorithmProto*)));
		connect(m_algProtoMng, SIGNAL(algorithmProtoSelected(CAlgorithmProto*)), this, SLOT(onAlgProtoProgrammSelected(CAlgorithmProto*)));
		connect(m_algProtoMng, SIGNAL(destroyed(QObject*)), this, SLOT(onAlgProtoDestroyed(QObject*)));
		fill();
	}
}

void CAlgProtoView::onAlgProtoAdded(CAlgorithmProto *algProto)
{
}

void CAlgProtoView::onAlgProtoRemoved(CAlgorithmProto *algProto)
{
}

void CAlgProtoView::onAlgProtoProgrammSelected(CAlgorithmProto *algProto)
{
	if(!algProto)
	{
		releaseChekedButton();
	}
	else
	{
		//
	}
}

void CAlgProtoView::onAlgProtoDestroyed(QObject *objAlgProto)
{
}

void CAlgProtoView::onAlgProtoMngDestroyed(QObject *objAlgProtoMng)
{
	Q_UNUSED(objAlgProtoMng)
	m_algProtoMng = 0;
}

void CAlgProtoView::onButtonGroupButtonPressed(QAbstractButton *button)
{
	if(m_buttonGroup)
	{
		foreach(QAbstractButton *bt, m_buttonGroup->buttons())
		{
			if(bt != button) bt->setChecked(false);
		}
	}

	QToolButton *tb = qobject_cast<QToolButton*>(button);
	if(!tb) return;
	if(!m_buttonAlgProtoMap.contains(tb)) return;
	if(!m_buttonAlgProtoMap[tb]) return;

	if(m_algProtoMng) m_algProtoMng->setSelectedAlgorithmProto(m_buttonAlgProtoMap[tb]);
}

void CAlgProtoView::releaseChekedButton(void)
{
	if(!m_buttonGroup) return;
	if(!m_buttonGroup->checkedButton()) return;

	m_buttonGroup->checkedButton()->setChecked(false);
}

void CAlgProtoView::clear(void)
{
	m_algProtoMng = 0;
}