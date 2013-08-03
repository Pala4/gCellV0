#ifndef CABSTRACTDATABUFFER_H
#define CABSTRACTDATABUFFER_H

#include <QObject>

class CAbstractDataBuffer : public QObject
{
	Q_OBJECT
protected:
	virtual bool writeData(const int &timeFrame, const qreal &data) = 0;
	virtual qreal readData(const int &timeFrame) = 0;
	virtual void clearData(void) = 0;
	virtual bool containsData(const int &timeFrame) = 0;
public:
	explicit CAbstractDataBuffer(QObject *parent = 0);
public slots:
	void addData(const int &timeFrame, const qreal &data);
	qreal data(const int &timeFrame);
	void clear(void);
	bool isDataReady(const int &timeFrame);
	virtual int count(void) = 0;
	virtual bool isEmpty(void) = 0;
signals:
	void dataAdded(const int &timeFrame, const qreal &data);
};

#endif // CABSTRACTDATABUFFER_H