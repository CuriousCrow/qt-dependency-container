TEMPLATE = subdirs

SUBDIRS += \
  DemoSimple/DemoSimple.pro \
  DemoDependencyInjection/DemoDependencyInjection.pro \
  DemoPropertyInjection/DemoPropertyInjection.pro \
  Tests

OTHER_FILES += \
  readme.md
	
CONFIG += qt
