#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>

#include <QMap>

class QActionGroup;

class CAlgorithmProtosToolBar;
class CDataWindow;
class CAlgorithmProto;
class CAlgorithmProtoMng;
class CElement;
class CAlgorithm;
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
	CDataWindow *m_dataWindow;

	CAlgorithmProtoMng *m_algorithmProtoMng;
	CSchemeEditor *m_schemeEditor;
    CScheme *m_scheme;
    CEngine *m_engine;

    void setupToolBars(void);
	void writeScheme(CScheme *scheme, const QString &fileName);
	void readScheme(CScheme *scheme, const QString &fileName);
protected:
    virtual void closeEvent(QCloseEvent *event);
public:
    explicit CMainWindow(QWidget *parent = 0);
private slots:
    void onCursorTriggered(const bool &checked);
    void onHandTriggered(const bool &checked);
	void onLinkingTriggered(const bool &checked);
	void onAlgorithmProtoSelected(CAlgorithmProto *selectedProto);
    void onSchemeEditorMouseReleased(const QPointF &pos);

	void showData(void);

	void newScheme(void);
	void saveScheme(void);
    bool saveSchemeAs(void);
	void openScheme(void);
    void closeScheme(void);

    void saveConfig(const QString &fileName);
    void restoreConfig(const QString &fileName);
};

#endif // CMAINWINDOW_H