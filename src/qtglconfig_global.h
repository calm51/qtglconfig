#pragma execution_character_set("utf-8")

#ifndef QTGLCONFIG_GLOBAL_H
#define QTGLCONFIG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTGLCONFIG_LIBRARY)
#define QTGLCONFIG_EXPORT Q_DECL_EXPORT
#else
#define QTGLCONFIG_EXPORT Q_DECL_IMPORT
#endif

#endif // QTGLCONFIG_GLOBAL_H
