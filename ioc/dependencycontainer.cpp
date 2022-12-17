#include "dependencycontainer.h"
#include <QDebug>
#include <QMetaMethod>


DependencyContainer::DependencyContainer(QObject *parent) : QObject(parent)
{
    qDebug() << "Dependency container constructor";
    _propertyProvider = new VariantHashPropertyStore();
}

DependencyContainer::~DependencyContainer()
{
    qDebug() << "Dependency container destructor";
    delete _propertyProvider;

    qDebug() << "Destroying all registered singletons";
    while(!_singletonHash.isEmpty()) {
        QStringList keys = _singletonHash.keys();
        QString name = keys.first();
        QObject* obj = _singletonHash.value(name);
        _singletonHash.remove(name);
        delete obj;
        qDebug() << name << "deleted!";
    }
}

DependencyMeta *DependencyContainer::registerDependency(DependencyMeta *meta)
{
    QString checkResult = dependencyCheck(meta);
    if (!checkResult.isEmpty()) {
      qDebug() << "Dependency check failed:" << meta << checkResult;
      return meta;
    }
    qDebug() << QString("Registering dependency: %1").arg(meta->toString());
    if (contains(meta->name())) {
        qDebug() << QString("Overriding previous %1 dependency").arg(meta->name());
        removeDependency(meta->name());
    }
    QStringList classes = meta->ancessors();
    foreach(QString className, classes) {
        _metaByClass.insert(className, meta);
    }
    _metaByName.insert(meta->name(), meta);
    return meta;
}

DependencyMeta *DependencyContainer::registerDependency(QString name, const QMetaObject *metaObj, InstanceMode mode)
{
    return registerDependency(new DependencyMeta(name, metaObj, mode));
}

DependencyMeta *DependencyContainer::registerSingletonObject(DependencyMeta *meta, QObject *object)
{
    if (meta->mode() != InstanceMode::Singleton) {
        qWarning() << "RegisterSingletonObject method only with singleton mode";
        return nullptr;
    }
    object->setObjectName(meta->name());
    object->setProperty(PRM_MODE, Singleton);
    object->setProperty(PRM_BEAN_NAME, meta->name());
    _singletonHash.insert(meta->name(), object);
    return registerDependency(meta);
}

bool DependencyContainer::contains(const QString &name)
{
    return _metaByName.contains(name);
}

void DependencyContainer::removeSingleton(QString name)
{
    _singletonHash.remove(name);
}

void DependencyContainer::removeDependency(QString name)
{
    DependencyMeta* meta = _metaByName.value(name);
    QStringList classes = meta->ancessors();
    foreach(QString className, classes) {
        _metaByClass.remove(className, meta);
    }
    delete _metaByName.take(name);
    _singletonHash.remove(name);
}

void DependencyContainer::setPropertyProvider(AbstractPropertyStore *propertyProvider)
{
    delete _propertyProvider;
    _propertyProvider = propertyProvider;
}

void DependencyContainer::setErrorOnInjectFail(bool value)
{
    _errorOnInjectFail = value;
}

void DependencyContainer::setPropertyValue(QString propName, QVariant propValue)
{
    _propertyProvider->setValue(propName, propValue);
}

QStringList DependencyContainer::allMetaInfo()
{
    QStringList lines;
    foreach(DependencyMeta* meta, _metaByName) {
        lines.append(meta->toString());
    }
    return lines;
}

QStringList DependencyContainer::allSingletonInfo()
{
    QStringList lines;
    QList<QObject*> values = _singletonHash.values();
    foreach(QObject* obj, values){
        lines.append(QString("%1: %2").arg(obj->objectName(), obj->metaObject()->className()));
    }
    return lines;
}

QStringList DependencyContainer::namesByClass(QString className)
{
    QStringList resList;
    foreach(DependencyMeta* meta, _metaByClass.values(className)) {
        resList.append(meta->name());
    }
    return resList;
}

QObject *DependencyContainer::dependency(const QString &className, const QVariantHash &params, const QObject *arg)
{
    qDebug() << "Dependency request of class:" << className;
    if (!_metaByClass.contains(className)) {
        qWarning() << "No dependency registered implementing:" << className;
        return nullptr;
    }
    QList<DependencyMeta*> metaList = _metaByClass.values(className);
    foreach(DependencyMeta* meta, metaList) {
        if (meta->exactClassMatch(className))
            return dependency(meta->name(), arg);
        if (meta->hasMatch(params))
            return dependency(meta->name(), arg);
    }
    qWarning() << "Meta object not registered:" << className << params;
    return nullptr;
}

QObject *DependencyContainer::dependency(const QString &name, const QObject *arg)
{
    qDebug() << "Dependency request:" << name;
    if (!_metaByName.contains(name)) {
        qCritical() << QString("Dependency %1 not registered in the dependency container").arg(name);
        return nullptr;
    }
    DependencyMeta* meta = _metaByName.value(name);
    const QMetaObject* metaObj = meta->metaObj();

    //If Singleton mode, check if already created
    if (meta->mode() == InstanceMode::Singleton) {
        if (_singletonHash.contains(name)) {
            qDebug() << QString("Singleton object found: %1").arg(name);
            return _singletonHash.value(name);
        }
    }
    if (metaObj->constructorCount() == 0) {
        qCritical() << QString("No constructor for type %1 is available. Please, mark at least one constructor with Q_INVOKABLE macro").arg(metaObj->className());
        return nullptr;
    }

    //Check for default constructor
    bool hasMatchConstructor = false;
    QByteArray paramType = 0;
    for(int i=0; i<metaObj->constructorCount(); i++) {
        QMetaMethod ctor = metaObj->constructor(i);
        if (arg == nullptr) {
            if (ctor.parameterCount() == 0) {
                hasMatchConstructor = true;
                break;
            }
        }
        else if (ctor.parameterCount() > 0) {
            QString argType = QString::fromLatin1(ctor.parameterTypes().at(0)).remove("*").trimmed();
            paramType = ctor.parameterTypes().at(0);
            if (arg->inherits(argType.toLatin1())) {
                hasMatchConstructor = true;
                break;
            }
        }
    }
    if (!hasMatchConstructor) {
        qWarning() << QString("No matching constructor for class %1 or it has not been marked with Q_INVOKABLE macro")
                      .arg(metaObj->className());
        return nullptr;
    }

    QObject* newObj = metaObj->newInstance(QGenericArgument(paramType, &arg));
    //Meta information of new object
    newObj->setProperty(PRM_MODE, meta->mode());
    newObj->setProperty(PRM_BEAN_NAME, meta->name());

    if (meta->mode() == Singleton)
      newObj->setObjectName(meta->name());
    injectProperties(newObj, name);
    if (!newObj) {
        qWarning() << "Cant instantiate object:" << name;
        return nullptr;
    }
    qDebug() << "Bean created of metaobject:" << name;
    for(int i=0; i<metaObj->methodCount(); i++) {
        QMetaMethod method = metaObj->method(i);
        QString methodName = method.name();
        if (methodName.contains(INJECT_PREFIX)) {
            qDebug() << "Inject method detected:" << methodName;
            QString injectCommand = methodName;
            QStringList injectTokens = injectCommand.split("_", Qt::SkipEmptyParts);
            //Not injectMethod
            if (injectTokens.size() < 2)
                continue;
            QString beanName;
            if (injectTokens.contains("by"))
                beanName = injectTokens[injectTokens.indexOf("by") + 1];
            else
                beanName = injectTokens[1];
            QObject* dependencyObj = dependency(beanName);
            if (!dependencyObj) {
                Q_ASSERT_X(!_errorOnInjectFail, "DependencyContainer::dependency()", "Field injection failed. Fix the bug or disable errorOnInjectFail flag.");
                continue;
            }
            newInjectProcessing(dependencyObj, newObj);
            QString argType = QString::fromLatin1(method.parameterTypes().at(0)).remove("*").trimmed();

            if (!dependencyObj->inherits(argType.toLatin1())) {
                QString errStr = "Bean %1: Inject method %2 expects arg type %3 descedant. Found bean (%4) instead";
                qCritical() << errStr.arg(name, methodName, argType, beanName, dependencyObj->metaObject()->className());
                continue;
            }

            bool injectResult = method.invoke(newObj, Qt::DirectConnection, QGenericArgument(method.parameterTypes().at(0), &dependencyObj));

            qDebug() << QString("Inject method %1 result: %2").arg(methodName).arg(injectResult);
        }

    }
    newInstanceProccessing(newObj);
    //Save singleton object
    if (meta->mode() == InstanceMode::Singleton) {
        newObj->setParent(this);
        _singletonHash.insert(name, newObj);
    }
    qDebug() << "Dependency" << name << "successfully instatiated" << "\n";
    return newObj;
}

QString DependencyContainer::dependencyCheck(const DependencyMeta *meta)
{
  Q_UNUSED(meta)
  //By default no dependency check
  return "";
}

bool DependencyContainer::dependencyFilter(const DependencyMeta *meta)
{
  Q_UNUSED(meta)
  //By default no filter
  return true;
}

void DependencyContainer::newInstanceProccessing(QObject *obj)
{
  Q_UNUSED(obj)
  //By default do nothing
}

void DependencyContainer::newInjectProcessing(QObject *injectedObj, QObject *targetObj)
{
  Q_UNUSED(injectedObj)
  Q_UNUSED(targetObj)
  //By default do nothing
  //Here can be implemented parenting logic, logging etc
}

void DependencyContainer::injectProperties(QObject* obj, const QString &name)
{
    Q_ASSERT_X(obj, "DependencyContainer::injectProperties", "Trying to inject properties to NULLPTR");

    for(int i=0; i<obj->metaObject()->propertyCount(); i++) {
        QString propName = obj->metaObject()->property(i).name();
        if (propName == "objectName")
            continue;
        //Trying to inject bean-specific properties
        if (!injectProperty(obj, name + "." + propName, propName))
            injectProperty(obj, propName);
    }
}

bool DependencyContainer::injectProperty(QObject *obj, QString propName, QString objPropName)
{
    if (objPropName.isEmpty())
        objPropName = propName;
    if (_propertyProvider->contains(propName)) {
        QVariant propValue = _propertyProvider->value(propName);
        qDebug() << "Injecting property:" << propName << "=" << propValue;
        obj->setProperty(objPropName.toLatin1(), propValue);
        return true;
    }
    return false;
}


DependencyMeta::DependencyMeta(QString name, const QMetaObject* metaObj, InstanceMode mode) : QVariantHash(),
    _name(name), _instanceMode(mode), _metaObj(metaObj)
{

}

QString DependencyMeta::name() const
{
    return _name;
}

InstanceMode DependencyMeta::mode() const
{
    return _instanceMode;
}

QStringList DependencyMeta::ancessors() const
{
//    qDebug() << "Bean" << name() << "implements:";
    QStringList list;
    addSuperClass(_metaObj, list);
//    qDebug() << "end";
    return list;
}

const QMetaObject* DependencyMeta::metaObj() const
{
    return _metaObj;
}

DependencyMeta *DependencyMeta::setParam(QString name, QVariant value)
{
    insert(name, value);
    return this;
}


bool DependencyMeta::hasMatch(QVariantHash params) const
{
    QStringList keys = params.keys();
    for(int i=0; i<keys.count(); i++) {
        QString key = keys.value(i);
        if (value(key) != params.value(key))
            return false;
    }
    return true;
}

bool DependencyMeta::exactClassMatch(const QString &className) const
{
    return contains(PRM_CLASS) && value(PRM_CLASS).toString() == className;
}

QString DependencyMeta::toString() const
{
    QString pattern = "%1=%2";
    QStringList params;
    params.append(pattern.arg("name", name()));
    params.append(pattern.arg("class", metaObj()->className()));
    params.append(pattern.arg("instanceMode", _instanceMode == InstanceMode::Singleton ? "Singleton" : "Prototype"));
    return params.join(", ");
}

void DependencyMeta::addSuperClass(const QMetaObject *metaObj, QStringList &list) const
{
    list.append(metaObj->className());
//    qDebug() << metaObj->className();
    if (metaObj->superClass()) {
        addSuperClass(metaObj->superClass(), list);
    }
}

void VariantHashPropertyStore::init(const QVariantHash &props)
{
    Q_UNUSED(props)
}

void VariantHashPropertyStore::setValue(QString propName, QVariant propValue)
{
    _propHash.insert(propName, propValue);
}

bool VariantHashPropertyStore::contains(QString propName)
{
    return _propHash.contains(propName);
}

QVariant VariantHashPropertyStore::value(QString propName)
{
    return _propHash.value(propName);
}

void VariantHashPropertyStore::reset()
{
}

AbstractPropertyStore::~AbstractPropertyStore()
{
}
