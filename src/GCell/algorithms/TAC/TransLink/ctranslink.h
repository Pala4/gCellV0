#ifndef CTRANSLINK_H
#define CTRANSLINK_H

#include "../../../scheme/algorithm/cdatatransmitter.h"

#include "../../../scheme/databuffer/cdatabuffer.h"

class QComboBox;
class QLineEdit;

class CArgument;
class CResult;
class CTLAbstractAlgorithm;

class CTransLink : public CDataTransmitter
{
	Q_OBJECT
	Q_PROPERTY(int selectedTLAlg READ selectedTLAlg WRITE setSelectedTLAlg)
	Q_PROPERTY(QString factorSeparator READ factorSeparator WRITE setFactorSeparator)
	Q_PROPERTY(QString nomFactors READ nomFactorsStr WRITE setNomFactorsStr)
	Q_PROPERTY(QString denomFactors READ denomFactorsStr WRITE setDenomFactorsStr)
private:
	CArgument *m_inArg;
	CResult *m_outRes;

	QComboBox *m_selTLAlgComboBox;
	QLineEdit *m_nominatorFactorsLineEdit;
	QLineEdit *m_denominatorFactorsLineEdit;

	int m_selectedTLAlg;
	QString m_factorSeparator;
	QVector<qreal> m_nomFactors;
	QVector<qreal> m_denomFactors;

	QList<CTLAbstractAlgorithm*> m_tlAlgs;

	QVector<qreal> stringToVector(const QString &string, const QString &sep);
	QString vectorToString(const QVector<qreal> &vector, const QString &sep);
protected:
	virtual void proced(const stTimeLine &timeLine);
public:
	Q_INVOKABLE explicit CTransLink(QGraphicsItem *parent = 0);
	virtual ~CTransLink(void);

	const int& selectedTLAlg(void) const{return m_selectedTLAlg;}
	void setSelectedTLAlg(const int &selectedTLAlg);
	const QString& factorSeparator(void) const{return m_factorSeparator;}
	void setFactorSeparator(const QString &factorSeparator){m_factorSeparator = factorSeparator;}
	QString nomFactorsStr(void);
	void setNomFactorsStr(const QString &nomFactors);
	QString denomFactorsStr(void);
	void setDenomFactorsStr(const QString &denomFactors);
	const QVector<qreal>& nomFactors(void) const{return m_nomFactors;}
	void setNomFactors(const QVector<qreal> &nomFactors);
	const QVector<qreal>& denomFactors(void) const{return m_denomFactors;}
	void setDenomFactors(const QVector<qreal> &denomFactors);

	virtual CElementOptionsWgt* optionsWidget(QWidget *parentWidget);
	virtual void acceptOptions(CElementOptionsWgt *optWgt);
	virtual void beforeCalc(const qreal &startTime, const qreal &timeStep, const qreal &endTime);
signals:
	void nomFactorsChanged(QVector<qreal> nomFactors);
	void denomFactorsChanged(QVector<qreal> denomFactors);
};

#endif // CTRANSLINK_H