#ifndef ABSTRACTSERVICE_H
#define ABSTRACTSERVICE_H

#include <QObject>
#include "dependencycontainer.h"

class AbstractService : public QObject
{
    Q_OBJECT
public:
    AbstractService(QObject *parent = nullptr);
    virtual void doSomething() = 0;
};

class ChildService : public AbstractService
{
    Q_OBJECT
public:
    Q_INVOKABLE ChildService();

    // AbstractService interface
public:
    virtual void doSomething() override;
};

class MainService : public AbstractService
{
    Q_OBJECT
public:
    Q_INVOKABLE MainService();
    //Inject method format:
    //inject_[beanName](QObject* obj)

    //Create injection method inject_childService(QObject* obj)
    //and class member AbstractService* _childService
    INJECT(AbstractService*, childService)

    // AbstractService interface
public:
    virtual void doSomething() override;
};

#endif // ABSTRACTSERVICE_H
