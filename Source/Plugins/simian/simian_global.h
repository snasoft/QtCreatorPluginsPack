#ifndef SIMIAN_GLOBAL_H
#define SIMIAN_GLOBAL_H

#include <QtGlobal>

#if defined(SIMIAN_LIBRARY)
#  define SIMIANSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SIMIANSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SIMIAN_GLOBAL_H

