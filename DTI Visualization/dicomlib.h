/*
 * MATLAB Compiler: 4.8 (R2008a)
 * Date: Thu Aug 12 13:19:15 2010
 * Arguments: "-B" "macro_default" "-B" "csharedlib:dicomlib" "-W"
 * "lib:dicomlib" "-T" "link:lib" "read_dicom_image.m" 
 */

#ifndef __dicomlib_h
#define __dicomlib_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_dicomlib
#define PUBLIC_dicomlib_C_API __global
#else
#define PUBLIC_dicomlib_C_API /* No import statement needed. */
#endif

#define LIB_dicomlib_C_API PUBLIC_dicomlib_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_dicomlib
#define PUBLIC_dicomlib_C_API __declspec(dllexport)
#else
#define PUBLIC_dicomlib_C_API __declspec(dllimport)
#endif

#define LIB_dicomlib_C_API PUBLIC_dicomlib_C_API


#else

#define LIB_dicomlib_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_dicomlib_C_API 
#define LIB_dicomlib_C_API /* No special import/export declaration */
#endif

extern LIB_dicomlib_C_API 
bool MW_CALL_CONV dicomlibInitializeWithHandlers(mclOutputHandlerFcn error_handler,
                                                 mclOutputHandlerFcn print_handler);

extern LIB_dicomlib_C_API 
bool MW_CALL_CONV dicomlibInitialize(void);

extern LIB_dicomlib_C_API 
void MW_CALL_CONV dicomlibTerminate(void);



extern LIB_dicomlib_C_API 
void MW_CALL_CONV dicomlibPrintStackTrace(void);


extern LIB_dicomlib_C_API 
bool MW_CALL_CONV mlxRead_dicom_image(int nlhs, mxArray *plhs[],
                                      int nrhs, mxArray *prhs[]);


extern LIB_dicomlib_C_API bool MW_CALL_CONV mlfRead_dicom_image(int nargout
                                                                , mxArray** result
                                                                , mxArray* P);

#ifdef __cplusplus
}
#endif

#endif
