#ifndef COPTIONSWINDOW_H
#define COPTIONSWINDOW_H

#include <QDialog>

class QDoubleSpinBox;

class COptionsWindow : public QDialog
{
	Q_OBJECT
private:
	QDoubleSpinBox *m_startTimeSpBox;
	QDoubleSpinBox *m_timeStepSpBox;
	QDoubleSpinBox *m_endTimeSpBox;
public:
	explicit COptionsWindow(QWidget *parent = 0);

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