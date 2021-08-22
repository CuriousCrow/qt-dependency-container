#include <QCoreApplication>
#include "dependencycontainer.h"
#include "abstractservice.h"

//Dependency bean-names
#define B_MAIN_SERVICE "mainService"
#define B_CHILD_SERVICE "childService"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DependencyContainer* iocContainer = new DependencyContainer(QCoreApplication::instance());

    //The order of dependencies plays no role if before any object instantiation. Otherwise should be watched carefully
    iocContainer->registerDependency(B_MAIN_SERVICE, CLASSMETA(MainService), InstanceMode::Singleton);
    iocContainer->registerDependency(B_CHILD_SERVICE, CLASSMETA(ChildService), InstanceMode::Singleton);

    //Will throw an assert-exception if any inject action fails
    //(You can comment regestering B_CHILD_SERVICE to look how it works)
    iocContainer->setErrorOnInjectFail(true);

    //Request of mainService (will create and inject childService as well, for details look debug messages)
    AbstractService* mainService = iocContainer->dependency<AbstractService>(B_MAIN_SERVICE);
    mainService->doSomething();
}
