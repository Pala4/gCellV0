#include "csavemodschemesdlg.h"

#include <QListView>
#include <QDialogButtonBox>
#include <QVBoxLayout>

#include "../scheme/cscheme.h"

/*!
  \class CSavedSchemeModel
  */
CSavedSchemeModel::CSavedSchemeModel(QObject *parent) : QAbstractItemModel(parent)
{
	setObjectName("CSavedSchemeModel");
}

int CSavedSchemeModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return 1;
}

int CSavedSchemeModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return m_savedSchemes.count();
}

QModelIndex CSavedSchemeModel::index(int row, int column, const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	if(row < 0 || row >= m_savedSchemes.count()) return QModelIndex();
	return createIndex(row, column, row);
}

QModelIndex CSavedSchemeModel::parent(const QModelIndex &child) const
{
	Q_UNUSED(child)
	return QModelIndex();
}

QVariant CSavedSchemeModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid()) return QVariant();
	if(index.row() < 0 || index.row() >= m_savedSchemes.count()) return QVariant();

	stSavedScheme savedSchemes = m_savedSchemes.at(index.row());
	if(!savedSchemes.scheme) return QVariant();

	switch(role)
	{
		case Qt::DisplayRole: return QVariant(savedSchemes.scheme->fileName());
		case Qt::CheckStateRole: return QVariant(savedSchemes.canBeStored ? Qt::Checked : Qt::Unchecked);
	}

	return QVariant();
}

bool CSavedSchemeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(!index.isValid() || !value.isValid()) return QAbstractItemModel::setData(index, value, role);
	if(index.row() < 0 || index.row() >= m_savedSchemes.count()) return QAbstractItemModel::setData(index, value, role);

	switch(role)
	{
		case Qt::CheckStateRole: m_savedSchemes[index.row()].canBeStored = (value.toInt() == Qt::Checked); return true;
	}

	return QAbstractItemModel::setData(index, value, role);
}

Qt::ItemFlags CSavedSchemeModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags defFlags = QAbstractItemModel::flags(index);

	defFlags |= Qt::ItemIsUserCheckable;

	return defFlags;
}

void CSavedSchemeModel::setSavedSchemes(const QList<stSavedScheme> &savedSchemes)
{
	emit layoutAboutToBeChanged();

	m_savedSchemes = savedSchemes;

	emit layoutChanged();
}

/*!
  \class CSaveModSchemesDlg
  */
CSaveModSchemesDlg::CSaveModSchemesDlg(QWidget *parent) : QDialog(parent)
{
	setObjectName("CSaveModSchemesDlg");

	m_savedSchemesModel = 0;
	m_savedSchemesView = 0;
	m_buttonBox = 0;

	m_savedSchemesModel = new CSavedSchemeModel(this);
	m_savedSchemesModel->setObjectName("savedSchemesModel");

	m_savedSchemesView = new QListView();
	m_savedSchemesView->setObjectName("savedSchemesView");
	m_savedSchemesView->setModel(m_savedSchemesModel);

	m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Discard | QDialogButtonBox::Cancel);
	m_buttonBox->setObjectName("dbbMain");
	connect(m_buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onDlgButtonClicked(QAbstractButton*)));
	connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QVBoxLayout *vblMain = new QVBoxLayout();
	vblMain->setObjectName("vblMain");
	vblMain->setMargin(2);

	vblMain->addWidget(m_savedSchemesView);
	vblMain->addWidget(m_buttonBox);

	setLayout(vblMain);
}

void CSaveModSchemesDlg::setSavedSchemes(const QList<CScheme*> &schemes)
{
	if(!m_savedSchemesModel) return;

	QList<stSavedScheme> savedSchemes;

	foreach(CScheme *scheme, schemes)
	{
		if(!scheme) continue;
		if(!scheme->isModified()) continue;

		stSavedScheme savedScheme = stSavedScheme(scheme);
		savedSchemes << savedScheme;
	}

	m_savedSchemesModel->setSavedSchemes(savedSchemes);
}

const QList<stSavedScheme> CSaveModSchemesDlg::savedSchemes(void) const
{
	QList<stSavedScheme> result;

	if(m_savedSchemesModel) return m_savedSchemesModel->savedSchemes();

	return result;
}

void CSaveModSchemesDlg::onDlgButtonClicked(QAbstractButton *button)
{
	if(!button || !m_buttonBox) return;

	QDialogButtonBox::ButtonRole buttonRole = m_buttonBox->buttonRole(button);
	if(buttonRole == QDialogButtonBox::DestructiveRole) done(CSaveModSchemesDlg::Discarded);
}