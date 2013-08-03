#include "cdatabuffer.h"

bool CDataBuffer::writeData(const int &timeFrame, const qreal &data)
{
	if(m_datas.contains(timeFrame)) return false;
	m_datas[timeFrame] = data;
	return true;
}

qreal CDataBuffer::readData(const int &timeFrame)
{
	if(!m_datas.contains(timeFrame)) return 0.0;
	return m_datas[timeFrame];
}

void CDataBuffer::clearData(void)
{
	m_datas.clear();
}

bool CDataBuffer::containsData(const int &timeFrame)
{
	return m_datas.contains(timeFrame);
}

CDataBuffer::CDataBuffer(QObject *parent) : CAbstractDataBuffer(parent)
{
	setObjectName(QStringLiteral("CDataBuffer"));
}

int CDataBuffer::count(void)
{
	return m_datas.count();
}

bool CDataBuffer::isEmpty(void)
{
	return m_datas.isEmpty();
}