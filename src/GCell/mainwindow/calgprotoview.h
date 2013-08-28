#ifndef CALGPROTOVIEW_H
#define CALGPROTOVIEW_H

#include <QTabWidget>

#include <QMap>

class QBoxLayout;
class QPushButton;
class QButtonGroup;
class QAbstractButton;

class CAlgorithmProto;
class CAlgorithmProtoMng;

class CAlgProtoGroupWidget : public QWidget
{
	Q_OBJECT
private:
	QBoxLayout *m_buttonsLayout;

	Qt::Orientation m_orientation;
public:
	explicit CAlgProtoGroupWidget(const Qt::Orientation &orientation = Qt::Vertical, QWidget *parent = 0);

	const Qt::Orientation& orientation(void) const{return m_orientation;}
	void setOrientation(const Qt::Orientation &orientation);

	void addAlgProtoButton(QPushButton *algProtoButton);
};

class CAlgProtoView : public QTabWidget
{
	Q_OBJECT
private:
	QString m_defaultGroupName;
	QMap<QString, CAlgProtoGroupWidget*> m_algProtoGroupWidgets;
	QMap<QPushButton*, CAlgorithmProto*> m_buttonAlgProtoMap;
	QButtonGroup *m_buttonGroup;

	Qt::Orientation m_orientation;
	QTabWidget::TabPosition m_verTabPosition;
	QTabWidget::TabPosition m_horTabPosition;
	CAlgorithmProtoMng *m_algProtoMng;

	void fill(void);
	CAlgProtoGroupWidget* algProtoGroupWidget(const QString &groupName);

	void init(void);
public:
	explicit CAlgProtoView(QWidget *parent = 0);
	explicit CAlgProtoView(CAlgorithmProtoMng *algProtoMng, const Qt::Orientation &orientation = Qt::Vertical, QWidget *parent = 0);

	const QString& defaultGroupName(void) const{return m_defaultGroupName;}
	void setDefaultGroupName(const QString &defaultGroupName){m_defaultGroupName = defaultGroupName;}

	CAlgorithmProtoMng* algProtoMng(void){return m_algProtoMng;}
	void setAlgProtoMng(CAlgorithmProtoMng *algProtoMng);

	const Qt::Orientation& orientation(void) const{return m_orientation;}
	void setOrientation(const Qt::Orientation &orientation);
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