#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>

#include <QMap>

class QActionGroup;
class QTabWidget;

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
	Q_PROPERTY(bool autoSaveConfig READ isAutoSaveConfig WRITE setAutoSaveConfig)
	Q_PROPERTY(bool autoSaveDesktop READ isAutoSaveDesktop WRITE setAutoSaveDesktop)
	Q_PROPERTY(bool autoSaveLastScheme READ isAutoSaveLastScheme WRITE setAutoSaveLastScheme)
	Q_PROPERTY(bool autoLoadLastScheme READ isAutoLoadLastScheme WRITE setAutoLoadLastScheme)
	Q_PROPERTY(QColor gridColor READ gridColor WRITE setGridColor)
	Q_PROPERTY(QColor gridBkGndColor READ gridBkGndColor WRITE setGridBkGndColor)
	Q_PROPERTY(int gridStep READ gridStep WRITE setGridStep)
	Q_PROPERTY(bool gridAlign READ isGridAlign WRITE setGridAlign)
private:
	bool m_autoSaveConfig;
	bool m_autoSaveDesktop;
	bool m_autoSaveLastScheme;
	bool m_autoLoadLastScheme;
	QColor m_gridColor;
	QColor m_gridBkGndColor;
	int m_gridStep;
	bool m_gridAlign;

	QAction *m_acCalc;
	QActionGroup *m_acGrSchemeMouseMode;
	QAction *m_acCursor;
	QAction *m_acHand;
	QAction *m_acLinking;

	QTabWidget *m_workSpaceTabWgt;

	CDataWindow *m_dataWindow;

	CAlgorithmProtoMng *m_algorithmProtoMng;
	CAlgProtoView *m_algProtoView;
	QDockWidget *m_algProtoViewDock;

	CSchemeEditor *m_schemeEditor;
	CScheme *m_scheme;
	CEngine *m_engine;

	void setupToolBars(void);
	void setupStatusBar(void);
	void writeScheme(CScheme *scheme, const QString &fileName);
	void readScheme(CScheme *scheme, const QString &fileName);
protected:
	virtual void closeEvent(QCloseEvent *event);
public:
	explicit CMainWindow(QWidget *parent = 0);

	const bool& isAutoSaveConfig(void) const{return m_autoSaveConfig;}
	void setAutoSaveConfig(const bool &autoSaveConfig){m_autoSaveConfig = autoSaveConfig;}
	const bool& isAutoSaveDesktop(void) const{return m_autoSaveDesktop;}
	void setAutoSaveDesktop(const bool &autoSaveDesktop){m_autoSaveDesktop = autoSaveDesktop;}
	const bool& isAutoSaveLastScheme(void) const{return m_autoSaveLastScheme;}
	void setAutoSaveLastScheme(const bool &autoSaveLastScheme){m_autoSaveLastScheme = autoSaveLastScheme;}
	const bool& isAutoLoadLastScheme(void) const{return m_autoLoadLastScheme;}
	void setAutoLoadLastScheme(const bool &autoLoadLastScheme){m_autoLoadLastScheme = autoLoadLastScheme;}
	const QColor& gridColor(void) const{return m_gridColor;}
	void setGridColor(const QColor &gridColor);
	const QColor& gridBkGndColor(void) const{return m_gridBkGndColor;}
	void setGridBkGndColor(const QColor &gridBkGndColor);
	const int& gridStep(void) const{return m_gridStep;}
	void setGridStep(const int &gridStep);
	const bool& isGridAlign(void) const{return m_gridAlign;}
	void setGridAlign(const bool &gridAlign);
private slots:
	void onCursorTriggered(const bool &checked);
	void onHandTriggered(const bool &checked);
	void onLinkingTriggered(const bool &checked);
	void onAlgorithmProtoSelected(CAlgorithmProto *selectedProto);
	void onSchemeEditorMouseReleased(const QPointF &pos);
	void onAlgProtosViewDockLocationChanged(const Qt::DockWidgetArea &area);
	void onAlgProtosViewDockTopLevelChanged(const bool &topLevel);

	void onSchemeEditorWindowTitleChanged(void);

	void showOptions(void);
	void showData(void);

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