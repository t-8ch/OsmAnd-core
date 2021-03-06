#if !defined(__OSMAND_CORE_H_)
#define __OSMAND_CORE_H_

#ifdef OSMAND_CORE_EXPORTS
#if defined(_WIN32) || defined(__CYGWIN__)
#       define OSMAND_CORE_API \
            __declspec(dllexport)
#       define OSMAND_CORE_CALL \
            __stdcall
#   else
#       if !defined(__arm__)
#           define OSMAND_CORE_CALL
#       else
#           define OSMAND_CORE_CALL
#       endif
#       if __GNUC__ >= 4
#           define OSMAND_CORE_API \
                __attribute__ ((visibility ("default")))
#       else
#           define OSMAND_CORE_API
#       endif
#   endif
#   define OSMAND_CORE_API_DL \
        OSMAND_CORE_API
#else
#if defined(_WIN32) || defined(__CYGWIN__)
#       define OSMAND_CORE_API \
            __declspec(dllimport)
#       define OSMAND_CORE_CALL \
            __stdcall
#   else
#       if !defined(__arm__)
#           define OSMAND_CORE_CALL
#       else
#           define OSMAND_CORE_CALL
#       endif
#       if __GNUC__ >= 4
#           define OSMAND_CORE_API \
                __attribute__ ((visibility ("default")))
#       else
#           define OSMAND_CORE_API
#       endif
#   endif
#   define OSMAND_CORE_API_DL \
        OSMAND_CORE_API
#endif

#endif // __OSMAND_CORE_H_
