#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>

#include <QMap>

class QActionGroup;

class CAlgorithmProtosToolBar;
class CAlgorithmProto;
class CAlgorithmProtoMng;
class CElement;
class CSchemeEditor;
class CScheme;
class CEngine;

class CMainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QAction *m_acCalc;
    QAction *m_acCursor;
    QAction *m_acHand;
	QAction *m_acLinking;

	CAlgorithmProtosToolBar *m_tbAlgorithmProtos;
	CAlgorithmProtoMng *m_algorithmProtoMng;
	CSchemeEditor *m_schemeEditor;
    CScheme *m_scheme;
    CEngine *m_engine;

    void setupToolBars(void);
public:
    explicit CMainWindow(QWidget *parent = 0);
private slots:
    void onCursorTriggered(const bool &checked);
    void onHandTriggered(const bool &checked);
	void onLinkingTriggered(const bool &checked);
	void onAlgorithmProtoSelected(CAlgorithmProto *selectedProto);
    void onSchemeEditorMouseReleased(const QPointF &pos);

	void newScheme(void);
    void closeScheme(void);
};

#endif // CMAINWINDOW_H