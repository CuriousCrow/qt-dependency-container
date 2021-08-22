#include "abstractdriver.h"
#include <QDebug>

AbstractDriver::AbstractDriver(QObject *parent) : QObject(parent)
{
}

AbstractDriver::~AbstractDriver()
{
    qDebug() << "Driver destructor";
}

ImplementedDriver::ImplementedDriver(QObject *parent) : AbstractDriver(parent)
{
}

void ImplementedDriver::doSomething()
{
    qDebug() << "Hello from ImplementedDriver::doSomething()";
}

TestDriver::TestDriver(QObject *parent) : AbstractDriver(parent)
{
}

void TestDriver::doSomething()
{
    qDebug() << "Hello from TestDriver::doSomething()";
}
