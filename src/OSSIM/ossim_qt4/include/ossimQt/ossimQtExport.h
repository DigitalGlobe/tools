#ifndef ossimQtExport_HEADER
#define ossimQtExport_HEADER

// define used to include in API which is being fazed out
// if you can compile your apps with this turned off you are
// well placed for compatablity with future versions.
#define USE_DEPRECATED_API

#if defined(_MSC_VER)
    #pragma warning( disable : 4244 )
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4267 )
    #pragma warning( disable : 4275 )
    #pragma warning( disable : 4290 )
    #pragma warning( disable : 4786 )
    #pragma warning( disable : 4305 )
#endif

#if defined(_MSC_VER) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
#    ifdef OSSIMQT_LIBRARY
#        define OSSIMQT_EXPORT   __declspec(dllexport)
#        define OSSIMQT_DLL   OSSIMQT_EXPORT
#    else
#        define OSSIMQT_EXPORT   __declspec(dllimport)
#        define OSSIMQT_DLL   OSSIMQT_EXPORT
#    endif /* OSSIMQT_LIBRARY */
#else
#    define OSSIMQT_EXPORT
#    define OSSIMQT_DLL   OSSIMQT_EXPORT
#endif  

// set up define for whether member templates are supported by VisualStudio compilers.
#ifdef _MSC_VER
# if (_MSC_VER >= 1300)
#  define __STL_MEMBER_TEMPLATES
# endif
#endif




#endif
