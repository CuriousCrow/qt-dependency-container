#ifndef ABSTRACTSERVICE_H
#define ABSTRACTSERVICE_H

#include <QObject>
#include "dependencycontainer.h"

class AbstractService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool verboseMode READ verboseMode WRITE setVerboseMode)
public:
    AbstractService(QObject *parent = nullptr);
    virtual void doSomething() = 0;

    bool verboseMode() const;
    void setVerboseMode(bool verboseMode);

private:
    bool _verboseMode = false;
};

class ServiceImpl : public AbstractService
{
    Q_OBJECT
public:
    Q_INVOKABLE ServiceImpl();

    // AbstractService interface
public:
    virtual void doSomething() override;
};

#endif // ABSTRACTSERVICE_H
