#ifndef DEPENDENCYCONTAINER_H
#define DEPENDENCYCONTAINER_H

#include <QObject>
#include <QHash>
#include <QSet>
#include <QMap>
#include <QMultiHash>
#include <QVariantMap>


#define INJECT_PREFIX "inject_"
#define INJECT(Type, Name) Type _##Name; Q_INVOKABLE void inject_##Name(QObject* obj){ _##Name = static_cast<Type>(obj); }
#define INJECT_INITIALIZE(Type, Member, Name) Q_INVOKABLE void inject_##Name##_into_##Member(QObject* obj){ Member = static_cast<Type>(obj); }
#define CLASS(Type) Type::staticMetaObject.className()
#define CLASSMETA(Type) &Type::staticMetaObject

#define PRM_NAME "name"
#define PRM_CLASS "class"
#define PRM_MODE "mode"
#define PRM_CLASS_PREFIX "class_"

#define ERR_ONLY_QOBJECT "Only QObject descedants allowed"

enum InstanceMode {
    Singleton = 0,
    Prototype = 1
};

class DependencyMeta : public QVariantHash
{
public:
    DependencyMeta(QString name, const QMetaObject* metaObj, InstanceMode mode = Singleton);
    QString name() const;
    InstanceMode mode() const;
    QStringList ancessors() const;
    const QMetaObject* metaObj() const;

    DependencyMeta* setParam(QString name, QVariant value);

    bool hasMatch(QVariantHash params) const;
    bool exactClassMatch(const QString &className) const;

    QString toString() const;

private:
    QString _name;
    InstanceMode _instanceMode;
    const QMetaObject* _metaObj;
    void addSuperClass(const QMetaObject *metaObj, QStringList &list) const;
};

class AbstractPropertyProvider
{
public:
    virtual ~AbstractPropertyProvider();

    virtual void init(const QVariantHash &props) = 0;
    virtual void setValue(QString propName, QVariant propValue) = 0;
    virtual bool contains(QString propName) = 0;
    virtual QVariant value(QString propName) = 0;
    virtual void reset() = 0;
};

class VariantHashPropertyProvider : public AbstractPropertyProvider
{
    // AbstractPropertyProvider interface
public:
    virtual void init(const QVariantHash &props) override;
    virtual void setValue(QString propName, QVariant propValue) override;
    virtual bool contains(QString propName) override;
    virtual QVariant value(QString propName) override;
    virtual void reset() override;
private:
    QVariantHash _propHash;
};

class DependencyContainer : public QObject
{
    Q_OBJECT
public:
    explicit DependencyContainer(QObject *parent = nullptr);
    virtual ~DependencyContainer();
    //Registering
    DependencyMeta* registerDependency(DependencyMeta* meta);
    DependencyMeta* registerDependency(QString name, const QMetaObject* metaObj, InstanceMode mode = Singleton);
    DependencyMeta* registerSingletonObject(DependencyMeta* meta, QObject* object);
    //Management
    bool contains(const QString &name);
    void removeSingleton(QString name);
    void removeDependency(QString name);
    //Container settings
    void setPropertyProvider(AbstractPropertyProvider *propertyProvider);
    void setErrorOnInjectFail(bool value);
    void setPropertyValue(QString propName, QVariant propValue);
    //Dependency info
    QStringList allMetaInfo();
    QStringList allSingletonInfo();
    QStringList namesByClass(QString className);

    //Dependency request
    QObject* dependency(const QString &className, const QVariantHash &params);
    QObject* dependency(const QString &name);

    template<class T>
    QStringList namesByClass(){
        static_assert (std::is_base_of<QObject,T>::value, ERR_ONLY_QOBJECT);
        return namesByClass(CLASS(T));
    }

    template<class T>
    T* dependency(const QVariantHash &params){
        static_assert (std::is_base_of<QObject,T>::value, ERR_ONLY_QOBJECT);
        return static_cast<T*>(dependency(CLASS(T), params));
    }
    template<class T>
    T* dependency(const QString &name){
        static_assert (std::is_base_of<QObject,T>::value, ERR_ONLY_QOBJECT);
        return static_cast<T*>(dependency(name));
    }
signals:

private:
    void injectProperties(QObject* obj, const QString &name = "");
    bool injectProperty(QObject* obj, QString propName, QString objPropName = "");
    QHash<QString, DependencyMeta*> _metaByName; //itemByName
    QMultiHash<QString, DependencyMeta*> _metaByClass;
    QHash<QString, QObject*> _singletonHash; //all instaciated singletons
    AbstractPropertyProvider* _propertyProvider;
    bool _errorOnInjectFail = true;
};

#define DependencyContainer_iid "ru.levolex.dependencycontainer"
Q_DECLARE_INTERFACE(DependencyContainer, DependencyContainer_iid)


#endif // DEPENDENCYCONTAINER_H
