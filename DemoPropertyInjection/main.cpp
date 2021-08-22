#include <QCoreApplication>
#include "dependencycontainer.h"
#include "abstractservice.h"
#include "settingspropertyprovider.h"

//Dependency bean-names
#define B_SERVICE_ONE "beanOne"
#define B_SERVICE_TWO "beanTwo"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //In constructor DependencyContainer create default PropertyProvider (based on QVariantHash)
    DependencyContainer* iocContainer = new DependencyContainer(QCoreApplication::instance());
    //Setting default property value for all dependencies who needs it
    iocContainer->setPropertyValue("verboseMode", true);
    //Specifying property value for certain dependency
    iocContainer->setPropertyValue("beanTwo.verboseMode", false);

    //SettingsPropertyProvider used QSettings as a source of properties
//    iocContainer->setPropertyProvider(new SettingsPropertyProvider(new QSettings("app.ini", QSettings::IniFormat)));

    //It's fully normal to register several dependencies of one class
    iocContainer->registerDependency(B_SERVICE_ONE, CLASSMETA(ServiceImpl), InstanceMode::Singleton);
    iocContainer->registerDependency(B_SERVICE_TWO, CLASSMETA(ServiceImpl), InstanceMode::Singleton);

    //DependencyContainer::namesByClass() is usefull for cycle through all dependencies implementing some interface
    foreach(QString beanName, iocContainer->namesByClass(CLASS(AbstractService))) {
        AbstractService* service = iocContainer->dependency<AbstractService>(beanName);
        service->doSomething();
    }
}
