#ifndef CCOMMAND_H
#define CCOMMAND_H

#include "gcellcorelib_global.h"

#include <QObject>

#include <QVariant>
#include <QVector>
#include <QMetaMethod>

namespace gccore {

#define ERR_NOT_ERROR 0
#define ERR_NOT_ERROR_STR tr("Not error")
#define ERR_METHOD_NOT_FOUND 1
#define ERR_METHOD_NOT_FOUND_STR(methodName) tr("Error: Method [%1] not found").arg(methodName)
#define ERR_METHOD_NOT_VALID 2
#define ERR_METHOD_NOT_VALID_STR(methodName) tr("Error: Method [%1] not valid").arg(methodName)
#define ERR_METHOD_NOT_COMMAND 3
#define ERR_METHOD_NOT_COMMAND_STR(methodName) tr("Error: Method [%1] is not a command") \
                                                .arg(methodName)
#define ERR_PARAM_COUNT 4
#define ERR_PARAM_COUNT_STR tr("Error: Invalid parametrs count")
#define ERR_TYPE_MISMATCH 5
#define ERR_TYPE_MISMATCH_STR(val, arg_name, arg_type) \
    tr("Error: Type mismatch [val = %1, arg. name = %2, req. arg. type = %3]") \
    .arg(val) \
    .arg(arg_name) \
    .arg(arg_type)
#define COMMAND_LAST_ERROR ERR_TYPE_MISMATCH

class fValue
{
public:
    fValue(){}
    virtual ~fValue(){}

    virtual void* voidVal() = 0;
};

template<typename T>
class Value : public fValue
{
private:
    T m_value;
public:
    Value(T value) : fValue(){m_value = value;}
    void* voidVal(){return reinterpret_cast<void*>(&m_value);}
};

class GCELLCORELIBSHARED_EXPORT CCommand : public QObject
{
    Q_OBJECT
private:
    QObject *m_cmdReceiver;
    QMetaMethod m_method;
    QString m_cmdName;

    struct Argument
    {
        Argument(){value = nullptr;}
        int type;
        const char *typeName;
        QString name;
        fValue *value;
    };
    QVector<Argument> m_args;

    QString m_senderID;
    QString m_respons;

    fValue* varToVal(const QVariant &var);

    bool registerMethod(QObject *cmdReceiver, const char *method, int &errcode, QString &error);
    void unregisterMethod();
    bool fillArgs(const QStringList &args, int &errcode, QString &error);
    void clrArgs();
    QGenericArgument arg(const int &index);
    QGenericReturnArgument returnArg(QVariant &retVal);

    bool invoke(int &errcode, QString &error);
public:
    explicit CCommand(QObject *cmdReceiver = nullptr, const char *method = nullptr,
                      const QString &cmdName = QString(),
                      QObject *parent = nullptr);
    virtual ~CCommand();

    static QString simpleCmdName(const QString &cmdName);

    QObject* cmdReceiver() const{return m_cmdReceiver;}
    bool setCmdReceiver(QObject *cmdReceiver, const char *method, int &errcode, QString &error);

    const QString& cmdName() const{return m_cmdName;}
    void setCmdName(const QString &cmdName);

    const QString& senderID() const{return m_senderID;}
    void setSenderID(const QString &senderID){m_senderID = senderID;}
    const QString& respons() const{return m_respons;}
    void setRespons(const QString &respons){m_respons = respons;}
private slots:
    void onCmdReceiverDestroyed(){m_cmdReceiver = nullptr;}
public slots:
    bool execute(const QStringList &cmdList, int &errcode, QString &error);
};

}

#endif // CCOMMAND_H