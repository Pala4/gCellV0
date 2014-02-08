#ifndef CAMP_H
#define CAMP_H

#include "algorithm/cdatatransmitter.h"

class QDoubleSpinBox;

class CAmp : public CDataTransmitter
{
    Q_OBJECT
	Q_PROPERTY(qreal gain READ gain WRITE setGain)
private:
	qreal m_gain;

	CArgument *m_arg;
	CResult *m_res;

	QDoubleSpinBox *m_gainSpinBox;
protected:
    void proced(const unsigned long long &ullTFIndex, const long double &ldblTimeFrame,
                const long double &ldblStartTime, const long double &ldblTimeStep,
                const long double &ldblEndTime);
public:
    Q_INVOKABLE explicit CAmp(QGraphicsItem *parent = nullptr);

    const qreal& gain() const{return m_gain;}
	void setGain(const qreal &gain){m_gain = gain;}

    CElementOptionsWgt* optionsWidget(QWidget *parentWidget);
    void acceptOptions(CElementOptionsWgt *optWgt);
};

#endif // CAMP_H