#ifndef CCSVIN_H
#define CCSVIN_H

#include "../../../scheme/algorithm/cdatasource.h"

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
	virtual void proced(const int &timeFrame);
public:
	Q_INVOKABLE explicit CCSVIn(QGraphicsItem *parent = 0);

	const QString& fileName(void) const{return m_fileName;}
	void setFileName(const QString &fileName){m_fileName = fileName;}
    QByteArray resultDataMapping(void);
    void setResultDataMapping(QByteArray resultDataMapping);

    QList<int> dataIndexes(void) const{return m_resultDataMapping.keys();}

	virtual CElementOptionsWgt* optionsWidget(QWidget *parentWidget);
	virtual void acceptOptions(CElementOptionsWgt *optWgt);
    virtual void beforeCalc(void);
    virtual void afterCalc(void);
};

#endif // CCSVIN_H