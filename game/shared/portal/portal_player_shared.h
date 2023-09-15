//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#ifndef PORTAL_PLAYER_SHARED_H
#define PORTAL_PLAYER_SHARED_H
#pragma once

#define PORTAL_PUSHAWAY_THINK_INTERVAL		(1.0f / 20.0f)
#include "studio.h"

#ifdef GAME_DLL
#include "portal_player.h"
#else
#include "c_portal_player.h"
#endif


enum
{
	PLAYER_SOUNDS_CITIZEN = 0,
	PLAYER_SOUNDS_COMBINESOLDIER,
	PLAYER_SOUNDS_METROPOLICE,
	PLAYER_SOUNDS_MAX,
};

enum 
{
	CONCEPT_PLAYER_DEAD,
	CONCEPT_CHELL_IDLE,
	CONCEPT_MEL_IDLE,
	CONCEPT_MALE_PORTAL_PLAYER_IDLE,
	//Escape Idles
	CONCEPT_MEL_ESCAPE_IDLE,
};


extern const char *g_pszPortalPlayerConcepts[];
int GetChellConceptIndexFromString( const char *pszConcept );

#if defined( CLIENT_DLL )
#define CPortal_Player C_Portal_Player
#endif


#endif //PORTAL_PLAYER_SHARED_h
