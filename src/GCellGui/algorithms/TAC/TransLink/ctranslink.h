#ifndef CTRANSLINK_H
#define CTRANSLINK_H

#include "algorithm/cdatatransmitter.h"

#include "databuffer/cdatabuffer.h"

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
    CArgument *m_pInArg;
    CResult *m_pOutRes;

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
    void proced(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
                const long double &ldblStartTime, const long double &ldblTimeStep,
                const long double &ldblEndTime);
public:
    Q_INVOKABLE explicit CTransLink(QGraphicsItem *parent = nullptr);
    virtual ~CTransLink();

    const int& selectedTLAlg() const{return m_selectedTLAlg;}
	void setSelectedTLAlg(const int &selectedTLAlg);
    const QString& factorSeparator() const{return m_factorSeparator;}
	void setFactorSeparator(const QString &factorSeparator){m_factorSeparator = factorSeparator;}
    QString nomFactorsStr();
	void setNomFactorsStr(const QString &nomFactors);
    QString denomFactorsStr();
	void setDenomFactorsStr(const QString &denomFactors);
    const QVector<qreal>& nomFactors() const{return m_nomFactors;}
	void setNomFactors(const QVector<qreal> &nomFactors);
    const QVector<qreal>& denomFactors() const{return m_denomFactors;}
	void setDenomFactors(const QVector<qreal> &denomFactors);

    CElementOptionsWgt* optionsWidget(QWidget *parentWidget);
    void acceptOptions(CElementOptionsWgt *optWgt);
    void beforeCalc(const long double &ldblStartTime, const long double &ldblTimeStep,
                    const long double &ldblEndTime);
signals:
	void nomFactorsChanged(QVector<qreal> nomFactors);
	void denomFactorsChanged(QVector<qreal> denomFactors);
};

#endif // CTRANSLINK_H