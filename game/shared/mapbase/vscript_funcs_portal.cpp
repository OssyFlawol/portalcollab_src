//========= Mapbase - https://github.com/mapbase-source/source-sdk-2013 ============//
//
// Purpose: VScript functions for Half-Life 2.
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"

#include "hl2_gamerules.h"
#ifndef CLIENT_DLL
#include "weapon_portalgun.h"
#include "eventqueue.h"
#endif
#include "portal_player_shared.h"
#include "portal_gamerules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


#ifndef CLIENT_DLL
extern CBaseEntity *CreatePlayerLoadSave( Vector vOrigin, float flDuration, float flHoldTime, float flLoadTime );

#endif


HSCRIPT Script_ToPortalPlayer( HSCRIPT hScript )
{
	return ToHScript( ToPortalPlayer( ToEnt( hScript ) ) );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CPortalGameRules::RegisterScriptFunctions( void )
{
	BaseClass::RegisterScriptFunctions();
	
	ScriptRegisterFunctionNamed( g_pScriptVM, Script_ToPortalPlayer, "ToPortalPlayer", "Casts an entity to a Portal Player, returns NULL if the entity isn't a player. Usage: ToPortalPlayer( entity )" );
}
