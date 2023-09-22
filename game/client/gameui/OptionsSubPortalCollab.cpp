//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"

#if defined( WIN32 ) && !defined( _X360 )
#include <winlite.h> // SRC only!!
#endif

#include "OptionsSubPortalCollab.h"
#include <stdio.h>

#include <vgui_controls/Button.h>
#include <vgui_controls/QueryBox.h>
#include <vgui_controls/CheckButton.h>
#include "tier1/KeyValues.h"
#include <vgui_controls/Label.h>
#include <vgui/ISystem.h>
#include <vgui/ISurface.h>
#include <vgui/Cursor.h>
#include <vgui_controls/RadioButton.h>
#include <vgui_controls/ComboBox.h>
#include <vgui_controls/ImagePanel.h>
#include <vgui_controls/FileOpenDialog.h>
#include <vgui_controls/MessageBox.h>
#include <vgui/IVGui.h>
#include <vgui/ILocalize.h>
#include <vgui/IPanel.h>
#include <vgui_controls/MessageBox.h>

#include "CvarTextEntry.h"
#include "CvarToggleCheckButton.h"
#include "..\common\GameUI\cvarslider.h"
#include "LabeledCommandComboBox.h"
#include "filesystem.h"
#include "EngineInterface.h"
#include "BitmapImagePanel.h"
#include "tier1/utlbuffer.h"
#include "ModInfo.h"
#include "tier1/convar.h"
#include "tier0/icommandline.h"

#include "materialsystem/imaterial.h"
#include "materialsystem/imesh.h"
#include "materialsystem/imaterialvar.h"
#include "optionssubmultiplayer.h"

#include <setjmp.h>

#include "bitmap/tgawriter.h"
#include "ivtex.h"
#ifdef WIN32
#include <io.h>
#endif

#if defined( _X360 )
#include "xbox/xbox_win32stubs.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

using namespace vgui;


#define DEFAULT_SUIT_HUE 30
#define DEFAULT_PLATE_HUE 6

void UpdateLogoWAD( void *hdib, int r, int g, int b );

struct ColorItem_t
{
	const char	*name;
	int			r, g, b;
};

static ColorItem_t itemlist[]=
{
	{ "#Valve_Orange", 255, 120, 24 },
	{ "#Valve_Yellow", 225, 180, 24 },
	{ "#Valve_Blue", 0, 60, 255 },
	{ "#Valve_Ltblue", 0, 167, 255 },
	{ "#Valve_Green", 0, 167, 0 },
	{ "#Valve_Red", 255, 43, 0 },
	{ "#Valve_Brown", 123, 73, 0 },
	{ "#Valve_Ltgray", 100, 100, 100 },
	{ "#Valve_Dkgray", 36, 36, 36 },
};

//-----------------------------------------------------------------------------
// Purpose: Basic help dialog
//-----------------------------------------------------------------------------
COptionsSubPortalCollab::COptionsSubPortalCollab(vgui::Panel *parent) : vgui::PropertyPage(parent, "OptionsSubMultiplayer") 
{
	Button *cancel = new Button( this, "Cancel", "#GameUI_Cancel" );
	cancel->SetCommand( "Close" );

	Button *ok = new Button( this, "OK", "#GameUI_OK" );
	ok->SetCommand( "Ok" );

	Button *apply = new Button( this, "Apply", "#GameUI_Apply" );
	apply->SetCommand( "Apply" );

	Button *advanced = new Button( this, "Advanced", "#GameUI_AdvancedEllipsis" );
	advanced->SetCommand( "Advanced" );

	m_pPrimaryColorSlider = new CCvarSlider( this, "Primary Color Slider", "#GameUI_PrimaryColor",
		0.0f, 255.0f, "topcolor" );

	m_pSecondaryColorSlider = new CCvarSlider( this, "Secondary Color Slider", "#GameUI_SecondaryColor",
		0.0f, 255.0f, "bottomcolor" );
	
	m_pModelList = new CLabeledCommandComboBox( this, "Player model" );
	m_ModelName[0] = 0;
	InitModelList( m_pModelList );

	m_pModelImage = new CBitmapImagePanel( this, "ModelImage", NULL );
	m_pModelImage->AddActionSignalTarget( this );

	//=========

	LoadControlSettings("Resource/optionssubportalcollab.res");

	// turn off model selection stuff if the mod specifies "nomodels" in the gameinfo.txt file
	if ( ModInfo().NoModels() )
	{
		Panel *pTempPanel = NULL;

		if ( m_pModelImage )
		{
			m_pModelImage->SetVisible( false );
		}

		if ( m_pModelList )
		{
			m_pModelList->SetVisible( false );
		}

		if ( m_pPrimaryColorSlider )
		{
			m_pPrimaryColorSlider->SetVisible( false );
		}

		if ( m_pSecondaryColorSlider )
		{
			m_pSecondaryColorSlider->SetVisible( false );
		}

		// #GameUI_PlayerModel (from "Resource/OptionsSubMultiplayer.res")
		pTempPanel = FindChildByName( "Label1" );

		if ( pTempPanel )
		{
			pTempPanel->SetVisible( false );
		}

		// #GameUI_ColorSliders (from "Resource/OptionsSubMultiplayer.res")
		pTempPanel = FindChildByName( "Colors" );

		if ( pTempPanel )
		{
			pTempPanel->SetVisible( false );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
COptionsSubPortalCollab::~COptionsSubPortalCollab()
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubPortalCollab::OnCommand( const char *command )
{
	BaseClass::OnCommand( command );
}

#define MODEL_MATERIAL_BASE_FOLDER "materials/vgui/playermodels/"

//-----------------------------------------------------------------------------
// Purpose: Builds model list
//-----------------------------------------------------------------------------
void COptionsSubPortalCollab::InitModelList( CLabeledCommandComboBox *cb )
{
	// Find out images
	int i = 0, initialItem = 0;

	cb->DeleteAllItems();
	FindVMTFilesInFolder( MODEL_MATERIAL_BASE_FOLDER, "", cb, i, initialItem );
	cb->SetInitialItem( initialItem );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubPortalCollab::RemapModel()
{
	const char *pModelName = m_pModelList->GetActiveItemCommand();
	
	if( pModelName == NULL )
		return;

	char texture[ 256 ];
	Q_snprintf ( texture, sizeof( texture ), "vgui/playermodels/%s", pModelName );
	texture[ strlen( texture ) - 4 ] = 0;

	m_pModelImage->setTexture( texture );
}


//-----------------------------------------------------------------------------
// Purpose: Called whenever model name changes
//-----------------------------------------------------------------------------
void COptionsSubPortalCollab::OnTextChanged(vgui::Panel *panel)
{
	RemapModel();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubPortalCollab::OnControlModified()
{
	PostMessage(GetParent(), new KeyValues("ApplyButtonEnable"));
	InvalidateLayout();
}

#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')
#define SUIT_HUE_START 192
#define SUIT_HUE_END 223
#define PLATE_HUE_START 160
#define PLATE_HUE_END 191

#ifdef POSIX 
typedef struct tagRGBQUAD { 
  uint8 rgbBlue;
  uint8 rgbGreen;
  uint8 rgbRed;
  uint8 rgbReserved;
} RGBQUAD;
#endif

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
static void PaletteHueReplace( RGBQUAD *palSrc, int newHue, int Start, int end )
{
	int i;
	float r, b, g;
	float maxcol, mincol;
	float hue, val, sat;

	hue = (float)(newHue * (360.0 / 255));

	for (i = Start; i <= end; i++)
	{
		b = palSrc[ i ].rgbBlue;
		g = palSrc[ i ].rgbGreen;
		r = palSrc[ i ].rgbRed;
		
		maxcol = max( max( r, g ), b ) / 255.0f;
		mincol = min( min( r, g ), b ) / 255.0f;
		
		val = maxcol;
		sat = (maxcol - mincol) / maxcol;

		mincol = val * (1.0f - sat);

		if (hue <= 120)
		{
			b = mincol;
			if (hue < 60)
			{
				r = val;
				g = mincol + hue * (val - mincol)/(120 - hue);
			}
			else
			{
				g = val;
				r = mincol + (120 - hue)*(val-mincol)/hue;
			}
		}
		else if (hue <= 240)
		{
			r = mincol;
			if (hue < 180)
			{
				g = val;
				b = mincol + (hue - 120)*(val-mincol)/(240 - hue);
			}
			else
			{
				b = val;
				g = mincol + (240 - hue)*(val-mincol)/(hue - 120);
			}
		}
		else
		{
			g = mincol;
			if (hue < 300)
			{
				b = val;
				r = mincol + (hue - 240)*(val-mincol)/(360 - hue);
			}
			else
			{
				r = val;
				b = mincol + (360 - hue)*(val-mincol)/(hue - 240);
			}
		}

		palSrc[ i ].rgbBlue = (unsigned char)(b * 255);
		palSrc[ i ].rgbGreen = (unsigned char)(g * 255);
		palSrc[ i ].rgbRed = (unsigned char)(r * 255);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubPortalCollab::ColorForName( char const *pszColorName, int&r, int&g, int&b )
{
	r = g = b = 0;
	
	int count = sizeof( itemlist ) / sizeof( itemlist[0] );

	for ( int i = 0; i < count; i++ )
	{
		if (!Q_strnicmp(pszColorName, itemlist[ i ].name, strlen(itemlist[ i ].name)))
		{
			r = itemlist[ i ].r;
			g = itemlist[ i ].g;
			b = itemlist[ i ].b;
			return;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void COptionsSubPortalCollab::OnApplyChanges()
{
	m_pPrimaryColorSlider->ApplyChanges();
	m_pSecondaryColorSlider->ApplyChanges();
//	m_pModelList->ApplyChanges();

	for ( int i=0; i<m_cvarToggleCheckButtons.GetCount(); ++i )
	{
		CCvarToggleCheckButton *toggleButton = m_cvarToggleCheckButtons[i];
		if( toggleButton->IsVisible() && toggleButton->IsEnabled() )
		{
			toggleButton->ApplyChanges();
		}
	}

	if ( m_pModelList && m_pModelList->IsVisible() && m_pModelList->GetActiveItemCommand() )
	{
		char cmd[512];
	
		Q_strncpy( m_ModelName, m_pModelList->GetActiveItemCommand(), sizeof( m_ModelName ) );
		Q_StripExtension( m_ModelName, m_ModelName, sizeof ( m_ModelName ) );
		
		// save the player model name
		Q_snprintf(cmd, sizeof(cmd), "cl_playermodel models%s.mdl\n", m_ModelName );
		engine->ClientCmd_Unrestricted(cmd);
	}
	else
	{
		m_ModelName[0] = 0;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Allow the res file to create controls on per-mod basis
//-----------------------------------------------------------------------------
Panel *COptionsSubPortalCollab::CreateControlByName( const char *controlName )
{
	if( !Q_stricmp( "CCvarToggleCheckButton", controlName ) )
	{
		CCvarToggleCheckButton *newButton = new CCvarToggleCheckButton( this, controlName, "", "" );
		m_cvarToggleCheckButtons.AddElement( newButton );
		return newButton;
	}
	else
	{
		return BaseClass::CreateControlByName( controlName );
	}
}