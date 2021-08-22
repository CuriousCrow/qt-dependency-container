#include "postgresmodule.h"

PostgresModule::PostgresModule() : DbmsModule()
{

}

QString PostgresModule::driverName()
{
    return "QPSQL";
}

QString PostgresModule::title()
{
    return "PostgreSQL";
}
