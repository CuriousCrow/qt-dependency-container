#ifndef ABSTRACTDRIVER_H
#define ABSTRACTDRIVER_H

#include <QObject>

//DependencyContainer works only with QObject descendants
class AbstractDriver : public QObject
{
    //Dependency container work with QMetaObject. Do not forget Q_OBJECT macro
    Q_OBJECT
public:
    AbstractDriver(QObject *parent = nullptr);
    virtual ~AbstractDriver();
    virtual void doSomething() = 0;
};

class ImplementedDriver : public AbstractDriver
{
    Q_OBJECT
public:
    //DependencyContainer uses QMetaObject reflection and can call only marked with Q_INVOKABLE methods
    Q_INVOKABLE ImplementedDriver(QObject *parent = nullptr);

    // AbstractDriver interface
public:
    virtual void doSomething() override;
};

class TestDriver : public AbstractDriver
{
    Q_OBJECT
public:
    Q_INVOKABLE TestDriver(QObject *parent = nullptr);

    // AbstractDriver interface
public:
    virtual void doSomething() override;
};

#endif // ABSTRACTDRIVER_H
