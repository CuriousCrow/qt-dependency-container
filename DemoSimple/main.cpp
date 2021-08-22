#include <QCoreApplication>
#include "dependencycontainer.h"
#include "abstractdriver.h"

#define DRIVER_BEAN "driver"
//Uncomment TESTMODE define for switching to test dependencies
//#define TESTMODE

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DependencyContainer* iocContainer = new DependencyContainer(QCoreApplication::instance());
//Conditional compilation for switching between dependencies
#ifndef TESTMODE
    iocContainer->registerDependency(DRIVER_BEAN, CLASSMETA(ImplementedDriver), InstanceMode::Singleton);
#else
    iocContainer->registerDependency(DRIVER_BEAN, CLASSMETA(TestDriver), InstanceMode::Singleton);
#endif
    AbstractDriver* driver = iocContainer->dependency<AbstractDriver>(DRIVER_BEAN);
    driver->doSomething();

    //By default DependencyContainer will be a parent for all singletons it produces
    //so they will be destroyed automatically
}
