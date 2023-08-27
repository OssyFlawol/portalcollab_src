//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
#include "cbase.h"
#include "OptionsSubPortal.h"
#include "CvarToggleCheckButton.h"
#include "vgui_controls/ComboBox.h"

#include "EngineInterface.h"

#include <KeyValues.h>
#include <vgui/IScheme.h>
#include "tier1/convar.h"
#include <stdio.h>
#include <vgui_controls/TextEntry.h>
// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

using namespace vgui;

#ifdef COLORED_PORTALS
ConVar cl_portal_color_set("cl_portal_color_set", "0", FCVAR_ARCHIVE | FCVAR_USERINFO, "Sets the color portal set for the client", true, 0, true, 4);
#endif

COptionsSubPortal::COptionsSubPortal(vgui::Panel *parent) : PropertyPage(parent, NULL)
{
	m_pPortalFunnelCheckBox = new CCvarToggleCheckButton( 
		this, 
		"PortalFunnel", 
		"#GameUI_PortalFunnel", 
		"sv_player_funnel_into_portals" );

	m_pPortalDepthCombo = new ComboBox( this, "PortalDepth", 6, false );
	m_pPortalDepthCombo->AddItem( "#GameUI_PortalDepth0", new KeyValues("PortalDepth", "depth", 0) );
	m_pPortalDepthCombo->AddItem( "#GameUI_PortalDepth1", new KeyValues("PortalDepth", "depth", 1) );
	m_pPortalDepthCombo->AddItem( "#GameUI_PortalDepth2", new KeyValues("PortalDepth", "depth", 2) );
	m_pPortalDepthCombo->AddItem( "#GameUI_PortalDepth3", new KeyValues("PortalDepth", "depth", 3) );
	m_pPortalDepthCombo->AddItem( "#GameUI_PortalDepth4", new KeyValues("PortalDepth", "depth", 4) );
	m_pPortalDepthCombo->AddItem( "#GameUI_PortalDepth5", new KeyValues("PortalDepth", "depth", 5) );
	m_pPortalDepthCombo->AddItem( "#GameUI_PortalDepth6", new KeyValues("PortalDepth", "depth", 6) );
	m_pPortalDepthCombo->AddItem( "#GameUI_PortalDepth7", new KeyValues("PortalDepth", "depth", 7) );
	m_pPortalDepthCombo->AddItem( "#GameUI_PortalDepth8", new KeyValues("PortalDepth", "depth", 8) );
	m_pPortalDepthCombo->AddItem( "#GameUI_PortalDepth9", new KeyValues("PortalDepth", "depth", 9) );

#ifdef COLORED_PORTALS
	m_pPortalColorCombo = new ComboBox( this, "PortalColorSet", 6, false );
	m_pPortalColorCombo->AddItem( "#GameUI_PortalColorSet0", new KeyValues("PortalColorSet", "colorset", 0) );
	m_pPortalColorCombo->AddItem( "#GameUI_PortalColorSet1", new KeyValues("PortalColorSet", "colorset", 1) );
	m_pPortalColorCombo->AddItem( "#GameUI_PortalColorSet2", new KeyValues("PortalColorSet", "colorset", 2) );
	m_pPortalColorCombo->AddItem( "#GameUI_PortalColorSet3", new KeyValues("PortalColorSet", "colorset", 3) );
	m_pPortalColorCombo->AddItem( "#GameUI_PortalColorSet4", new KeyValues("PortalColorSet", "colorset", 4) );
#endif
	LoadControlSettings("Resource\\OptionsSubPortal2.res");
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
COptionsSubPortal::~COptionsSubPortal()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubPortal::OnResetData()
{
	m_pPortalFunnelCheckBox->Reset();

	// Portal render depth
	ConVarRef r_portal_stencil_depth("r_portal_stencil_depth");
	if ( r_portal_stencil_depth.IsValid() )
	{
		m_pPortalDepthCombo->ActivateItem(r_portal_stencil_depth.GetInt());
	}
#ifdef COLORED_PORTALS
	{
		m_pPortalColorCombo->ActivateItem(cl_portal_color_set.GetInt());
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubPortal::OnApplyChanges()
{
	m_pPortalFunnelCheckBox->ApplyChanges();

	// Portal render depth
	if ( m_pPortalDepthCombo->IsEnabled() )
	{
		ConVarRef r_portal_stencil_depth( "r_portal_stencil_depth" );
		r_portal_stencil_depth.SetValue( m_pPortalDepthCombo->GetActiveItem() );
	}
#ifdef COLORED_PORTALS
	if ( m_pPortalColorCombo->IsEnabled() )
	{
		cl_portal_color_set.SetValue( m_pPortalColorCombo->GetActiveItem() );
	}
#endif
}

//-----------------------------------------------------------------------------
// Purpose: sets background color & border
//-----------------------------------------------------------------------------
void COptionsSubPortal::ApplySchemeSettings(IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings(pScheme);
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubPortal::OnControlModified()
{
	PostActionSignal(new KeyValues("ApplyButtonEnable"));
}
