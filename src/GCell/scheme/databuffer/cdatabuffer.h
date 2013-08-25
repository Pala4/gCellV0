#ifndef CDATABUFFER_H
#define CDATABUFFER_H

#include <QObject>

#include <QVector>
#include <QPointF>

#include "../timeframe.h"

struct stData
{
	stTimeFrame timeFrame;
	qreal value;

	stData(void){value = 0.0;}
	stData(const stTimeFrame &a_frame, const qreal &a_value){timeFrame = a_frame, value = a_value;}

	bool operator ==(const stData &other) const
	{
		return ((timeFrame == other.timeFrame) && (value == other.value));
	}
};

class CDataBuffer : public QObject
{
	Q_OBJECT
private:
	QVector<stData> m_data;
public:
	explicit CDataBuffer(QObject *parent = 0);

	const QVector<stData>& data(void) const{return m_data;}
	stData data(const int &index) const;
	stData last(void) const;
	void append(const stTimeFrame &timeFrame, const qreal &value);
	QVector<stTimeFrame> frames(const qreal &value) const;
	QVector<qreal> values(const stTimeFrame &frame) const;
	int count(void) const{return m_data.count();}
	bool isEmpty(void) const{return m_data.isEmpty();}
	bool contains(const int &index) const;
	bool containsFrame(const stTimeFrame &frame);
	bool containsValue(const qreal &value);
	void clear(void);
signals:
	void dataAppended(stTimeFrame timeFrame, stData data);
	void cleared(void);
};

#endif // CDATABUFFER_H