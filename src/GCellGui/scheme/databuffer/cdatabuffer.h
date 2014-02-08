#ifndef CDATABUFFER_H
#define CDATABUFFER_H

#include <QObject>

#include <QVector>

struct stData
{
    long double ldblTimeFrame;
    long double ldblValue;

    stData() : ldblTimeFrame(0.0L), ldblValue(0.0L){}
    stData(const long double &a_ldblTimeFrame, const long double &a_ldblValue)
    {
        ldblTimeFrame = a_ldblTimeFrame;
        ldblValue = a_ldblValue;
    }

	bool operator ==(const stData &other) const
	{
        return ((ldblTimeFrame == other.ldblTimeFrame) && (ldblValue == other.ldblValue));
	}
};

class CDataBuffer : public QObject
{
	Q_OBJECT
private:
	QVector<stData> m_data;
public:
	explicit CDataBuffer(QObject *parent = 0);

    const QVector<stData>& data() const{return m_data;}
    stData data(const unsigned long long &ullTFIndex) const;
    stData last() const;
    void append(const long double &ldblTimeFrame, const long double &ldblValue);
    QVector<long double> frames(const long double &ldblValue) const;
    QVector<long double> values(const long double &ldblFrame) const;
    unsigned long long size() const{return (unsigned long long)m_data.count();}
    bool isEmpty() const{return m_data.isEmpty();}
    bool contains(const unsigned long long &ullTFIndex) const;
    bool containsFrame(const long double &ldblFrame);
    bool containsValue(const long double &ldblValue);
    void clear();
};

#endif // CDATABUFFER_H