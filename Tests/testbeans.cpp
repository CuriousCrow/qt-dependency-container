#include "testbeans.h"
#include <QDebug>

TestComplexBean::TestComplexBean(QObject *parent) : QObject(parent)
{

}

QString TestComplexBean::testChildMethod()
{
    return _child->testMethod();
}

QString TestComplexBean::testProperty() const
{
    return _testProperty;
}

void TestComplexBean::setTestProperty(const QString &testProperty)
{
    _testProperty = testProperty;
}

TestSimpleBean::TestSimpleBean() : QObject(nullptr)
{
}

QString TestSimpleBean::testMethod()
{
    return QString::fromLatin1(CLASS(TestSimpleBean));
}


TestNoDefaultConstructorBean::TestNoDefaultConstructorBean(SimpleObject *argObj) : QObject()
{
    qDebug() << "Constructor arg: " << argObj->objectName();
}

SimpleObject::SimpleObject() : QObject()
{
    qDebug() << "Simple object constructor";
    setObjectName("Simple object");
}
