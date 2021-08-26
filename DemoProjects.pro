TEMPLATE = subdirs

SUBDIRS += \
  DemoSimple/DemoSimple.pro \
  DemoDependencyInjection/DemoDependencyInjection.pro \
  DemoPropertyInjection/DemoPropertyInjection.pro

OTHER_FILES += \
  readme.md
	
CONFIG += qt
