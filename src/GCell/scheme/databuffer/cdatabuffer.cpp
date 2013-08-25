#include "cdatabuffer.h"

CDataBuffer::CDataBuffer(QObject *parent) : QObject(parent)
{
	setObjectName(QStringLiteral("CDataBuffer"));
}

stData CDataBuffer::data(const int &index) const
{
	if(index < 0 || index >= m_data.count()) return stData();
	return m_data.at(index);
}

stData CDataBuffer::last(void) const
{
	if(m_data.isEmpty()) return stData();
	return m_data.last();
}

void CDataBuffer::append(const stTimeFrame &timeFrame, const qreal &value)
{
	stData data(timeFrame, value);
	m_data.append(data);
	emit dataAppended(timeFrame, data);
}

QVector<stTimeFrame> CDataBuffer::frames(const qreal &value) const
{
	QVector<stTimeFrame> resFrames;
	foreach(stData data, m_data)
	{
		if(data.value == value) resFrames << data.timeFrame;
	}
	return resFrames;
}

QVector<qreal> CDataBuffer::values(const stTimeFrame &frame) const
{
	QVector<qreal> resVals;
	foreach(stData data, m_data)
	{
		if(data.timeFrame == frame) resVals << data.value;
	}
	return resVals;
}

bool CDataBuffer::contains(const int &index) const
{
	if(index < 0 || index >= m_data.count()) return false;
	return true;
}

bool CDataBuffer::containsFrame(const stTimeFrame &frame)
{
	foreach(stData data, m_data)
	{
		if(data.timeFrame == frame) return true;
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