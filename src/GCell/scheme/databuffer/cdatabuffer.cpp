#include "cdatabuffer.h"

CDataBuffer::CDataBuffer(QObject *parent) : QObject(parent)
{
	setObjectName(QStringLiteral("CDataBuffer"));
}

stData CDataBuffer::data(const int &index) const
{
	if(m_data.count() <= index) return stData();
	return m_data.at(index);
}

stData CDataBuffer::last(void) const
{
	if(m_data.isEmpty()) return stData();
	return m_data.last();
}

void CDataBuffer::appendData(const stData &data)
{
	if(m_data.contains(data)) return;
	m_data.append(data);
	emit dataAppended(data);
}

void CDataBuffer::appendData(const qreal &timeFrame, const qreal &data)
{
	appendData(stData(timeFrame, data));
}

QVector<qreal> CDataBuffer::timeFrames(const qreal &value) const
{
	QVector<qreal> resTFs;
	foreach(stData data, m_data)
	{
		if(data.value == value) resTFs << data.timeFrame;
	}
	return resTFs;
}

QVector<qreal> CDataBuffer::values(const qreal &timeFrame) const
{
	QVector<qreal> resVals;
	foreach(stData data, m_data)
	{
		if(data.timeFrame == timeFrame) resVals << data.value;
	}
	return resVals;
}

bool CDataBuffer::contains(const int &index) const
{
	if(index < 0 || index >= m_data.count()) return false;
	return true;
}

bool CDataBuffer::containsTimeFrame(const qreal &timeFrame)
{
	foreach(stData data, m_data)
	{
		if(data.timeFrame == timeFrame) return true;
	}
	return false;
}

bool CDataBuffer::containsValue(const qreal &value)
{
	foreach(stData data, m_data)
	{
		if(data.value == value) return true;
	}
	return false;
}

void CDataBuffer::clear(void)
{
	if(m_data.isEmpty()) return;
	m_data.clear();
	emit cleared();
}