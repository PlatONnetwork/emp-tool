#ifndef CONFIG_H__
#define CONFIG_H__
namespace emp {
const static int AES_BATCH_SIZE = 2048;
const static int HASH_BUFFER_SIZE = 1024*8;
const static int NETWORK_BUFFER_SIZE = 1024*16;//Should change depending on the network
const static int FILE_BUFFER_SIZE = 1024*16;
const static int CHECK_BUFFER_SIZE = 1024*8;

const static int XOR = -1;
const static int PUBLIC = 0;
const static int ALICE = 1;
const static int BOB = 2;

#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
#define UNIX_PLATFORM
#endif
}

// for ir compiler "plang", must define EMP_MPC_DLL_API as empty
#ifdef EMP_USE_DLL_API_AS_MODULE
#ifdef _WIN32
#  if defined(EMP_MPC_STATIC)
#    define EMP_MPC_DLL_API 
#  else
#    if defined(EMP_MPC_EXPORTS)
#      define EMP_MPC_DLL_API __declspec(dllexport)
#    else
#      define EMP_MPC_DLL_API __declspec(dllimport)
#    endif
#  endif
#else
#  define EMP_MPC_DLL_API 
#endif
#else
#  define EMP_MPC_DLL_API 
#endif

#endif// CONFIG_H__
