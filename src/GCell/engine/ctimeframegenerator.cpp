#include "ctimeframegenerator.h"

#include <QDateTime>

void CTimeFrameGenerator::init(void)
{
	setObjectName(QStringLiteral("CTimeFrameGenerator"));

	m_startTime = 0.0;
	m_timeStep = 1.0;
	m_endTime = 0.0;

	m_stopped = true;
}

CTimeFrameGenerator::CTimeFrameGenerator(QObject *parent) : QObject(parent)
{
	init();
}

CTimeFrameGenerator::CTimeFrameGenerator(const qreal &startTime, const qreal &timeStep, const qreal &endTime,
										 QObject *parent) : QObject(parent)
{
	init();
	setTimeLine(startTime, timeStep, endTime);
}

void CTimeFrameGenerator::setTimeLine(const qreal &startTime, const qreal &timeStep, const qreal &endTime)
{
	setStartTime(startTime);
	setTimeStep(timeStep);
	setEndTime(endTime);
}

bool CTimeFrameGenerator::checkTimeLineArguments(const qreal &startTime, const qreal &timeStep, const qreal &endTime)
{
	return ((startTime <= endTime) && (timeStep >= 0.0));
}

void CTimeFrameGenerator::start(void)
{
	if(!checkTimeLineArguments(startTime(), timeStep(), endTime())) return;

	setStopped(false);

	stTimeLine timeLine;
	timeLine.timeFrame.time = timeLine.timeFrame.timeFrameIndex*timeStep() + startTime();
	timeLine.args.startTime = startTime();
	timeLine.args.timeStep = timeStep();
	timeLine.args.endTime = endTime();

	while(!isStopped())
	{
		if(timeLine.timeFrame.time > timeLine.args.endTime)
		{
			timeLine.timeFrame.time = timeLine.args.endTime;
			setStopped(true);
		}

		emit newTimeFrame(timeLine);

		if(timeLine.timeFrame.time < timeLine.args.endTime)
		{
			if(timeLine.timeFrame.timeStamp != QDateTime::currentMSecsSinceEpoch())
			{
				timeLine.timeFrame.timeStamp = QDateTime::currentMSecsSinceEpoch();
				timeLine.timeFrame.timeStampCounter = 0;
			}
			else
			{
				++timeLine.timeFrame.timeStampCounter;
			}

			timeLine.timeFrame.time = ++timeLine.timeFrame.timeFrameIndex*timeLine.args.timeStep +
									  timeLine.args.startTime;
		}
		else if(timeLine.timeFrame.time == timeLine.args.endTime)
		{
			setStopped(true);
		}
	}
}

void CTimeFrameGenerator::stop(void)
{
	setStopped(true);
}