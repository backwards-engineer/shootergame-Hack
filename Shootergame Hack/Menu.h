#pragma once
#include "globals.h"
#include <XInput.h>
#include "ESP.h"
#include "Render.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <dwmapi.h>
#define DIRECTINPUT_VERSION 0x0800

int    OverlayLoop();


class MenuControl
{
public:
	bool	godMode;
	bool	infinitAmmo;
	bool	highJump;
	float	runningSpeed;
	bool	scopeIn;
	bool	highSpeed;
	bool	playerESP;
	bool	ItemESP;
	bool	ActorIdESP;
	float	ESPcolor[3];
	bool	showESPcolor;
	bool	aimbot;

	MenuControl()
	{
		aimbot = false;
		showESPcolor = false;
		ESPcolor[0] = 0.25;
		ESPcolor[1] = 0.15;
		ESPcolor[2] = 0.60;
		godMode = false;
		infinitAmmo = false;
		highJump = false;
		runningSpeed = 2.2;
		scopeIn = false;
		highSpeed = false;
		playerESP = false;
		ItemESP = false;
		ActorIdESP = false;
	}

};

extern MenuControl* menuControl;