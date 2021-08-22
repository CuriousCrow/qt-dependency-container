# Welcome to Qt Dependency Container (by Lev Alekseevskiy)

Qt Dependency Container is an **extremely lightweight** set of C++ interfaces and classes implementing dependency container pattern for QObject descendants. Practically the core of the library consists of one main class DependencyContainer.

## Key features

 - Registering dependencies by *QMetaObject*
 - Registering already created instance as a singleton dependency
 - Instantiation of dependencies instances by request (by name, by classname)
 - Instantiation modes: *Singleton*, *Prototype*
 - Dependency beans overriding (by name)
 - Dependency injection by class method (method injection)
 - Abstract property provider interface for *QObject* property injection (VariantHashPropertyProvider as a default property provider implementation).
 - SettingsPropertyProvider is implementation that allows to use standard Qt *QSettings* class as a source for property values.
 - Informative console debug information
 - Convenience methods for retrieving registered dependencies info
 - Properties and flags to adjust container behaviour

## Limitations
- Only QObject descendants can be managed by Qt Dependency Container
- Injection methods should be marked with Q_INVOKABLE macro
- Currently only method (setter) injection supported
- Thread safe workflows are not developed and tested yet
- Dependency injection by name only (*planned in future*)
- Only default constructor is used for creating new instances

## How to use
To use Qt Dependency Container one should simply:
 1. Add ioc folder to your project sources directory
 2. Include library pri-file to your project pro-file (for example `include(../ioc/ioc.pri)`)

### Registering dependency in container
##### *Important!*
Qt Dependency Container uses QObject metadata for making reflection calls. That`s why one should be always aware that dependency class: 
- Inherit from QObject class
- Marked with Q_OBJECT macro
- Constructor and inject methods marked with Q_INVOKABLE macro

Use `DependencyContainer::registerDependency()` method to register dependency in the DependencyContainer:

      //Creating dependency container object
      DependencyContainer objContainer = new DependencyContainer(this);
      //register by new DependencyMeta object
      objContainer->registerDependency(new DependencyMeta("postgres",  CLASSMETA(CustomPostgres), InstanceMode::Singleton));
      //register with convenience method
      objContainer->registerDependency("firebird",  CLASSMETA(CustomFirebird), InstanceMode::Singleton);

`CLASSMETA` - is a convenience macro that receives Type as a parameter and converts it to `&Type::staticMetaObject`
`CLASS` - convenience macro to get class name as a QString from Type.

*Register existing object as a Singleton dependency*

      CustomProperty* prop1 = new CustomProperty();
      prop2->setName("Postgres properties");
      objContainer->registerSingletonObject(new DependencyMeta("PostgresProps", CLASSMETA(CustomProperty)), prop1);
        
### Requesting dependency from container
*Request dependency by name*

    MyCustom* myCustom = objContainer->dependency<MyCustom>("postgres");
    myCustom->doSomething();

*Request dependency by class*

      QVariantHash params;
      params.insert("primary", false);
      DbmsModule* module = objContainer->dependency<DbmsModule>(params);

*Get all registered dependencies by class*

      QStringList  beanNames  =  objContainer->namesByClass(CLASS(MenuItemClass));
      foreach(QString name, beanNames)  {
	      MenuItemClass* item = objContainer<MenuItemClass>(name);
	      menu->addItem(item);
      }

  

