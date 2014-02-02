#ifndef CSUM_H
#define CSUM_H

#include "algorithm/cdatatransmitter.h"
#include "portal/cargument.h"

class CSumArgument : public CArgument
{
	Q_OBJECT
	Q_PROPERTY(bool invert READ isInvert WRITE setInvert)
private:
    bool m_bInvert;
public:
    explicit CSumArgument(QGraphicsItem *parent = nullptr);

    const bool& isInvert() const{return m_bInvert;}
    stData bufferData(const unsigned long long &ullTFIndex);
public slots:
	void setInvert(const bool &invert);
};

class CSum : public CDataTransmitter
{
	Q_OBJECT
private:
    QList<CSumArgument*> m_sumArgs;
    CResult *m_pSumRes;
protected:
    void proced(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
                const long double &ldblStartTime, const long double &ldblTimeStep,
                const long double &ldblEndTime);
public:
    Q_INVOKABLE explicit CSum(QGraphicsItem *parent = nullptr);

    CElement* createElement(const QString &typeID);
private slots:
    CSumArgument* addSumArg();
	void onSumArgDestroyed(QObject *objSumArg);
};

#endif // CSUM_H