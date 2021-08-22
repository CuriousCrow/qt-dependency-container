#include "customproperty.h"

CustomProperty::CustomProperty() : QObject()
{

}

void CustomProperty::setName(QString name)
{
    _name = name;
}

QString CustomProperty::name()
{
    return _name;
}

QString CustomProperty::author() const
{
    return _author;
}

void CustomProperty::setAuthor(const QString &author)
{
    _author = author;
}
