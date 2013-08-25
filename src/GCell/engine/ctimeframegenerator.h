#ifndef CTIMEFRAMEGENERATOR_H
#define CTIMEFRAMEGENERATOR_H

#include <QObject>

#include "../scheme/timeframe.h"

class CTimeFrameGenerator : public QObject
{
	Q_OBJECT
	Q_PROPERTY(qreal startTime READ startTime WRITE setStartTime)
	Q_PROPERTY(qreal timeStep READ timeStep WRITE setTimeStep)
	Q_PROPERTY(qreal endTime READ endTime WRITE setEndTime)
private:
	qreal m_startTime;
	qreal m_timeStep;
	qreal m_endTime;

	bool m_stopped;

	void init(void);
	void setStopped(const bool &stopped){m_stopped = stopped;}
public:
	explicit CTimeFrameGenerator(QObject *parent = 0);
	explicit CTimeFrameGenerator(const qreal &startTime, const qreal &timeStep, const qreal &endTime, QObject *parent = 0);
	const qreal& startTime(void) const{return m_startTime;}
	void setStartTime(const qreal &startTime){m_startTime = startTime;}
	const qreal& timeStep(void) const{return m_timeStep;}
	void setTimeStep(const qreal &timeStep){m_timeStep = timeStep;}
	const qreal& endTime(void) const{return m_endTime;}
	void setEndTime(const qreal &endTime){m_endTime = endTime;}
	void setTimeLine(const qreal &startTime, const qreal &timeStep, const qreal &endTime);

	static bool checkTimeLineArguments(const qreal &startTime, const qreal &timeStep, const qreal &endTime);
	const bool& isStopped(void) const{return m_stopped;}
public slots:
	void start(void);
	void stop(void);
signals:
	void newTimeFrame(stTimeLine timeLine);
};

#endif // CTIMEFRAMEGENERATOR_H