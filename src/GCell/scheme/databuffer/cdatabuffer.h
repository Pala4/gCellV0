#ifndef CDATABUFFER_H
#define CDATABUFFER_H

#include "cabstractdatabuffer.h"

#include <QMap>

class CDataBuffer : public CAbstractDataBuffer
{
	Q_OBJECT
private:
	QMap<int, qreal> m_datas;
protected:
	virtual bool writeData(const int &timeFrame, const qreal &data);
	virtual qreal readData(const int &timeFrame);
	virtual void clearData(void);
	virtual bool containsData(const int &timeFrame);
public:
	explicit CDataBuffer(QObject *parent = 0);
public slots:
	virtual int count(void);
	virtual bool isEmpty(void);
};

#endif // CDATABUFFER_H