#ifndef CUSTOMPROPERTY_H
#define CUSTOMPROPERTY_H

#include <QObject>

class CustomProperty : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString author READ author WRITE setAuthor)
public:
    Q_INVOKABLE CustomProperty();
    void setName(QString name);
    QString name();

    QString author() const;
    void setAuthor(const QString &author);
private:
    QString _name = "I'm injected property";
    QString _author;
signals:

};

#endif // CUSTOMPROPERTY_H
