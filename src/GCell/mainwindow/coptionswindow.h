#ifndef COPTIONSWINDOW_H
#define COPTIONSWINDOW_H

#include <QDialog>

#include "../widgets/clineeditbutton.h"

class QCheckBox;
class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;

class CColorLineEdit : public CLineEditButton
{
	Q_OBJECT
public:
	explicit CColorLineEdit(QWidget *parent = 0);

	QColor color(void) const;
	void setColor(const QColor &color);
private slots:
	void onButtonClicked(void);
};

class COptionsWindow : public QDialog
{
	Q_OBJECT
private:
	QCheckBox *m_autoSaveConfigChBox;
	QCheckBox *m_autoSaveDesktopChBox;
	QCheckBox *m_autoSaveLastSchemeChBox;
	QCheckBox *m_autoLoadLastSchemeChBox;

	CColorLineEdit *m_gridColorLEdit;
	CColorLineEdit *m_gridBkGndColorLEdit;
	QSpinBox *m_gridStepSpBox;
	QCheckBox *m_gridAlignChBox;

	QWidget *m_schemeTab;
	QDoubleSpinBox *m_schemeWidthSpBox;
	QDoubleSpinBox *m_schemeHeightSpBox;

	QWidget *m_calcParamTab;
	QDoubleSpinBox *m_startTimeSpBox;
	QDoubleSpinBox *m_timeStepSpBox;
	QDoubleSpinBox *m_endTimeSpBox;
public:
	explicit COptionsWindow(QWidget *parent = 0);

	bool isAutoSaveConfig(void) const;
	void setAutoSaveConfig(const bool &autoSaveConfig);
	bool isAutoSaveDesktop(void) const;
	void setAutoSaveDesktop(const bool &autoSaveDesktop);
	bool isAutoSaveLastScheme(void) const;
	void setAutoSaveLastScheme(const bool &autoSaveLastScheme);
	bool isAutoLoadLastScheme(void) const;
	void setAutoLoadLastScheme(const bool &autoLoadLastScheme);

	QColor gridColor(void) const;
	void setGridColor(const QColor &color);
	QColor gridBkGndColor(void) const;
	void setGridBkGndColor(const QColor &bkGndColor);
	int gridStep(void) const;
	void setGridStep(const int &gridStep);
	bool isGridAlign(void) const;
	void setGridAlign(const bool &gridAlign);

	QWidget* schemeTab(void){return m_schemeTab;}
	qreal schemeWidth(void) const;
	void setSchemeWidhet(const qreal &schemeWidhet);
	qreal schemeHeight(void) const;
	void setSchemeHeight(const qreal &schemeHeight);

	QWidget* calcParamTab(void){return m_calcParamTab;}
	qreal startTime(void) const;
	void setStartTime(const qreal &startTime);
	qreal timeStep(void) const;
	void setTimeStep(const qreal &timeStep);
	qreal endTime(void) const;
	void setEndTime(const qreal &endTime);
public slots:
	virtual void accept(void);
};

#endif // COPTIONSWINDOW_H