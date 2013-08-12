#ifndef VARUTIL_H
#define VARUTIL_H

#include <QVariant>

QString variantToString(const QVariant &v);
QVariant stringToVariant(const QString &s);

#endif // VARUTIL_H