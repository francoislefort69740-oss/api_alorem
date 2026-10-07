#include <QCoreApplication>
#include <QHttpServer>
#include <QHttpServerResponse>
#include <QHttpServerRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QHostAddress>
#include <QTcpServer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QJsonDocument>
#include <QHostInfo>
#include <QNetworkInterface>

#include "commanderepository.h"
#include "MyDatabase.cpp"
#include "hellocontroller.h"
#include "commandecontroller.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Test base de données

    MyDatabase database = MyDatabase();

    if (!database.open()) {
        qCritical() << "Erreur PostgreSQL :";
        return 1;
    }

    qInfo() << "Connexion PostgreSQL OK";

    QHttpServer server;

    HelloController helloController(server);

    CommandeRepository commandeRepository;

    CommandeService commandeService(commandeRepository);

    CommandeController commandeController(server, commandeService);

    auto tcpServer = std::make_unique<QTcpServer>();

    if (!tcpServer->listen(QHostAddress::Any, 8080)
        || !server.bind(tcpServer.get()))
    {
        qCritical() << "Impossible de démarrer le serveur";
        qInfo() << "Adresse IP utilisée par la machine :"
                << QHostInfo::localHostName();
        return 1;
    }

    qInfo() << "Serveur démarré sur :";
    qInfo() << "  Local : http://localhost:" << tcpServer->serverPort();

    const auto addresses = QNetworkInterface::allAddresses();

    for (const QHostAddress &address : addresses)
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol &&
            address != QHostAddress::LocalHost)
        {
            qInfo() << "  Réseau :"
                    << "http://" + address.toString()
                           + ":" + QString::number(tcpServer->serverPort());
        }
    }
    tcpServer.release();

    return app.exec();
}
