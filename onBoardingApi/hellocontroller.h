#ifndef HELLOCONTROLLER_H
#define HELLOCONTROLLER_H

#include <QHttpServer>

class HelloController
{
public:
    explicit HelloController(QHttpServer &server);
private:
    void registerRoutes(QHttpServer &server);
};

#endif // HELLOCONTROLLER_H
