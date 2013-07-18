#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>

#include <QMap>
#include <QGraphicsItem>

class QActionGroup;

class CElementProtosToolBar;
class CElementProto;
class CElementProtoMng;
class CSchemeView;
class CSchemeModel;
class CScheme;
class CGrElement;
class CEngine;

class CMainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QAction *m_acCalc;
    QAction *m_acCursor;
    QAction *m_acHand;

	CElementProtosToolBar *m_tbElementProtos;
    CElementProtoMng *m_elementProtoMng;
    CSchemeView *m_schemeView;
    CSchemeModel *m_schemeModel;
    CScheme *m_scheme;
    CEngine *m_engine;

    void setupToolBars(void);
public:
    explicit CMainWindow(QWidget *parent = 0);
private slots:
    void onCursorTriggered(const bool &checked);
    void onHandTriggered(const bool &checked);
    void onElementProtoSelected(CElementProto *elementProto);
    void onSchemeEditorMouseReleased(const QPointF &pos);

	void newScheme(void);
    void closeScheme(void);

    void addElement(const QPointF &pos);
};

#endif // CMAINWINDOW_H