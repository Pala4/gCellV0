#ifndef CENGINE_H
#define CENGINE_H

#include <QObject>

class CScheme;

class CEngine : public QObject
{
    Q_OBJECT
private:
    CScheme *m_scheme;
public:
    explicit CEngine(QObject *parent = 0);

    CScheme* scheme(void){return m_scheme;}
    void setScheme(CScheme *scheme);
private slots:
    void onSchemeDestroyed(void);
public slots:
    void calc(void);
};

#endif // CENGINE_H