#ifndef CALGPROTOVIEW_H
#define CALGPROTOVIEW_H

#include <QToolBox>

#include <QMap>

class QGridLayout;
class QToolButton;
class QButtonGroup;
class QAbstractButton;

class CAlgorithmProto;
class CAlgorithmProtoMng;

class CAlgProtoGroupWidget : public QWidget
{
	Q_OBJECT
private:
	int m_columnCount;
	int m_currentRow;
	int m_currentCol;
	QGridLayout *m_buttonsGrid;
public:
	explicit CAlgProtoGroupWidget(QWidget *parent = 0);

	const int& columnCount(void) const{return m_columnCount;}
	void setColumnCount(const int &columnCount){m_columnCount = columnCount;}

	void addAlgProtoButton(QToolButton *algProtoButton);
};

class CAlgProtoView : public QToolBox
{
	Q_OBJECT
private:
	QString m_defaultGroupName;
	QMap<QString, CAlgProtoGroupWidget*> m_algProtoGroupWidgets;
	QMap<QToolButton*, CAlgorithmProto*> m_buttonAlgProtoMap;
	QButtonGroup *m_buttonGroup;
	CAlgorithmProtoMng *m_algProtoMng;

	void fill(void);
	CAlgProtoGroupWidget* algProtoGroupWidget(const QString &groupName);

	void init(void);
public:
	explicit CAlgProtoView(QWidget *parent = 0);
	explicit CAlgProtoView(CAlgorithmProtoMng *algProtoMng, QWidget *parent = 0);

	const QString& defaultGroupName(void) const{return m_defaultGroupName;}
	void setDefaultGroupName(const QString &defaultGroupName){m_defaultGroupName = defaultGroupName;}

	CAlgorithmProtoMng* algProtoMng(void){return m_algProtoMng;}
	void setAlgProtoMng(CAlgorithmProtoMng *algProtoMng);
private slots:
	void onAlgProtoAdded(CAlgorithmProto *algProto);
	void onAlgProtoRemoved(CAlgorithmProto *algProto);
	void onAlgProtoProgrammSelected(CAlgorithmProto *algProto);
	void onAlgProtoDestroyed(QObject *objAlgProto);
	void onAlgProtoMngDestroyed(QObject *objAlgProtoMng);
	void onButtonGroupButtonPressed(QAbstractButton *button);
public slots:
	void releaseChekedButton(void);
	void clear(void);
};

#endif // CALGPROTOVIEW_H