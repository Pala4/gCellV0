#include "cdatabuffer.h"

CDataBuffer::CDataBuffer(QObject *parent) : QObject(parent)
{
	setObjectName(QStringLiteral("CDataBuffer"));
}

stData CDataBuffer::data(const unsigned long long &ullTFIndex) const
{
    if (ullTFIndex >= (unsigned long long)m_data.count())
        return stData();

    return m_data.at((int)ullTFIndex);
}

stData CDataBuffer::last() const
{
    if (m_data.isEmpty())
        return stData();

	return m_data.last();
}

void CDataBuffer::append(const long double &ldblTimeFrame, const long double &ldblValue)
{
    stData data(ldblTimeFrame, ldblValue);
	m_data.append(data);
}

QVector<long double> CDataBuffer::frames(const long double &ldblValue) const
{
    QVector<long double> resFrames;
    foreach (stData data, m_data) {
        if (data.ldblValue == ldblValue)
            resFrames << data.ldblTimeFrame;
	}

	return resFrames;
}

QVector<long double> CDataBuffer::values(const long double &ldblFrame) const
{
    QVector<long double> resVals;
    foreach (stData data, m_data) {
        if (data.ldblTimeFrame == ldblFrame)
            resVals << data.ldblValue;
	}

	return resVals;
}

bool CDataBuffer::contains(const unsigned long long &ullTFIndex) const
{
    return (ullTFIndex < (unsigned long long)m_data.count());
}

bool CDataBuffer::containsFrame(const long double &ldblFrame)
{
    foreach (stData data, m_data) {
        if (data.ldblTimeFrame == ldblFrame)
            return true;
	}

	return false;
}

bool CDataBuffer::containsValue(const long double &ldblValue)
{
    foreach (stData data, m_data) {
        if (data.ldblValue == ldblValue)
            return true;
	}

	return false;
}

void CDataBuffer::clear()
{
	m_data.clear();
}