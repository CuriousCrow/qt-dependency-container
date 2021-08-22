#ifndef SQLITEMODULE_H
#define SQLITEMODULE_H

#include "dbmsmodule.h"
#include <QObject>

class SqliteModule : public DbmsModule
{
    Q_OBJECT
public:
    Q_INVOKABLE SqliteModule();

    // DbmsModule interface
public:
    virtual QString driverName() override;
    virtual QString title() override;
};

#endif // SQLITEMODULE_H
