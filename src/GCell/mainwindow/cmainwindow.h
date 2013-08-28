#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>

#include <QMap>

class QActionGroup;

class CDataWindow;
class CAlgorithmProto;
class CAlgorithmProtoMng;
class CAlgProtoView;
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
	QActionGroup *m_acGrSchemeMouseMode;
	QAction *m_acCursor;
	QAction *m_acHand;
	QAction *m_acLinking;

	CDataWindow *m_dataWindow;

	CAlgorithmProtoMng *m_algorithmProtoMng;
	CAlgProtoView *m_algProtoView;
	QDockWidget *m_algProtoViewDock;

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
	void onAlgProtosViewDockLocationChanged(const Qt::DockWidgetArea &area);
	void onAlgProtosViewDockTopLevelChanged(const bool &topLevel);

	void showOptions(void);
	void showData(void);

	void updateTitle(void);
	void newScheme(void);
	void saveScheme(void);
	bool saveSchemeAs(void);
	bool openScheme(const QString &fileName = QString());
	void closeScheme(void);

	void saveDesktop(const QString &fileName);
	void restoreDesktop(const QString &fileName);
	void saveConfig(const QString &fileName);
	void restoreConfig(const QString &fileName);
};

#endif // CMAINWINDOW_H