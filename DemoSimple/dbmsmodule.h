#ifndef DBMSMODULE_H
#define DBMSMODULE_H

#include <QObject>
#include "customproperty.h"
#include "dependencycontainer.h"

class DbmsModule : public QObject
{
    Q_OBJECT
public:
    explicit DbmsModule();
    virtual ~DbmsModule();
    Q_INVOKABLE virtual QString driverName() = 0;
    Q_INVOKABLE virtual QString title() = 0;
    INJECT(CustomProperty*, customProperty)
    CustomProperty* customProp() { return _customProperty; }
signals:

};

#endif // DBMSMODULE_H
