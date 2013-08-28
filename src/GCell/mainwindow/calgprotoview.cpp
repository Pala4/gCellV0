#include "calgprotoview.h"

#include <QBoxLayout>
#include <QPushButton>
#include <QButtonGroup>

#include "../scheme/algorithmproto/calgorithmproto.h"
#include "../scheme/algorithmproto/calgorithmprotomng.h"

/*!
 * \class CAlgProtoGroupWidget
 */
CAlgProtoGroupWidget::CAlgProtoGroupWidget(const Qt::Orientation &orientation, QWidget *parent) : QWidget(parent)
{
	setObjectName(QStringLiteral("CAlgProtoGroupWidget"));

	m_buttonsLayout = 0;
	m_orientation = Qt::Vertical;
	QBoxLayout::Direction dir = QBoxLayout::TopToBottom;
	if(m_orientation == Qt::Horizontal) dir = QBoxLayout::LeftToRight;

	m_buttonsLayout = new QBoxLayout(dir);
	m_buttonsLayout->setObjectName(QStringLiteral("buttonsLayout"));
	m_buttonsLayout->setSpacing(0);
	m_buttonsLayout->setMargin(0);
	m_buttonsLayout->setContentsMargins(0, 0, 0, 0);
	m_buttonsLayout->addStretch();
	setLayout(m_buttonsLayout);

	setOrientation(orientation);
}

void CAlgProtoGroupWidget::setOrientation(const Qt::Orientation &orientation)
{
	m_orientation = orientation;
	if(m_orientation == Qt::Vertical)
	{
		if(m_buttonsLayout) m_buttonsLayout->setDirection(QBoxLayout::TopToBottom);
	}
	else if(m_orientation == Qt::Horizontal)
	{
		if(m_buttonsLayout) m_buttonsLayout->setDirection(QBoxLayout::LeftToRight);
	}
}

void CAlgProtoGroupWidget::addAlgProtoButton(QPushButton *algProtoButton)
{
	if(!algProtoButton) return;

	if(m_buttonsLayout) m_buttonsLayout->insertWidget(m_buttonsLayout->count() - 1, algProtoButton, 0, Qt::AlignCenter);
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
			addTab(groupWidget, groupName);
			m_algProtoGroupWidgets[groupName] = groupWidget;
		}
		QPushButton *pbAlgProto = new QPushButton();
		pbAlgProto->setObjectName(algProto->name() + QStringLiteral("_tbAlg"));
		pbAlgProto->setText(algProto->name());
//		pbAlgProto->setFlat(true);
		pbAlgProto->setCheckable(true);
		m_buttonAlgProtoMap[pbAlgProto] = algProto;
		groupWidget->addAlgProtoButton(pbAlgProto);
		if(m_buttonGroup) m_buttonGroup->addButton(pbAlgProto);
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
	m_orientation = Qt::Vertical;
	m_verTabPosition = QTabWidget::East;
	m_horTabPosition = QTabWidget::North;
	m_algProtoMng = 0;

	m_buttonGroup = new QButtonGroup(this);
	m_buttonGroup->setObjectName(QStringLiteral("buttonGroup"));
	m_buttonGroup->setExclusive(false);
	connect(m_buttonGroup, SIGNAL(buttonPressed(QAbstractButton*)), this, SLOT(onButtonGroupButtonPressed(QAbstractButton*)));

	setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored));
}

CAlgProtoView::CAlgProtoView(QWidget *parent) : QTabWidget(parent)
{
	init();

	setOrientation(m_orientation);
}

CAlgProtoView::CAlgProtoView(CAlgorithmProtoMng *algProtoMng, const Qt::Orientation &orientation, QWidget *parent) : QTabWidget(parent)
{
	init();

	setOrientation(orientation);
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

void CAlgProtoView::setOrientation(const Qt::Orientation &orientation)
{
	m_orientation = orientation;
	for(int ci = 0; ci < m_algProtoGroupWidgets.values().count(); ++ci)
	{
		CAlgProtoGroupWidget *algProtoWgt = m_algProtoGroupWidgets.values().at(ci);
		if(algProtoWgt) algProtoWgt->setOrientation(m_orientation);
	}
	if(m_orientation == Qt::Vertical)
	{
		setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored));
		setTabPosition(m_verTabPosition);
	}
	else if(m_orientation == Qt::Horizontal)
	{
		setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed));
		setTabPosition(m_horTabPosition);
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

	QPushButton *tb = qobject_cast<QPushButton*>(button);
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