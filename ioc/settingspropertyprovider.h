#ifndef SETTINGSPROPERTYPROVIDER_H
#define SETTINGSPROPERTYPROVIDER_H

#include <QObject>
#include "dependencycontainer.h"
#include <QSettings>

class SettingsPropertyProvider : public AbstractPropertyProvider
{
public:
    SettingsPropertyProvider(QSettings *settings);
    virtual ~SettingsPropertyProvider();

    // AbstractPropertyProvider interface
public:
    virtual void init(const QVariantHash &props) override;
    virtual void setValue(QString propName, QVariant propValue) override;
    virtual bool contains(QString propName) override;
    virtual QVariant value(QString propName) override;
    virtual void reset() override;
private:
    QSettings* _settings;
};

#endif // SETTINGSPROPERTYPROVIDER_H
