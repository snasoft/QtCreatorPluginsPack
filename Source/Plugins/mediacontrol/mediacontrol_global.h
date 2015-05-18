#ifndef MEDIACONTROL_GLOBAL_H
#define MEDIACONTROL_GLOBAL_H

#include <QtGlobal>

#if defined(MEDIACONTROL_LIBRARY)
#  define MEDIACONTROLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MEDIACONTROLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MEDIACONTROL_GLOBAL_H
