#include <windows.h>

//----------------------------------------------------------------------------------------------

extern "C" void  PrintL( char );
extern "C" DWORD CalcLen( char * );

//----------------------------------------------------------------------------------------------

char *PrintS( char *pszTxt );
char *PrintCS( char *pszTxt );

//----------------------------------------------------------------------------------------------

extern "C" DWORD		*g_pText;
extern "C" ULONG		g_lTextPitch;
extern "C" DWORD		g_dwTextPrnX;
extern "C" DWORD		g_dwTextPrnY;
extern "C" DWORD		g_dwTextPrnC;
extern "C" DWORD		g_dwTextPrnM;
extern "C" DWORD		g_dwTextCutLf;
extern "C" DWORD		g_dwTextCutRt;
