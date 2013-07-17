#ifndef CELEMENTPROTOSTOOLBAR_H
#define CELEMENTPROTOSTOOLBAR_H

#include <QToolBar>

#include <QMap>

class QActionGroup;

class CElementProtoMng;
class CElementProto;

class CElementProtosToolBar : public QToolBar
{
    Q_OBJECT
private:
    bool m_blockOnProtoSelected;
    bool m_blockOnManaginActionTriggered;

    QActionGroup *m_actionGroup;
    CElementProtoMng *m_elementProtoMng;

    QList<QAction*> m_managinActions;
    QAction *m_managinSeparator;
    QAction *m_defaultAction;
    QMap<QAction*, CElementProto*> m_protoActions;

    void addActionToGroup(QAction *action);
public:
    explicit CElementProtosToolBar(CElementProtoMng *elementProtoMng, QWidget *parent = 0);

    CElementProtoMng* elementProtoMng(void){return m_elementProtoMng;}
    void setElementProtoMng(CElementProtoMng *elementProtoMng);

    void addManaginAction(QAction *action);
    void setDefaultAction(QAction *action){m_defaultAction = action;}
private slots:
    void onManaginActionTriggered(const bool &checked);
    void onProtoActionTriggered(const bool &checked);
    void onProtoSelected(CElementProto *elementProto);
public slots:
    void addProto(CElementProto *elementProto);
    void removeProto(CElementProto *elementProto);
};

#endif // CELEMENTPROTOSTOOLBAR_H