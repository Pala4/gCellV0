#ifndef CDATABUFFER_H
#define CDATABUFFER_H

#include <QObject>

#include <QVector>
#include <QPointF>

struct stData
{
	qreal timeFrame;
	qreal value;

	stData(void){timeFrame = 0.0; value = 0.0;}
	stData(const qreal &a_timeFrame, const qreal &a_value){timeFrame = a_timeFrame, value = a_value;}

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
	void appendData(const stData &data);
	void appendData(const qreal &timeFrame, const qreal &data);
	QVector<qreal> timeFrames(const qreal &value) const;
	QVector<qreal> values(const qreal &timeFrame) const;
	int count(void) const{return m_data.count();}
	bool isEmpty(void) const{return m_data.isEmpty();}
	bool contains(const int &index) const;
	bool containsTimeFrame(const qreal &timeFrame);
	bool containsValue(const qreal &value);
	void clear(void);
signals:
	void dataAppended(stData data);
	void cleared(void);
};

#endif // CDATABUFFER_H