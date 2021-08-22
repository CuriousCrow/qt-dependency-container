#include "dbmsmodule.h"
#include <QDebug>

DbmsModule::DbmsModule() : QObject()
{
    qDebug() << "New dbms module object created";
}

DbmsModule::~DbmsModule()
{
    qDebug() << "Dbms module object destroyed";
}
