#include "cabstractdatabuffer.h"

CAbstractDataBuffer::CAbstractDataBuffer(QObject *parent) : QObject(parent)
{
	setObjectName(QStringLiteral("CAbstractDataBuffer"));
}

void CAbstractDataBuffer::addData(const int &timeFrame, const qreal &data)
{
	if(writeData(timeFrame, data)) emit dataAdded(timeFrame, data);
}

qreal CAbstractDataBuffer::data(const int &timeFrame)
{
	return readData(timeFrame);
}

void CAbstractDataBuffer::clear(void)
{
	clearData();
}

bool CAbstractDataBuffer::isDataReady(const int &timeFrame)
{
	return containsData(timeFrame);
}