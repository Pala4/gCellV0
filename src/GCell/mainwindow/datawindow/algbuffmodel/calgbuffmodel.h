#ifndef CALGBUFFMODEL_H
#define CALGBUFFMODEL_H

#include <QStandardItemModel>

class CAlgorithm;

class CAlgBuffModel : public QStandardItemModel
{
	Q_OBJECT
public:
	explicit CAlgBuffModel(QObject *parent = 0);

	void setAlgorithms(const QList<CAlgorithm*> &algs);
};

#endif // CALGBUFFMODEL_H