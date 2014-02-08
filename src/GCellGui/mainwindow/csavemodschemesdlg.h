#ifndef CSAVEMODSCHEMESDLG_H
#define CSAVEMODSCHEMESDLG_H

#include <QDialog>
#include <QAbstractItemModel>

class QListView;
class QAbstractButton;
class QDialogButtonBox;

class CScheme;

struct stSavedScheme
{
	CScheme *scheme;
	bool canBeStored;
	stSavedScheme(CScheme *_scheme) : scheme(_scheme), canBeStored(true){}
};

class CSavedSchemeModel : public QAbstractItemModel
{
	Q_OBJECT
private:
	QList<stSavedScheme> m_savedSchemes;
public:
	explicit CSavedSchemeModel(QObject *parent = 0);

	virtual int columnCount(const QModelIndex &parent) const;
	virtual int rowCount(const QModelIndex &parent) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
	virtual QModelIndex parent(const QModelIndex &child) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;

	void setSavedSchemes(const QList<stSavedScheme> &savedSchemes);
	const QList<stSavedScheme>& savedSchemes(void) const{return m_savedSchemes;}
};

class CSaveModSchemesDlg : public QDialog
{
	Q_OBJECT
public:
	enum TSaveMDDlgResult{Discarded = QDialog::Accepted + 1};
private:
	CSavedSchemeModel *m_savedSchemesModel;
	QListView *m_savedSchemesView;
	QDialogButtonBox *m_buttonBox;
public:
	explicit CSaveModSchemesDlg(QWidget *parent = 0);

	void setSavedSchemes(const QList<CScheme*> &schemes);
	const QList<stSavedScheme> savedSchemes(void) const;
private slots:
	void onDlgButtonClicked(QAbstractButton *button);
};

#endif // CSAVEMODSCHEMESDLG_H