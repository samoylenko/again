/***

  Known bugs: maximized applications loose their size when demo finished.

***/

#define WIN32_LEAN_AND_MEAN
#define STRICT
#define BASSDEF(f) (WINAPI *f)

//----------------------------------------------------------------------------------------------

#include <windows.h>
#include <ddraw.h>
#include <mmsystem.h>
#include <stdio.h>
#include <math.h>

#include "text.h"
#include "palgraph.h"
#include "bass.h"

//----------------------------------------------------------------------------------------------

#define SAFE_RELEASE(p) { if( NULL != p ) { ( p )->Release(); ( p )=NULL; } }
#define SAFE_FREE(p)    { if( NULL != p ) { free( p ); ( p )=NULL; } }

char					g_szAppName[]	= "again";
char					g_szWinTitle[]	= "again - 2005 - reanimation";

LPDIRECTDRAW7			g_pDD			= NULL;
LPDIRECTDRAWSURFACE7	g_pDDSPri		= NULL;
LPDIRECTDRAWSURFACE7	g_pDDSBck		= NULL;
LPDIRECTDRAWSURFACE7	g_pDDSWrk		= NULL;
LPDIRECTDRAWSURFACE7	g_pDDSPalPri	= NULL;
LPDIRECTDRAWSURFACE7	g_pDDSPalSec	= NULL;

void					*g_pWrk			= NULL;
void					*g_pPalPri		= NULL;
void					*g_pPalSec		= NULL;
ULONG					g_lPitchWrk		= 800 * 4;
ULONG					g_lPitchPalPri	= 800;
ULONG					g_lPitchPalSec	= 800;

DWORD					g_PalVic[256];
DWORD					g_PalDef[256];

HWND					g_hWnd;
BOOL					g_bActive		= TRUE;
BOOL					g_bMP3			= FALSE;
DWORD					g_dwLastTick;
char					*g_szErr		= NULL;
HINSTANCE				g_hBass			= NULL;
DWORD					g_dwChan;

//----------------------------------------------------------------------------------------------

HRESULT RestoreSurfaces()
{
	HRESULT hr;

	if( FAILED( hr = g_pDDSPalSec->Restore() ) )
	{
		return hr;
	}

	if( FAILED( hr = g_pDDSPalPri->Restore() ) )
	{
		return hr;
	}

	if( FAILED( hr = g_pDDSWrk->Restore() ) )
	{
		return hr;
	}

	if( FAILED( hr = g_pDDSBck->Restore() ) )
	{
		return hr;
	}

	return g_pDDSPri->Restore();
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------

HRESULT InitMP3()
{
	if( !(g_hBass = LoadLibrary( "bass.dll" ) ) )
	{
		return E_FAIL;
	}

	BASS_Init				= ( BOOL ( __stdcall * )( DWORD, DWORD, DWORD, HWND, const GUID * ) )GetProcAddress( g_hBass, "BASS_Init" );
	BASS_Free				= GetProcAddress( g_hBass, "BASS_Free" );
	BASS_StreamCreateFile	= ( HSTREAM ( __stdcall * )( BOOL, const void *, DWORD, DWORD, DWORD ) )GetProcAddress( g_hBass, "BASS_StreamCreateFile" );
	BASS_ChannelPlay		= ( BOOL ( __stdcall * )( DWORD, BOOL ) )GetProcAddress( g_hBass, "BASS_ChannelPlay" );

	if(
	  ( NULL == BASS_Init )
	||( NULL == BASS_Free )
	||( NULL == BASS_StreamCreateFile )
	||( NULL == BASS_ChannelPlay )
	)
	{
		return E_FAIL;
	}

	if( !BASS_Init( 1, 44100, 0, 0, NULL) )
	{
		return E_FAIL;
	}

	if( !( g_dwChan = BASS_StreamCreateFile( FALSE, "again.mp3", 0, 0, BASS_SAMPLE_LOOP ) ) )
	{
		return E_FAIL;
	}

	BASS_ChannelPlay( g_dwChan, FALSE );

	return S_OK;
}

//----------------------------------------------------------------------------------------------

HRESULT Defination( BYTE *pScrPri, ULONG lPitchPri, BYTE *pScrSec, ULONG lPitchSec )
{
#define NUM_BALLS	20
#define PI			3.1415926536
#define XC			800/2-12
#define YC			600/2-12
#define ANGLE_XY	PI/1500

	static BYTE ball[] = 
	{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 
		0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
		0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
		0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
		0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
		0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
		0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 
		0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	};

	static double	axy = 0;
	static int		rx	= 0;
	static int		ry	= 0;
	
	HRESULT hr;
	int		x, y, x1, y1;

	Blur( pScrSec, pScrPri );

	for( int i = 0; i < NUM_BALLS; i++ )
	{
#pragma warning( push )
#pragma warning( disable : 4244 )
		x	= rx*sin( axy*i+axy );
		y	= ry*cos( axy*i-axy );
		x1	= rx*sin( axy*i-axy );
		y1	= ry*cos( axy*i+axy );
#pragma warning( pop )

		PutSprite( ball, 24, 24, pScrPri, lPitchPri, XC+x, YC+y );
		PutSprite( ball, 24, 24, pScrPri, lPitchPri, XC+x1, YC+y1 );
	}

	if( rx < 380 ) rx++;
	if( ry < 280 ) ry++;

	axy += ANGLE_XY;
	
	if( DDERR_SURFACELOST == g_pDDSPalSec->BltFast( 0, 0, g_pDDSPalPri, NULL, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT ) )
	{
		if( FAILED( hr = RestoreSurfaces() ) )
		{
			g_szErr = "Failed to BltFast Indexed pallettes";
			return hr;
		}
	}

	return S_OK;
}

//----------------------------------------------------------------------------------------------

HRESULT DisplayFrame()
{
	static FLOAT	fFPS = 0;
	static FLOAT	fLastTime = 0.0f;
	static DWORD	dwFrames  = 0L;
	static char		szFPS[20];

	HRESULT			hr;

	FLOAT			fTime = ( FLOAT )( timeGetTime() * 0.001 );
	++dwFrames;

    if( fTime - fLastTime > 1.0f )
    {
		fFPS		= dwFrames / (fTime - fLastTime);
		fLastTime	= fTime;
		dwFrames	= 0L;

		sprintf( szFPS, "fps: %.1f", fFPS );
	}

	if( FAILED( hr = Defination( ( BYTE * )g_pPalPri, 800, ( BYTE * )g_pPalSec, 800 ) ) )
	{
		return hr;
	}

	IndexedToRGB( g_pPalSec, g_pWrk, g_PalDef );

	g_dwTextPrnX	= 1;
	g_dwTextPrnY	= 1;
	g_dwTextPrnC	= 0x00ffffff;
	g_dwTextPrnM	= 0x00000000;
	PrintS( szFPS );

	g_dwTextPrnC	= 0x00ffffff;
	g_dwTextPrnM	= 0x00000000;
	g_dwTextPrnX	= 100;
	g_dwTextPrnY	= 100;
	PrintS( "Hello World!" );
	g_dwTextPrnX	= 100;
	g_dwTextPrnY	= 116;
	PrintS( "Testing TEST test TeSt [Tt][Ee][Ss][Tt]" );

	if( DDERR_SURFACELOST == g_pDDSBck->BltFast( 0, 0, g_pDDSWrk, NULL, DDBLTFAST_NOCOLORKEY | DDBLTFAST_WAIT ) )
	{
		if( FAILED( hr = RestoreSurfaces() ) )
		{
			g_szErr = "Failed to BltFast Wrk->Bck";
			return hr;
		}
	}

	if( DDERR_SURFACELOST == g_pDDSPri->Flip( NULL, DDFLIP_WAIT /*| DDFLIP_NOVSYNC*/ ) )
	{
		if( FAILED( hr = RestoreSurfaces() ) )
		{
			g_szErr = "Failed to flip primary surfaces";
			return hr;
		}
	}

	return S_OK;
}

//----------------------------------------------------------------------------------------------

void CreatePallettes()
{
	int i;
	BYTE *pal = ( BYTE * )g_PalVic;

	for( i = 0; i < 0x40; i++ )
	{
		pal[i*4]	= i;
		pal[i*4+1]	= 0;
		pal[i*4+2]	= 0;
		pal[i*4+3]	= 0;
	}

	for( ; i < 0x80; i++ )
	{
		pal[i*4]	= 0x3f;
		pal[i*4+1]	= i - 0x40;
		pal[i*4+2]	= 0;
		pal[i*4+3]	= 0;
	}

	for( ; i < 0xc0; i++ )
	{
		pal[i*4]	= 0xbf - i;
		pal[i*4+1]	= 0x3f;
		pal[i*4+2]	= 0;
		pal[i*4+3]	= 0;
	}

	for( ; i < 0x100; i++ )
	{
		pal[i*4]	= 0;
		pal[i*4+1]	= 0xff - i;
		pal[i*4+2]	= i - 0xc0;
		pal[i*4+3]	= 0;
	}

	pal = ( BYTE * )g_PalDef;

	for( i = 0; i < 0x40; i++ )
	{
		pal[i*4+0x00]	= i*4;
		pal[i*4+0x01]	= 0;
		pal[i*4+0x02]	= 0;
		pal[i*4+0x03]	= 0;

		pal[i*4+0x100]	= 0; //0xff - i*4;
		pal[i*4+0x101]	= 0;
		pal[i*4+0x102]	= i*4;
		pal[i*4+0x103]	= 0;

		pal[i*4+0x200]	= 0;
		pal[i*4+0x201]	= i*4;
		pal[i*4+0x202]	= 0xff;
		pal[i*4+0x203]	= 0;

		pal[i*4+0x300]	= i*4;
		pal[i*4+0x301]	= 0xff;
		pal[i*4+0x302]	= 0xff;
		pal[i*4+0x303]	= 0;
	}
}

//----------------------------------------------------------------------------------------------

void FreeDD()
{
	if( NULL != g_pDD )
	{
		SAFE_RELEASE( g_pDDSPalSec );
		SAFE_RELEASE( g_pDDSPalPri );
		SAFE_RELEASE( g_pDDSWrk );
		SAFE_RELEASE( g_pDDSPri );

		g_pDD->Release();
		g_pDD = NULL;
	}

	SAFE_FREE( g_pPalSec );
	SAFE_FREE( g_pPalPri );
	SAFE_FREE( g_pWrk );

	if( g_bMP3 )
	{
		BASS_Free();
	}

	if( NULL != g_hBass )
	{
		FreeLibrary( g_hBass );
	}

}

//----------------------------------------------------------------------------------------------

HRESULT InitDD()
{
	HRESULT hr;

	DDSURFACEDESC2	ddsd;
	DDSCAPS2		ddscaps;

	if( FAILED( hr = DirectDrawCreateEx( NULL, ( VOID** )&g_pDD, IID_IDirectDraw7, NULL ) ) )
	{
		g_szErr = "DirectDraw create error";
		return hr;
	}

	if( FAILED( hr = g_pDD->SetCooperativeLevel( g_hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN ) ) )
	{
		g_szErr = "Set cooperative level error";
		return hr;
	}

	if( FAILED( hr = g_pDD->SetDisplayMode( 800, 600, 32, 0, 0 ) ) )
	{
		g_szErr = "Set video mode error";
		return hr;
	}

	ZeroMemory( &ddsd, sizeof( ddsd ) );
	ddsd.dwSize				= sizeof( ddsd );
	ddsd.dwFlags			= DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps		= DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	ddsd.dwBackBufferCount	= 1;

	if( FAILED( hr = g_pDD->CreateSurface( &ddsd, &g_pDDSPri, NULL ) ) )
	{
		g_szErr = "Create primary surface error";
		return hr;
	}

	ZeroMemory( &ddscaps, sizeof( ddscaps ) );
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;

	if( FAILED( hr = g_pDDSPri->GetAttachedSurface( &ddscaps, &g_pDDSBck ) ) )
	{
		g_szErr = "Create back surface error";
		return hr;
	}

	if( NULL == ( g_pWrk = malloc( 800*600*4 ) ) )
	{
		g_szErr = "Memory allocation error (work surface)";
		return DDERR_OUTOFMEMORY;
	}

	ZeroMemory( g_pWrk, 800*600*4 );
	ZeroMemory( &ddsd, sizeof( ddsd ) );
	ZeroMemory( &ddsd.ddpfPixelFormat, sizeof( ddsd.ddpfPixelFormat ) );

	ddsd.dwSize			= sizeof( ddsd );
	ddsd.dwFlags		= DDSD_WIDTH | DDSD_HEIGHT | DDSD_LPSURFACE | 
						DDSD_PITCH | DDSD_PIXELFORMAT | DDSD_CAPS;
	ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth		= 800;
	ddsd.dwHeight		= 600;
	ddsd.lPitch			= g_lPitchWrk;
	ddsd.lpSurface		= g_pWrk;

	ddsd.ddpfPixelFormat.dwSize			= sizeof( ddsd.ddpfPixelFormat );
	ddsd.ddpfPixelFormat.dwFlags		= DDPF_RGB;
	ddsd.ddpfPixelFormat.dwRGBBitCount	= 32;
	ddsd.ddpfPixelFormat.dwRBitMask		= 0x00ff0000;
	ddsd.ddpfPixelFormat.dwGBitMask		= 0x0000ff00;
	ddsd.ddpfPixelFormat.dwBBitMask		= 0x000000ff;

	if( FAILED( hr = g_pDD->CreateSurface( &ddsd, &g_pDDSWrk, NULL ) ) )
	{
		g_szErr = "Create work surface error";
		return hr;
	}

	if( NULL == ( g_pPalPri = malloc( 800*600 ) ) )
	{
		g_szErr = "Memory allocation error (primary indexed surface)";
		return DDERR_OUTOFMEMORY;
	}

	ZeroMemory( g_pPalPri, 800*600 );
	ZeroMemory( &ddsd, sizeof( ddsd ) );
	ZeroMemory( &ddsd.ddpfPixelFormat, sizeof( ddsd.ddpfPixelFormat ) );

	ddsd.dwSize			= sizeof( ddsd );
	ddsd.dwFlags		= DDSD_WIDTH | DDSD_HEIGHT | DDSD_LPSURFACE | 
						DDSD_PITCH | DDSD_PIXELFORMAT | DDSD_CAPS;
	ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth		= 800;
	ddsd.dwHeight		= 600;
	ddsd.lPitch			= g_lPitchPalPri;
	ddsd.lpSurface		= g_pPalPri;

	ddsd.ddpfPixelFormat.dwSize			= sizeof( ddsd.ddpfPixelFormat );
	ddsd.ddpfPixelFormat.dwFlags		= DDPF_PALETTEINDEXED8 | DDPF_RGB;
	ddsd.ddpfPixelFormat.dwRGBBitCount	= 8;

	if( FAILED( hr = g_pDD->CreateSurface( &ddsd, &g_pDDSPalPri, NULL ) ) )
	{
		g_szErr = "Create primary indexed surface error";
		return hr;
	}

	if( NULL == ( g_pPalSec = malloc( 800*600 ) ) )
	{
		g_szErr = "Memory allocation error (secondary indexed surface)";
		return DDERR_OUTOFMEMORY;
	}

	ZeroMemory( g_pPalSec, 800*600 );
	ZeroMemory( &ddsd, sizeof( ddsd ) );
	ZeroMemory( &ddsd.ddpfPixelFormat, sizeof( ddsd.ddpfPixelFormat ) );

	ddsd.dwSize			= sizeof( ddsd );
	ddsd.dwFlags		= DDSD_WIDTH | DDSD_HEIGHT | DDSD_LPSURFACE | 
						DDSD_PITCH | DDSD_PIXELFORMAT | DDSD_CAPS;
	ddsd.ddsCaps.dwCaps	= DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth		= 800;
	ddsd.dwHeight		= 600;
	ddsd.lPitch			= g_lPitchPalSec;
	ddsd.lpSurface		= g_pPalSec;

	ddsd.ddpfPixelFormat.dwSize			= sizeof( ddsd.ddpfPixelFormat );
	ddsd.ddpfPixelFormat.dwFlags		= DDPF_PALETTEINDEXED8 | DDPF_RGB;
	ddsd.ddpfPixelFormat.dwRGBBitCount	= 8;

	if( FAILED( hr = g_pDD->CreateSurface( &ddsd, &g_pDDSPalSec, NULL ) ) )
	{
		g_szErr = "Create secondary indexed surface error";
		return hr;
	}

	g_pText			= ( ULONG * )g_pWrk;
	g_lTextPitch	= g_lPitchWrk;

	return S_OK;
}

//----------------------------------------------------------------------------------------------

LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_SIZE:

		if( ( SIZE_MAXHIDE == wParam )||( SIZE_MINIMIZED == wParam ) )
		{
			g_bActive = FALSE;
		}
		else
		{
			g_bActive = TRUE;
		}
		break;

	case WM_KEYDOWN:

		switch( wParam )
		{
		case VK_ESCAPE:

			PostMessage( hWnd, WM_CLOSE, 0, 0 );
			break;
		}

		return 0;

	case WM_SETCURSOR:

		SetCursor( NULL == g_pDD ? LoadCursor( NULL, IDC_ARROW ) : NULL );
		return TRUE;

	case WM_DESTROY:

		FreeDD();
		PostQuitMessage( 0 );

		return 0;
	} 

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

//----------------------------------------------------------------------------------------------

HRESULT CrtWnd( HINSTANCE hInst, int nCmdShow )
{
	WNDCLASS wc;

	wc.lpszClassName	= g_szAppName;
	wc.lpfnWndProc		= WndProc;
	wc.style			= CS_VREDRAW | CS_HREDRAW;
	wc.hInstance		= hInst;
	wc.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground	= ( HBRUSH )GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName		= NULL;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;

	if( !RegisterClass( &wc ) )
	{
		g_szErr = "Register class error";
		return E_FAIL;
	} 

	g_hWnd = CreateWindowEx(
		0,
		g_szAppName,
		g_szWinTitle,
		WS_POPUP,
		0,
		0,
		GetSystemMetrics( SM_CXSCREEN ),
		GetSystemMetrics( SM_CYSCREEN ),
		NULL,
		NULL,
		hInst,
		NULL
	); 

	if( !g_hWnd )
	{
		g_szErr = "Create window error";
		return E_FAIL;
	} 

	ShowWindow( g_hWnd, nCmdShow );
	UpdateWindow( g_hWnd );

	return S_OK;
} 


//----------------------------------------------------------------------------------------------

int APIENTRY WinMain( HINSTANCE hInst, HINSTANCE hPInst, LPSTR lpCmdLine, int nCmdShow )
{
	MSG		msg;
	HRESULT hr;

	g_dwLastTick = timeGetTime();

	if( FAILED( CrtWnd( hInst, nCmdShow ) ) )
	{
		MessageBox( NULL, NULL != g_szErr ? g_szErr : "Failed to create window", g_szWinTitle, MB_ICONERROR | MB_OK );
		return FALSE;
	} 

	if( SUCCEEDED( hr = InitMP3() ) )
	{
		g_bMP3 = TRUE;
	}

	if( FAILED( hr = InitDD() ) )
	{
		FreeDD();
		MessageBox( g_hWnd, NULL != g_szErr ? g_szErr : "DirectDraw init failed", g_szWinTitle, MB_ICONERROR | MB_OK );

		return FALSE;	
	}

	CreatePallettes();
	g_dwLastTick = timeGetTime();

	while( TRUE )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if( !GetMessage(&msg, NULL, 0, 0 ) )
			{
				return ( int )msg.wParam;
			} 

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		} 
		else
		{
			if( g_bActive )
			{
				if( FAILED( hr = DisplayFrame() ) )
				{
					FreeDD();
					MessageBox( g_hWnd, NULL != g_szErr ? g_szErr : "Displaying the next frame failed", g_szWinTitle, MB_ICONERROR | MB_OK ); 

					return FALSE;
				} 
			} 
			else
			{
				WaitMessage();
				g_dwLastTick = timeGetTime();
			} 
		} 
	} 
} 