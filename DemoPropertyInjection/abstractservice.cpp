#include "abstractservice.h"
#include <QDebug>

AbstractService::AbstractService(QObject *parent) : QObject(parent)
{
}

bool AbstractService::verboseMode() const
{
    return _verboseMode;
}

void AbstractService::setVerboseMode(bool verboseMode)
{
    qDebug() << "Verbose mode:" << verboseMode;
    _verboseMode = verboseMode;
}

ServiceImpl::ServiceImpl() : AbstractService()
{
}

void ServiceImpl::doSomething()
{
    if (verboseMode())
        qDebug() << "Verbose mode:" << "Starting...";
    qDebug() << "ChildService::doSomething()";
    if (verboseMode())
        qDebug() << "Verbose mode:" << "End of method...";
}
