#include <QtTest>
#include <QDebug>

// add necessary includes here
#include "dependencycontainer.h"
#include "testbeans.h"

#define BEAN_NAME "bean"
#define CHILD_BEAN "child"
#define COMPLEX_BEAN_NAME "complexBean"
#define CONSTRUCTOR_ARG_BEAN_NAME "constructorArgBean"

class InjectTest : public QObject
{
    Q_OBJECT

public:
    InjectTest();
    ~InjectTest();

private slots:
    void testSingleton();
    void testPrototype();
    void testExternalSingleton();
    void testInject();
    void testConstructorArg();
    void testPropertyInject();
};

InjectTest::InjectTest()
{
    qDebug() << "InjectTest constructor";
}

InjectTest::~InjectTest()
{
    qDebug() << "InjectTest destructor";
}

void InjectTest::testSingleton()
{
    DependencyContainer ioc;
    ioc.registerDependency(BEAN_NAME, CLASSMETA(QObject), InstanceMode::Singleton);
    QObject* singletonObj1 = ioc.dependency(BEAN_NAME);
    QObject* singletonObj2 = ioc.dependency(BEAN_NAME);
    Q_ASSERT(singletonObj1 == singletonObj2);
}

void InjectTest::testPrototype()
{
    DependencyContainer ioc;
    ioc.registerDependency(BEAN_NAME, CLASSMETA(QObject), InstanceMode::Prototype);
    QObject* obj1 = ioc.dependency(BEAN_NAME);
    QObject* obj2 = ioc.dependency(BEAN_NAME);
    Q_ASSERT(obj1 != obj2);
    delete obj1;
    delete obj2;
}

void InjectTest::testExternalSingleton()
{
    DependencyContainer ioc;
    QObject* extSingleton = new QObject(&ioc);
    ioc.registerSingletonObject(new DependencyMeta(BEAN_NAME, CLASSMETA(QObject)), extSingleton);
    QObject* obj1 = ioc.dependency(BEAN_NAME);
    Q_ASSERT(obj1 == extSingleton);
}

void InjectTest::testInject()
{
    DependencyContainer ioc;
    ioc.registerDependency(CHILD_BEAN, CLASSMETA(TestSimpleBean));
    ioc.registerDependency(COMPLEX_BEAN_NAME, CLASSMETA(TestComplexBean));

    TestComplexBean* complexObj = ioc.dependency<TestComplexBean>(COMPLEX_BEAN_NAME);
    Q_ASSERT(complexObj->testChildMethod() == CLASS(TestSimpleBean));
}

void InjectTest::testConstructorArg()
{
    SimpleObject* simpleObj = new SimpleObject();

    DependencyContainer ioc;
    ioc.registerDependency(CONSTRUCTOR_ARG_BEAN_NAME, CLASSMETA(TestNoDefaultConstructorBean));
    TestNoDefaultConstructorBean* bean
            = ioc.dependency<TestNoDefaultConstructorBean>(CONSTRUCTOR_ARG_BEAN_NAME, simpleObj);
    Q_ASSERT(bean != nullptr);
}

void InjectTest::testPropertyInject()
{
    QString propValue = "TestValue";

    DependencyContainer ioc;
    ioc.registerDependency(BEAN_NAME, CLASSMETA(TestComplexBean));
    ioc.setPropertyValue("testProperty", propValue);
    ioc.setErrorOnInjectFail(false);

    TestComplexBean* obj = ioc.dependency<TestComplexBean>(BEAN_NAME);
    Q_ASSERT(propValue == obj->testProperty());
}


QTEST_APPLESS_MAIN(InjectTest)

#include "tst_injecttest.moc"
