//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//
#include "cbase.h"
#include "portal_shareddefs.h"
#include "filesystem.h"

char *g_ppszPortalPassThroughMaterials[] = 
{ 
	"lights/light_orange001", 
	NULL,
};

KeyValues *LoadRadioData()
{	
	KeyValues *radios = new KeyValues( "radios.txt" );
	if ( !radios->LoadFromFile( g_pFullFileSystem, RADIO_DATA_FILE, "MOD" ) )
	{
		AssertMsg( false, "Failed to load radio data" );
		return NULL;
	}

	return radios;
}