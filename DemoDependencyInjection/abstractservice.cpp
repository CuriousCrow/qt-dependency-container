#include "abstractservice.h"
#include <QDebug>

AbstractService::AbstractService(QObject *parent) : QObject(parent)
{
}

MainService::MainService() : AbstractService()
{
}

void MainService::doSomething()
{
    qDebug() << "MainService::doSomething()";
    _childService->doSomething();
}

ChildService::ChildService() : AbstractService()
{
}

void ChildService::doSomething()
{
    qDebug() << "ChildService::doSomething()";
}
