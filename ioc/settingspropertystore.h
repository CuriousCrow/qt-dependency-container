#ifndef SETTINGSPROPERTYSTORE_H
#define SETTINGSPROPERTYSTORE_H

#include <QObject>
#include "dependencycontainer.h"
#include <QSettings>

class SettingsPropertyStore : public AbstractPropertyStore
{
public:
    SettingsPropertyStore(QSettings *settings);
    virtual ~SettingsPropertyStore();

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

#endif // SETTINGSPROPERTYSTORE_H
