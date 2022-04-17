#ifndef TESTBEANS_H
#define TESTBEANS_H

#include <QObject>
#include "dependencycontainer.h"

class TestSimpleBean : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE TestSimpleBean();
public slots:
    virtual QString testMethod();
};


class TestComplexBean : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString testProperty WRITE setTestProperty READ testProperty)
public:
    Q_INVOKABLE TestComplexBean(QObject *parent = nullptr);

    INJECT(TestSimpleBean*, child)

    Q_INVOKABLE QString testChildMethod();

    QString testProperty() const;
    void setTestProperty(const QString &testProperty);

private:
    QString _testProperty;

signals:

};

#endif // TESTBEANS_H
