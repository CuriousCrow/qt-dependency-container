#include "sqlitemodule.h"


SqliteModule::SqliteModule() : DbmsModule()
{

}

QString SqliteModule::driverName()
{
    return "QSQLITE";
}

QString SqliteModule::title()
{
    return "Sqlite";
}
