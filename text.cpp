#include "text.h"

//----------------------------------------------------------------------------------------------

char *PrintCS( char *pszTxt )
{
    g_dwTextPrnX = g_dwTextCutLf + ( ( g_dwTextCutRt - g_dwTextCutLf  - CalcLen( pszTxt ) ) / 2 );
    return PrintS( pszTxt );

}

//----------------------------------------------------------------------------------------------

char *PrintS( char *pszTxt )
{
    char a;

    while( TRUE )
    {
        switch( a = *pszTxt++ )
        {
        case 0x00:

            return pszTxt;

        case 0x0d:

            g_dwTextPrnX = g_dwTextCutLf;
            break;

        case 0x0a:

            g_dwTextPrnY += 16;
            break;

        default:

            PrintL( a );
        }
    }
}
