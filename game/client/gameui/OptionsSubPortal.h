//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef OPTIONS_SUB_PORTAL_H
#define OPTIONS_SUB_PORTAL_H
#ifdef _WIN32
#pragma once
#endif

// portalcoop allows players to change their portal colors, but we're going to save this feature just in case we decide we want it - Wonderland_War
//#define COLORED_PORTALS

#include <vgui_controls/PropertyPage.h>

class CLabeledCommandComboBox;
class CCvarToggleCheckButton;

namespace vgui
{
	class Label;
	class Panel;
}

//-----------------------------------------------------------------------------
// Purpose: Mouse Details, Part of OptionsDialog
//-----------------------------------------------------------------------------
class COptionsSubPortal : public vgui::PropertyPage
{
	DECLARE_CLASS_SIMPLE( COptionsSubPortal, vgui::PropertyPage );

public:
	COptionsSubPortal(vgui::Panel *parent);
	~COptionsSubPortal();

	virtual void OnResetData();
	virtual void OnApplyChanges();

protected:
	virtual void ApplySchemeSettings(vgui::IScheme *pScheme);

private:
	MESSAGE_FUNC( OnCheckButtonChecked, "CheckButtonChecked" )
	{
		OnControlModified();
	}
	MESSAGE_FUNC( OnControlModified, "ControlModified" );
	MESSAGE_FUNC( OnTextChanged, "TextChanged" )
	{
		OnControlModified();
	}

	CCvarToggleCheckButton		*m_pPortalFunnelCheckBox;
	vgui::ComboBox				*m_pPortalDepthCombo;

#ifdef COLORED_PORTALS
	vgui::ComboBox				*m_pPortalColorCombo;
#endif
};



#endif // OPTIONS_SUB_MOUSE_H