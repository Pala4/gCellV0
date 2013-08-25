#include "ccsvin.h"

#include <QGridLayout>
#include <QFile>

#include <qmath.h>

#include "ccsvinoptionshit.h"
#include "../../../scheme/celementoptionswgt.h"
#include "../../../scheme/portal/cargument.h"
#include "../../../scheme/portal/cresult.h"

/*!
 * \class CCSVIn
 */
void CCSVIn::proced(const stTimeLine &timeLine)
{
	if(timeLine.timeFrame.timeFrameIndex >= (quint64)m_csvData.count()) return;
	QList<qreal> row = m_csvData.at(timeLine.timeFrame.timeFrameIndex);
    for(int ci = 0; ci < m_resultDataMapping.count(); ++ci)
    {
        int dataIndex = m_resultDataMapping.keys().at(ci);
        if(dataIndex >= row.count()) continue;

        QString portalID = m_resultDataMapping.values().at(ci);
        qreal data = row.at(dataIndex);
        CResult *res = result(portalID);
		if(res) res->appendBuffer(timeLine.timeFrame, data);
    }
}

CCSVIn::CCSVIn(QGraphicsItem *parent) : CDataSource(parent)
{
    setObjectName(QStringLiteral("CCSVIn"));

	m_optShit = 0;
}

QByteArray CCSVIn::resultDataMapping(void)
{
    QByteArray mapping;
    QDataStream ds(&mapping, QIODevice::WriteOnly);
    ds.setVersion(QDataStream::Qt_5_1);
    for(int ci = 0; ci < m_resultDataMapping.count(); ++ci)
    {
        ds << (qint32)m_resultDataMapping.keys().at(ci);
        ds << m_resultDataMapping.values().at(ci);
    }
    return mapping;
}

void CCSVIn::setResultDataMapping(QByteArray resultDataMapping)
{
    QDataStream ds(&resultDataMapping, QIODevice::ReadOnly);
    if(ds.version() != QDataStream::Qt_5_1) return;
    m_resultDataMapping.clear();
    while(!ds.atEnd())
    {
        int dataIndex = 0;
        QString portalID;
        ds >> dataIndex;
        ds >> portalID;
        if(!m_resultDataMapping.contains(dataIndex) && !m_resultDataMapping.values().contains(portalID)) m_resultDataMapping[dataIndex] = portalID;
    }
}

CElementOptionsWgt* CCSVIn::optionsWidget(QWidget *parentWidget)
{
	CElementOptionsWgt *optWgt = CDataSource::optionsWidget(parentWidget);
	if(optWgt)
	{
        m_optShit = new CCSVInOptionShit(this);
		optWgt->generalLayout()->addWidget(m_optShit, optWgt->generalLayout()->rowCount(), 0);
	}
	return optWgt;
}

void CCSVIn::acceptOptions(CElementOptionsWgt *optWgt)
{
	CDataSource::acceptOptions(optWgt);

    if(m_optShit)
    {
        setFileName(m_optShit->fileName());
        QMap<int, QString> dataIndexes = m_optShit->dataIndexes();

        QList<QString> remResults;
        for(int ci = 0; ci < m_resultDataMapping.count(); ++ci)
        {
            if(!dataIndexes.contains(m_resultDataMapping.keys().at(ci))) remResults << m_resultDataMapping.values().at(ci);
        }
        foreach(QString resID, remResults)
        {
            m_resultDataMapping.remove(m_resultDataMapping.key(resID));
            removePortal(resID);
        }

        for(int ci = 0; ci < dataIndexes.count(); ++ci)
        {
			if(!m_resultDataMapping.contains(dataIndexes.keys().at(ci)))
			{
				m_resultDataMapping[dataIndexes.keys().at(ci)] = addResult(dataIndexes.values().at(ci))->id();
			}
			else
			{
				CResult *res = result(m_resultDataMapping[dataIndexes.keys().at(ci)]);
				if(res) res->setName(dataIndexes.values().at(ci));
			}
        }
    }
}

void CCSVIn::beforeCalc(void)
{
    QString l;
    int size;
    bool isQuoted, headerSet = false, withHeader = true;
    int maxColumn = 0;
    QChar separator = QChar(';');

    QFile file(fileName());
    if(!file.exists()) return;

    file.open(QIODevice::ReadOnly);
    while(!file.atEnd())
    {
        l = file.readLine();
        l.remove('\n');
        l.remove('\r');
        size = l.length();
        isQuoted = false;
        for(int i = 0; i < size; ++i)
        {
            if(i > 0)
            {
                if(l[i] == '"' && l[i-1] != '\\')
                {
                    isQuoted = !isQuoted;
                }
                else if(!isQuoted && l[i] == separator)
                {
                    l[i] = QChar(1);
                }
            }
            else
            {
                if(l[i] == '"')
                {
                    isQuoted = !isQuoted;
                }
                else if(!isQuoted && l[i] == separator)
                {
                    l[i] = QChar(1);
                }
            }
        }
        if(l.count(QChar(1)) + 1 > maxColumn) maxColumn = l.count(QChar(1)) + 1;
        if(withHeader && !headerSet)
        {
            headerSet = true;
        }
        else
        {
            QStringList slRow = l.split(QChar(1));
            QList<qreal> row;
            foreach(QString sItem, slRow)
            {
                bool ok;
                qreal item = sItem.toDouble(&ok);
//                if(!ok) continue;
                row << item;
            }
            if(!row.isEmpty()) m_csvData << row;
        }
    }
    file.close();	
}

void CCSVIn::afterCalc(void)
{
    foreach(QList<qreal> row, m_csvData)
    {
        row.clear();
    }
    m_csvData.clear();
}