#ifndef MYCUSTOM_H
#define MYCUSTOM_H

#include <QObject>
#include "customproperty.h"
#include "dependencycontainer.h"

class MyCustom : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE MyCustom(QObject* parent = nullptr);
    void printSomething();
protected:
    CustomProperty* _prop;

signals:

};

class CustomFirebird : public MyCustom
{
    Q_OBJECT
public:
    Q_INVOKABLE CustomFirebird(QObject* parent = nullptr);
    INJECT_INITIALIZE(CustomProperty*, _prop, FirebirdProps)
};

class CustomPostgres : public MyCustom
{
    Q_OBJECT
    Q_PROPERTY(QString author READ author WRITE setAuthor)
    QString m_author;

public:
    Q_INVOKABLE CustomPostgres(QObject* parent = nullptr);
    INJECT_INITIALIZE(CustomProperty*, _prop, PostgresProps)
    QString author() const;
    void setAuthor(QString author);
private:
    QString _author;
};

#endif // MYCUSTOM_H
