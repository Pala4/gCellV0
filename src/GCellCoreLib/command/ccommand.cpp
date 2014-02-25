#include "ccommand.h"

using namespace gccore;

fValue* CCommand::varToVal(const QVariant &var)
{
    switch (var.type()) {
        case QVariant::Bool:
        return new Value<bool>(var.toBool());
        case QVariant::Int:
        return new Value<int>(var.toInt());
        case QVariant::Double:
        return new Value<double>(var.toDouble());
        case QVariant::String:
        return new Value<QString>(var.toString());
        default:
        break;
    }

    return nullptr;
}

bool CCommand::registerMethod(QObject *cmdReceiver, const char *method,
                              int &errcode, QString &error)
{
    Q_ASSERT(cmdReceiver != nullptr);
    Q_ASSERT(method != nullptr);

    int methodIdx = cmdReceiver->metaObject()->indexOfMethod(method);
    if (methodIdx == -1) {
        ++method; //scip code
        methodIdx = cmdReceiver->metaObject()->indexOfMethod(method);
        if (methodIdx == -1) {
            errcode = ERR_METHOD_NOT_FOUND;
            error = ERR_METHOD_NOT_FOUND_STR(method);
            return false;
        }
    }

    m_method = cmdReceiver->metaObject()->method(methodIdx);
    if (!m_method.isValid()) {
        errcode = ERR_METHOD_NOT_VALID;
        error = ERR_METHOD_NOT_VALID_STR(method);
        return false;
    }

    if (m_method.parameterCount() < 1) {
        errcode = ERR_METHOD_NOT_COMMAND;
        error = ERR_METHOD_NOT_COMMAND_STR(method);
        return false;
    }

    clrArgs();
    for (int argIdx = 1; argIdx < m_method.parameterCount(); ++argIdx) {
        Argument arg;
        arg.type = m_method.parameterType(argIdx);
        arg.typeName = QMetaType::typeName(arg.type);
        arg.name = m_method.parameterNames().at(argIdx);
        m_args << arg;
    }

    errcode = ERR_NOT_ERROR;
    error = ERR_NOT_ERROR_STR;
    return true;
}

void CCommand::unregisterMethod()
{
    m_method = QMetaMethod();
    clrArgs();
}

bool CCommand::fillArgs(const QStringList &args, int &errcode, QString &error)
{
    if (m_args.count() != args.count()) {
        errcode = ERR_PARAM_COUNT;
        error = ERR_PARAM_COUNT_STR;
        return false;
    }

    for (int argIdx = 0; argIdx < m_args.count(); ++argIdx) {
        QVariant var(args.at(argIdx));
        if (!var.convert(m_args.at(argIdx).type)) {
            errcode = ERR_TYPE_MISMATCH;
            error = ERR_TYPE_MISMATCH_STR(args.at(argIdx), m_args.at(argIdx).name,
                                          QVariant::typeToName(m_args.at(argIdx).type));
            return false;
        }

        m_args[argIdx].value = varToVal(var);
    }

    return true;
}

void CCommand::clrArgs()
{
    for (int ci = 0; ci < m_args.count(); ++ci) {
        if (m_args.at(ci).value != nullptr)
            delete m_args.at(ci).value;
    }
    m_args.clear();
}

QGenericArgument CCommand::arg(const int &index)
{
    Q_ASSERT(index >= 0);

    if (index >= m_args.count())
        return QGenericArgument();

    return QGenericArgument(m_args.at(index).typeName, m_args.at(index).value->voidVal());
}

QGenericReturnArgument CCommand::returnArg(QVariant &retVal)
{
    return Q_RETURN_ARG(QVariant, retVal);
}

bool CCommand::invoke(int &errcode, QString &error)
{
    Q_ASSERT(m_cmdReceiver != nullptr);
    Q_ASSERT(m_method.isValid());

    bool retVal;
    if (!m_method.invoke(m_cmdReceiver, Q_RETURN_ARG(bool, retVal), Q_ARG(CCommand*, this),
                         arg(0), arg(1), arg(2), arg(3), arg(4), arg(5), arg(6), arg(7), arg(8))) {
        errcode = ERR_METHOD_NOT_FOUND;
        error = ERR_METHOD_NOT_FOUND_STR(QString(m_method.name()));
        return false;
    }

    errcode = ERR_NOT_ERROR;
    error = ERR_NOT_ERROR_STR;
    return retVal;
}

CCommand::CCommand(QObject *cmdReceiver, const char *method, const QString &cmdName,
                   QObject *parent) : QObject(parent)
{
    m_cmdReceiver = cmdReceiver;

    m_cmdReceiver = nullptr;
    if ((cmdReceiver != nullptr) && (method != nullptr)) {
        int errcode = 0;
        QString error;
        setCmdReceiver(cmdReceiver, method, errcode, error);
    }

    setCmdName(cmdName);
}

CCommand::~CCommand()
{
    clrArgs();
}

QString CCommand::simpleCmdName(const QString &cmdName)
{
    return cmdName.simplified().toLower().replace(QLatin1Char(' '), QLatin1Char('_'));
}

bool CCommand::setCmdReceiver(QObject *cmdReceiver, const char *method,
                              int &errcode, QString &error)
{
    if ((m_cmdReceiver != nullptr) && (m_cmdReceiver == cmdReceiver))
        return true;

    if (m_cmdReceiver) {
        disconnect(m_cmdReceiver, SIGNAL(destroyed()), this, SLOT(onCmdReceiverDestroyed()));
        unregisterMethod();
    }
    m_cmdReceiver = cmdReceiver;
    if (m_cmdReceiver) {
        connect(m_cmdReceiver, SIGNAL(destroyed()), this, SLOT(onCmdReceiverDestroyed()));
        return registerMethod(m_cmdReceiver, method, errcode, error);
    }

    return true;
}

void CCommand::setCmdName(const QString &cmdName)
{
    m_cmdName = simpleCmdName(cmdName);
}

bool CCommand::execute(const QStringList &cmdList, int &errcode, QString &error)
{
    if (cmdList.count() < 1) {
        errcode = ERR_PARAM_COUNT;
        error = ERR_PARAM_COUNT_STR;
        return false;
    }

    QString cmd = cmdList.at(0);
    if (m_cmdName != cmd) {
        errcode = ERR_METHOD_NOT_FOUND;
        error = ERR_METHOD_NOT_FOUND_STR(cmd);
        return false;
    }

    setRespons(QString());

    QStringList args;
    if (cmdList.count() > 1)
        args = cmdList.mid(1, cmdList.count() - 1);

    if (!fillArgs(args, errcode, error))
        return false;

    return invoke(errcode, error);
}