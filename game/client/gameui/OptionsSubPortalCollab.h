//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef OPTIONSSUBPORTALCOLLAB_H
#define OPTIONSSUBPORTALCOLLAB_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/PropertyPage.h>
#include <vgui_controls/ImagePanel.h>
#include "imageutils.h"

class CLabeledCommandComboBox;
class CBitmapImagePanel;

class CCvarToggleCheckButton;
class CCvarTextEntry;
class CCvarSlider;

class COptionsSubPortalCollab;

//-----------------------------------------------------------------------------
// Purpose: multiplayer options property page
//-----------------------------------------------------------------------------
class COptionsSubPortalCollab : public vgui::PropertyPage
{
	DECLARE_CLASS_SIMPLE( COptionsSubPortalCollab, vgui::PropertyPage );

public:
	COptionsSubPortalCollab(vgui::Panel *parent);
	~COptionsSubPortalCollab();

	virtual vgui::Panel *CreateControlByName(const char *controlName);

	MESSAGE_FUNC( OnControlModified, "ControlModified" );

protected:
	// Called when the OK / Apply button is pressed.  Changed data should be written into document.
	virtual void OnApplyChanges();

	virtual void OnCommand( const char *command );

private:
	void InitModelList(CLabeledCommandComboBox *cb);

	void RemapModel();
	
	MESSAGE_FUNC_PTR( OnTextChanged, "TextChanged", panel );

	void ColorForName(char const *pszColorName, int &r, int &g, int &b);

	CBitmapImagePanel *m_pModelImage;
	CLabeledCommandComboBox *m_pModelList;
	char m_ModelName[128];

    CCvarSlider *m_pPrimaryColorSlider;
    CCvarSlider *m_pSecondaryColorSlider;

	// Mod specific general checkboxes
	vgui::Dar< CCvarToggleCheckButton * > m_cvarToggleCheckButtons;
};

#endif // OPTIONSSUBMULTIPLAYER_H
