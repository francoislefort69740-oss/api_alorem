#pragma once

#include <QHttpServer>
#include <QJsonObject>

#include "CommandeService.h"

class CommandeController
{
public:
    CommandeController(QHttpServer &server,
                       CommandeService &service);

private:
    void registerRoutes(QHttpServer &server);

    std::optional<CommandeComplete> parseCommandeComplete(
        const QJsonObject &json);

    CommandeService &service;
};
