#ifndef POSTGRESMODULE_H
#define POSTGRESMODULE_H

#include "dbmsmodule.h"
#include <QObject>

class PostgresModule : public DbmsModule
{
    Q_OBJECT
public:
    Q_INVOKABLE PostgresModule();

    // DbmsModule interface
public:
    virtual QString driverName() override;
    virtual QString title() override;
};

#endif // POSTGRESMODULE_H
