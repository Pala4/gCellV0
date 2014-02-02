#ifndef CCSVIN_H
#define CCSVIN_H

#include "algorithm/cdatasource.h"

class CCSVInOptionShit;

class CCSVIn : public CDataSource
{
    Q_OBJECT
	Q_PROPERTY(QString fileName READ fileName WRITE setFileName)
    Q_PROPERTY(QByteArray resultDataMapping READ resultDataMapping WRITE setResultDataMapping)
private:
    QList<QList<qreal> > m_csvData;
	QString m_fileName;
	CCSVInOptionShit *m_optShit;
    QMap<int, QString> m_resultDataMapping;
protected:
    void proced(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
                const long double &ldblStartTime, const long double &ldblTimeStep,
                const long double &ldblEndTime);
public:
    Q_INVOKABLE explicit CCSVIn(QGraphicsItem *parent = nullptr);

    const QString& fileName() const{return m_fileName;}
	void setFileName(const QString &fileName){m_fileName = fileName;}
    QByteArray resultDataMapping();
    void setResultDataMapping(QByteArray resultDataMapping);

    QList<int> dataIndexes() const{return m_resultDataMapping.keys();}

    CElementOptionsWgt* optionsWidget(QWidget *parentWidget);
    void acceptOptions(CElementOptionsWgt *optWgt);
    void beforeCalc(const long double &ldblStartTime, const long double &ldblTimeStep,
                    const long double &ldblEndTime);
    void afterCalc(const unsigned long long &ullTFCount, const long double &ldblLastFrame,
                   const long double &ldblStartTime, const long double &ldblTimeStep,
                   const long double &ldblEndTime);
};

#endif // CCSVIN_H