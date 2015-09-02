#include <windows.h>
#include <ddraw.h>
#include <mmsystem.h>
#include <stdio.h>

#include "text.h"
#include "palgraph.h"

char					*g_szAppName;
char					*g_szWinTitle;

LPDIRECTDRAW7			g_pDD;
LPDIRECTDRAWSURFACE7	g_pDDSPri;
LPDIRECTDRAWSURFACE7	g_pDDSBck;
LPDIRECTDRAWSURFACE7	g_pDDSWrk;
LPDIRECTDRAWSURFACE7	g_pDDSPalPri;
LPDIRECTDRAWSURFACE7	g_pDDSPalSec;

void					*g_pWrk;
void					*g_pPalPri;
void					*g_pPalSec;
ULONG					g_lPitchWrk;
ULONG					g_lPitchPalPri;
ULONG					g_lPitchPalSec;

DWORD					g_PalVic[256];
DWORD					g_PalDef[256];

HWND					g_hWnd;
BOOL					g_bActive;
DWORD					g_dwLastTick;
char					*g_szErr;


HRESULT RestoreSurfaces();
HRESULT DisplayFrame();
void CreatePallettes();
void FreeDD();
HRESULT InitDD();
LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
HRESULT CrtWnd( HINSTANCE hInst, int nCmdShow );
int APIENTRY WinMain( HINSTANCE hInst, HINSTANCE hPInst, LPSTR lpCmdLine, int nCmdShow );
