#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sqlitemodule.h"
#include "postgresmodule.h"
#include "dbmsmodule.h"

#include "customproperty.h"
#include "mycustom.h"
#include <QMetaObject>
#include <QDebug>
#include "simpleclass.h"
#include "settingspropertyprovider.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    SqliteModule* module1 = new SqliteModule();
//    SqliteModule* module2 = new SqliteModule();
//    PostgresModule* module3 = new PostgresModule();

//    qDebug() << module1->metaObject() << &SqliteModule::staticMetaObject;

    objContainer = new DependencyContainer(this);
    QSettings* settings = new QSettings(S_APPLICATION ".ini", QSettings::IniFormat);
    objContainer->setPropertyProvider(new SettingsPropertyProvider(settings));
    objContainer->setPropertyValue("postgres.author", "Michman");

    initDependencies();

    qDebug() << "All meta info:" << objContainer->allMetaInfo();
    qDebug() << "All singleton info:" << objContainer->allSingletonInfo();

//    ui->cmbDbms->addItems(objContainer->namesByClass(CLASS(DbmsModule)));

//    DbmsModule* module = objContainer->dependency<DbmsModule>("sqlite");
//    qDebug() << "Dependency by name" << module->customProp()->name();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnGetObject_clicked()
{
    MyCustom* myCustom = objContainer->dependency<MyCustom>("postgres");
    myCustom->printSomething();

//    MyCustom* myCustom2 = objContainer->dependency<MyCustom>("firebird");
//    myCustom2->printSomething();

//    QVariantHash params1;
//    params1.insert("primary", false);
//    DbmsModule* module = objContainer->dependency<DbmsModule>(params1);
//    ui->lblMain->setText(module->driverName());
//    DbmsModule* module = objContainer->dependency<DbmsModule>(ui->cmbDbms->currentText());
//    module->setParent(this);
//    ui->lblMain->setText(module->driverName());
}

void MainWindow::initDependencies()
{
    CustomProperty* prop1 = new CustomProperty();
    prop1->setName("Firebird properties");

    CustomProperty* prop2 = new CustomProperty();
    prop2->setName("Postgres properties");

//    objContainer->registerSingletonObject(new DependencyMeta("FirebirdProps", CLASSMETA(CustomProperty)), prop1);
    objContainer->registerSingletonObject(new DependencyMeta("PostgresProps", CLASSMETA(CustomProperty)), prop2);
    objContainer->registerDependency(new DependencyMeta("postgres", CLASSMETA(CustomPostgres), InstanceMode::Singleton));
//    objContainer->registerDependency(new DependencyMeta("firebird", CLASSMETA(CustomFirebird), InstanceMode::Singleton));
//    DependencyMeta* meta1 = new DependencyMeta("sqlite", &SqliteModule::staticMetaObject, InstanceMode::Prototype);
//    meta1->insert("primary", true);
//    objContainer->registerDependency(meta1);

//    DependencyMeta* meta2 = new DependencyMeta("postgres", &PostgresModule::staticMetaObject, InstanceMode::Singleton);
//    meta2->insert("primary", false);
//    objContainer->registerDependency(meta2);

//    objContainer->registerDependency(new DependencyMeta("customProperty", &CustomProperty::staticMetaObject, InstanceMode::Prototype));
}
