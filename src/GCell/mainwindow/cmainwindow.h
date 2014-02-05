#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>

#include <QMap>
#include <QClipboard>

class QActionGroup;
class QTabWidget;

class CDataWindow;
class CAlgorithmProto;
class CAlgorithmProtoMng;
class CAlgProtoView;
class CElement;
class CAlgorithm;
class CScheme;
class CSchemeEditor;
class CThreadEngine;

class CMainWindow : public QMainWindow
{
	Q_OBJECT
	Q_PROPERTY(bool autoSaveConfig READ isAutoSaveConfig WRITE setAutoSaveConfig)
	Q_PROPERTY(bool autoSaveDesktop READ isAutoSaveDesktop WRITE setAutoSaveDesktop)
	Q_PROPERTY(bool autoSaveLastScheme READ isAutoSaveLastScheme WRITE setAutoSaveLastScheme)
	Q_PROPERTY(bool autoLoadLastScheme READ isAutoLoadLastScheme WRITE setAutoLoadLastScheme)
	Q_PROPERTY(QColor gridColor READ gridColor WRITE setGridColor)
	Q_PROPERTY(QColor gridBkGndColor READ gridBkGndColor WRITE setGridBkGndColor)
	Q_PROPERTY(int gridStep READ gridStep WRITE setGridStep)
	Q_PROPERTY(qreal gridPointSize READ gridPointSize WRITE setGridPointSize)
	Q_PROPERTY(bool gridAlign READ isGridAlign WRITE setGridAlign)
private:
	bool m_autoSaveConfig;
	bool m_autoSaveDesktop;
	bool m_autoSaveLastScheme;
	bool m_autoLoadLastScheme;
	QColor m_gridColor;
	QColor m_gridBkGndColor;
	int m_gridStep;
	qreal m_gridPointSize;
	bool m_gridAlign;

	QAction *m_acNewScheme;
	QAction *m_acOpenScheme;
	QAction *m_acSaveScheme;
	QAction *m_acSaveSchemeAs;
	QAction *m_acCloseScheme;
	QAction *m_acQuit;
	QActionGroup *m_acGrSchemeMouseMode;
	QAction *m_acCursor;
	QAction *m_acHand;
	QAction *m_acLinking;
	QAction *m_acEditSep;
	QAction *m_acCopy;
	QAction *m_acCut;
	QAction *m_acDelete;
	QAction *m_acPaste;
	QAction *m_acDataWindow;
    QAction *m_acCalcActiveScheme;
    QAction *m_acCalcAllSchemes;
	QAction *m_acOptions;
	QAction *m_acElementOptions;

	QMenu *m_viewToolBarsMenu;
	QMenu *m_viewDocksMenu;
	QMenu *m_schemeEditorContextMenu;

	QTabWidget *m_workSpaceTabWgt;
	CDataWindow *m_dataWindow;
	CAlgorithmProtoMng *m_algorithmProtoMng;
	CAlgProtoView *m_algProtoView;
	QDockWidget *m_algProtoViewDock;

    int m_schemeCounter;
    CScheme *m_activeScheme;
    QMap<CScheme*, CSchemeEditor*> m_documents;

    CThreadEngine *m_pThreadEngine;

	void setupActions(void);
	void setupMainMenu(void);
	void setupToolBars(void);
	void setupDocks(void);
	void setupStatusBar(void);
	void setupSchemeEditorContextMenu(void);

	void writeScheme(CScheme *scheme, const QString &fileName);
    bool readScheme(CScheme *scheme, const QString &fileName);
	bool saveSchemesBeforeClose(const QList<CScheme*> &schemes);

	CScheme* activeScheme(void);
    CSchemeEditor* activeSchemeEditor(void);

    CScheme *findScheme(const QString &schemeFileName);
protected:
	virtual void closeEvent(QCloseEvent *event);
public:
	explicit CMainWindow(QWidget *parent = 0);

	const bool& isAutoSaveConfig(void) const{return m_autoSaveConfig;}
	void setAutoSaveConfig(const bool &autoSaveConfig){m_autoSaveConfig = autoSaveConfig;}
	const bool& isAutoSaveDesktop(void) const{return m_autoSaveDesktop;}
	void setAutoSaveDesktop(const bool &autoSaveDesktop){m_autoSaveDesktop = autoSaveDesktop;}
	const bool& isAutoSaveLastScheme(void) const{return m_autoSaveLastScheme;}
    void setAutoSaveLastScheme(const bool &autoSaveLastScheme);
	const bool& isAutoLoadLastScheme(void) const{return m_autoLoadLastScheme;}
    void setAutoLoadLastScheme(const bool &autoLoadLastScheme);
	const QColor& gridColor(void) const{return m_gridColor;}
	void setGridColor(const QColor &gridColor);
	const QColor& gridBkGndColor(void) const{return m_gridBkGndColor;}
	void setGridBkGndColor(const QColor &gridBkGndColor);
	const int& gridStep(void) const{return m_gridStep;}
	void setGridStep(const int &gridStep);
	const qreal& gridPointSize(void) const{return m_gridPointSize;}
	void setGridPointSize(const qreal &gridPointSize);
	const bool& isGridAlign(void) const{return m_gridAlign;}
	void setGridAlign(const bool &gridAlign);

	bool isDataWindowVisisble(void) const;
private slots:
	void onCursorTriggered(const bool &checked);
	void onHandTriggered(const bool &checked);
	void onLinkingTriggered(const bool &checked);
	void onAlgorithmProtoSelected(CAlgorithmProto *selectedProto);
	void onSchemeEditorAddAlgorithmModeFinished(void);
	void onAlgProtosViewDockLocationChanged(const Qt::DockWidgetArea &area);
	void onAlgProtosViewDockTopLevelChanged(const bool &topLevel);
	void onSchemeEditorWindowTitleChanged(void);
	void onSchemeEditorElementsSelected(const QList<CElement*> &elements);
	void onClipBoardChanged(const QClipboard::Mode &mode);
    void onCurrentTabChanged(const int &currentTabIndex);
    void onTabCloseRequested(const int &tabIndex);

    void calcActiveScheme(void);
    void calcAllSchemes(void);
    void showOptions(void);
	void setDataWindowVisible(const bool &visible);
	void onDataWindowVisibleChanged(const bool &visible);

    CScheme* newScheme(CScheme *scheme = nullptr);
	void saveScheme(CScheme *scheme = 0);
	bool saveSchemeAs(CScheme *scheme = 0);
    bool openSchemes(const QStringList &fileNames = QStringList());
    bool openScheme(const QString &fileName = QString());
    bool closeScheme(CScheme *scheme = 0);
    void onSchemeDestroyed(QObject *objScheme);

	void copy(void);
	void cut(void);
	void del(void);
	void paste(void);

	void elementOptions(void);

	void saveDesktop(const QString &fileName);
	void restoreDesktop(const QString &fileName);
	void saveConfig(const QString &fileName);
	void restoreConfig(const QString &fileName);
};

#endif // CMAINWINDOW_H
