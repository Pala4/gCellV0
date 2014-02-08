#ifndef GCELLCORELIB_GLOBAL_H
#define GCELLCORELIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GCELLCORELIB_LIBRARY)
#  define GCELLCORELIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define GCELLCORELIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // GCELLCORELIB_GLOBAL_H