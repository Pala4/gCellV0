#ifndef TIMEFRAME_H
#define TIMEFRAME_H

#include <QtGlobal>
#include <QDateTime>

struct stTimeFrame
{
	qint64 timeStamp;
	int timeStampCounter;
	quint64 timeFrameIndex;
	qreal time;

	stTimeFrame(void) :
		timeStamp(QDateTime::currentMSecsSinceEpoch()),
		timeStampCounter(0),
		timeFrameIndex(0),
		time(0.0){}

	bool operator == (const stTimeFrame &other) const
	{
		return ((timeStamp == other.timeStamp) &&
				(timeStampCounter == other.timeStampCounter) &&
				(timeFrameIndex == other.timeFrameIndex) &&
				(time == other.time));
	}
};

struct stTFArgs
{
	qreal startTime;
	qreal timeStep;
	qreal endTime;

	stTFArgs(void) :
		startTime(0.0),
		timeStep(1.0),
		endTime(0.0){}
};

struct stTimeLine
{
	stTimeFrame timeFrame;
	stTFArgs args;

	stTimeLine(void){}
	qint64 timeFrameID(void) const{return timeFrame.timeStamp + timeFrame.timeStampCounter;}
};

#endif // TIMEFRAME_H