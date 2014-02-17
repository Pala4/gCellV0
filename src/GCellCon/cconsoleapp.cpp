#include "cconsoleapp.h"

#include "iosystem/ciosystem.h"
#include "cconio.h"
#include "network/csocket.h"

CConsoleApp::CConsoleApp(int &argc, char **argv) : QCoreApplication(argc, argv)
{
    setObjectName(QStringLiteral("CConsoleApp"));

    m_conIO = new CConIO(this);
    m_conIO->setObjectName(QStringLiteral("conIO"));

    m_ioSystem = new CIOSystem(this);
    m_ioSystem->setObjectName(QStringLiteral("ioSystem"));

    m_socket = new CSocket(this);

    m_conIO->connectForwardObject(m_ioSystem);

    connect(m_conIO, SIGNAL(halted()), this, SLOT(quit()), Qt::DirectConnection);

    connect(m_conIO, SIGNAL(transmitBackwardRespons(QString)),
            m_conIO, SLOT(receiveBackwardRespons(QString)));
    connect(m_ioSystem, SIGNAL(transmitBackwardRespons(QString)),
            m_conIO, SLOT(receiveBackwardRespons(QString)));
    connect(m_socket, SIGNAL(transmitBackwardRespons(QString)),
            m_conIO, SLOT(receiveBackwardRespons(QString)));

    m_ioSystem->registerTransaction(m_conIO, m_conIO, QStringLiteral("get_con_info"),
                                    static_cast<int>(CConIO::GetInfo));
    m_ioSystem->registerTransaction(m_conIO, m_conIO, QStringLiteral("halt"),
                                    static_cast<int>(CConIO::Halt));
    m_ioSystem->registerTransaction(m_socket, m_conIO, QStringLiteral("connect"),
                                    static_cast<int>(CSocket::Connect));
    m_ioSystem->registerTransaction(m_socket, m_conIO, QStringLiteral("disconnect"),
                                    static_cast<int>(CSocket::Disconnect));

    connect(this, SIGNAL(aboutToQuit()), this, SLOT(onAboutToQuit()));
}

void CConsoleApp::onAboutToQuit()
{
    if (m_socket != nullptr) {
        if (m_socket->state() == QAbstractSocket::ConnectedState) {
            m_socket->disconnectFromHost();
            //wait
            //close
        }
    }
    if (m_conIO != nullptr) {
        m_conIO->receiveBackwardRespons(tr("System halted"));
        m_conIO->receiveBackwardRespons(tr("Power down"));
    }
}

void CConsoleApp::start()
{
    if (m_conIO != nullptr) {
        m_conIO->start();
        m_conIO->receiveBackwardQuery(QStringLiteral("get_con_info"));
    }

    m_socket->connectToHost("127.0.0.1:33538");
}