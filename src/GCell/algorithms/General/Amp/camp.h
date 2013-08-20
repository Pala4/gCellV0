#ifndef CAMP_H
#define CAMP_H

#include "../../../scheme/algorithm/cdatatransmitter.h"

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
	virtual void proced(const int &timeFrame);
public:
    Q_INVOKABLE explicit CAmp(QGraphicsItem *parent = 0);

	const qreal& gain(void) const{return m_gain;}
	void setGain(const qreal &gain){m_gain = gain;}

	virtual CElementOptionsWgt* optionsWidget(QWidget *parentWidget);
	virtual void acceptOptions(CElementOptionsWgt *optWgt);
};

#endif // CAMP_H