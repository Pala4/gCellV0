#ifndef CALGORITHMPROTOSTOOLBAR_H
#define CALGORITHMPROTOSTOOLBAR_H

#include <QToolBar>

#include <QMap>

class QActionGroup;

class CAlgorithmProtoMng;
class CAlgorithmProto;

class CAlgorithmProtosToolBar : public QToolBar
{
    Q_OBJECT
private:
    bool m_blockOnProtoSelected;
    bool m_blockOnManaginActionTriggered;

    QActionGroup *m_actionGroup;
	CAlgorithmProtoMng *m_algorithmProtoMng;

    QList<QAction*> m_managinActions;
    QAction *m_managinSeparator;
    QAction *m_defaultAction;
    QMap<QAction*, CAlgorithmProto*> m_protoActions;

    void addActionToGroup(QAction *action);
public:
	explicit CAlgorithmProtosToolBar(CAlgorithmProtoMng *algorithmProtoMng, QWidget *parent = 0);

	CAlgorithmProtoMng* algorithmProtoMng(void){return m_algorithmProtoMng;}
	void setAlgorithmProtoMng(CAlgorithmProtoMng *algorithmProtoMng);

    void addManaginAction(QAction *action);
    void setDefaultAction(QAction *action){m_defaultAction = action;}
private slots:
    void onManaginActionTriggered(const bool &checked);
    void onProtoActionTriggered(const bool &checked);
	void onProtoSelected(CAlgorithmProto *algorithmProto);
public slots:
	void addProto(CAlgorithmProto *algorithmProto);
	void removeProto(CAlgorithmProto *algorithmProto);
};

#endif // CALGORITHMPROTOSTOOLBAR_H