#include "testbeans.h"

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

