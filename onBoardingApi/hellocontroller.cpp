#include "hellocontroller.h"

#include <QHttpServerResponse>
#include <QHttpServerRequest>
#include <QJsonObject>
#include <QJsonDocument>

HelloController::HelloController(QHttpServer &server)
{
    registerRoutes(server);
}

void HelloController::registerRoutes(QHttpServer &server)
{
    server.route("/api/salut",
     QHttpServerRequest::Method::Get,
     []()
     {
         QJsonObject json;
         json["message"] = "hello world";

         return QHttpServerResponse(
             "application/json",
             QJsonDocument(json).toJson()
             );
     });

    server.route("/api/autre_chose",
     QHttpServerRequest::Method::Get,
     []()
     {
         QJsonObject json;
         json["message"] = "hello android";

         return QHttpServerResponse(
             "application/json",
             QJsonDocument(json).toJson()
             );
     });
}