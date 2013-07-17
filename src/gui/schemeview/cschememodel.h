#ifndef CSCHEMEMODEL_H
#define CSCHEMEMODEL_H

#include <QGraphicsScene>

class CElement;
class CScheme;

class CSchemeModel : public QGraphicsScene
{
    Q_OBJECT
private:
    CScheme *m_scheme;
public:
    explicit CSchemeModel(QObject *parent = 0);

    CScheme* scheme(void){return m_scheme;}
    void setScheme(CScheme *scheme);
private slots:
    void onSchemeDestroyed(void);
public slots:
    void addElement(CElement *element);
};

#endif // CSCHEMEMODEL_H