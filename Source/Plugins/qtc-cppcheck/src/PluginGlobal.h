#ifndef QTCCPPCHECK_GLOBAL_H
#define QTCCPPCHECK_GLOBAL_H

#include <QtGlobal>

#if defined(QTCCPPCHECK_LIBRARY)
#  define QTCCPPCHECKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTCCPPCHECKSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTCCPPCHECK_GLOBAL_H

