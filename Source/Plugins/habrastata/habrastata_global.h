#ifndef HABRASTATA_GLOBAL_H
#define HABRASTATA_GLOBAL_H

#include <QtGlobal>

#if defined(HABRASTATA_LIBRARY)
#  define HABRASTATASHARED_EXPORT Q_DECL_EXPORT
#else
#  define HABRASTATASHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // HABRASTATA_GLOBAL_H

