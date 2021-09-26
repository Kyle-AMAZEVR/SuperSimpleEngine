
#   ifdef CORE_EXPORTS
#		define THIS_IS_CORE_MODULE
#       define CORE_API __declspec(dllexport)
#		define CORE_API_EXTERN extern __declspec(dllexport)
#   elif defined(CORE_IMPORTS)
#       define CORE_API __declspec(dllimport)
#		define CORE_API_EXTERN extern __declspec(dllimport)
#   else
#       define CORE_API
#   endif

