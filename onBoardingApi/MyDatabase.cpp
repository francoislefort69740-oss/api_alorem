#include <QSqlDatabase>

class MyDatabase
{
private :
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
public:
    MyDatabase() {
        db.setHostName("localhost");
        db.setPort(5432);
        db.setDatabaseName("test_db");
        db.setUserName("postgres");
        db.setPassword("1234");
    }

    bool open(){
        if(db.open()) {
            return true;
        }
        else {
            return false;
        }
    }
};