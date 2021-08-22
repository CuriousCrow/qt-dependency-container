#include "settingspropertyprovider.h"

SettingsPropertyProvider::SettingsPropertyProvider(QSettings *settings) : AbstractPropertyProvider(), _settings(settings)
{
}

SettingsPropertyProvider::~SettingsPropertyProvider()
{
}

void SettingsPropertyProvider::init(const QVariantHash &props)
{
    Q_UNUSED(props)
}

void SettingsPropertyProvider::setValue(QString propName, QVariant propValue)
{
    _settings->setValue(propName, propValue);
}

bool SettingsPropertyProvider::contains(QString propName)
{
    return _settings->contains(propName);
}

QVariant SettingsPropertyProvider::value(QString propName)
{
    return _settings->value(propName);
}

void SettingsPropertyProvider::reset()
{
    return _settings->sync();
}
