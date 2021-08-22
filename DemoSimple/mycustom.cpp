#include "mycustom.h"
#include <QDebug>

MyCustom::MyCustom(QObject* parent) : QObject(parent)
{

}

void MyCustom::printSomething()
{
    qDebug() << "Injected prop:" << _prop->name();
}

CustomFirebird::CustomFirebird(QObject *parent) : MyCustom(parent)
{

}

CustomPostgres::CustomPostgres(QObject *parent) : MyCustom(parent)
{

}

QString CustomPostgres::author() const
{
    return _author;
}

void CustomPostgres::setAuthor(QString author)
{
    qDebug() << "Setting author:" << author;
    _author = author;
}
