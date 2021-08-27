#include "settingspropertystore.h"

SettingsPropertyStore::SettingsPropertyStore(QSettings *settings) : AbstractPropertyStore(), _settings(settings)
{
}

SettingsPropertyStore::~SettingsPropertyStore()
{
}

void SettingsPropertyStore::init(const QVariantHash &props)
{
    Q_UNUSED(props)
}

void SettingsPropertyStore::setValue(QString propName, QVariant propValue)
{
    _settings->setValue(propName, propValue);
}

bool SettingsPropertyStore::contains(QString propName)
{
    return _settings->contains(propName);
}

QVariant SettingsPropertyStore::value(QString propName)
{
    return _settings->value(propName);
}

void SettingsPropertyStore::reset()
{
    return _settings->sync();
}
