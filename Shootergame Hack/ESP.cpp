﻿#include "ESP.h"
#include "Render.h"
#include "Menu.h"




bool WorldToScreen(vector3D world, vector2D* screen, vector3D Rotation, vector3D cameraPosition, float FovAngle) {

	vector3D Screenlocation = vector3D(0, 0, 0);

	vMatrix tempMatrix = Matrix(Rotation, Screenlocation);

	vector3D vAxisX, vAxisY, vAxisZ;

	vAxisX = vector3D(tempMatrix[0][0], tempMatrix[0][1], tempMatrix[0][2]);
	vAxisY = vector3D(tempMatrix[1][0], tempMatrix[1][1], tempMatrix[1][2]);
	vAxisZ = vector3D(tempMatrix[2][0], tempMatrix[2][1], tempMatrix[2][2]);

	vector3D vDelta = world - cameraPosition;
	vector3D vTransformed = vector3D(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;


	float ScreenCenterX = Globals->windowWidth / 2.0f;
	float ScreenCenterY = Globals->windowHeight / 2.0f;

	auto tmpFOV = tanf(FovAngle * (float)PI / 360.f);

	screen->x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tmpFOV) / vTransformed.z;
	screen->y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)PI / 360.f)) / vTransformed.z;


	return true;
}




DWORD64		currentActor;
DWORD64		rootComponent;
DWORD64		cameraManager;
vector3D	cameraLocation;
vector3D	cameraAngle;
float		cameraFOV;
vector3D	xyz;


int ESP(bool players, bool items, bool showActorID)
{

	for (int i = 0; i < localGameInfo->actorCount; ++i)
	{
		currentActor = Commands::read<DWORD64>(Globals->processHandle, (localGameInfo->actorList + 0x8 * i));

		int actorID = Commands::read<DWORD64>(Globals->processHandle, (currentActor + 0x18));

		rootComponent = Commands::read<DWORD64>(Globals->processHandle, (currentActor + Offsets::RootComponent));

		xyz = Commands::read<vector3D>(Globals->processHandle, (rootComponent + Offsets::Translation));

		cameraManager = Commands::read<DWORD64>(Globals->processHandle, (localGameInfo->playerController + Offsets::CameraManager));

		cameraLocation = Commands::read<vector3D>(Globals->processHandle, (cameraManager + Offsets::cameraCachePrivate + Offsets::POV +
			Offsets::cameraVector));

		cameraAngle = Commands::read<vector3D>(Globals->processHandle,
			(cameraManager + Offsets::cameraCachePrivate + Offsets::POV + Offsets::cameraRotation));

		cameraFOV = Commands::read<float>(Globals->processHandle, (cameraManager + Offsets::cameraCachePrivate + Offsets::POV + Offsets::cameraFOV));
		int health = Commands::read<float>(Globals->processHandle, (currentActor + Offsets::pawnHealth));



		vector2D	screenCoords(0, 0);
		bool hi = WorldToScreen(xyz, &screenCoords, cameraAngle, cameraLocation, cameraFOV);

		if (hi)
		{
			if (players)
			{
				/* 526275 for enemy, 325547 for local */
				if (actorID == localGameInfo->localactorId || actorID == localGameInfo->localactorId + 200728 || actorID == 526275)
				{
					if (health > 0)
					{
						float distance = localGameInfo->position.distance(xyz);

						Render::DrawBox(ImColor(menuControl->ESPcolor[0], menuControl->ESPcolor[1], menuControl->ESPcolor[2], 1.0f),
							screenCoords.x - (35000/distance), screenCoords.y - (45000/distance), 45000 / distance, 100000 / distance);
					}
				}
			}

			if (showActorID)
			{
				std::stringstream	ss;
				ss << std::dec << actorID;
				Render::EasyText(ImVec2(screenCoords.x, screenCoords.y), ImColor(menuControl->ESPcolor[0], menuControl->ESPcolor[1],
					menuControl->ESPcolor[2], 1.0f), ss.str().c_str(), 1100);
			}

			/*	899147 for rockets, 457713 for health	*/
			if (items)
			{
				switch (actorID)
				{
				case 457655:
					Render::EasyText(ImVec2(screenCoords.x, screenCoords.y), ImColor(menuControl->ESPcolor[0], menuControl->ESPcolor[1],
						menuControl->ESPcolor[2], 1.0f), "Rockets", 2000);
					break;
				case 457713:
					Render::EasyText(ImVec2(screenCoords.x, screenCoords.y), ImColor(menuControl->ESPcolor[0], menuControl->ESPcolor[1],
						menuControl->ESPcolor[2], 1.0f), "Health kit", 2000);
					break;
				case 457591:
					Render::EasyText(ImVec2(screenCoords.x, screenCoords.y), ImColor(menuControl->ESPcolor[0], menuControl->ESPcolor[1],
						menuControl->ESPcolor[2], 1.0f), "Bullets", 2000);
					break;
				default:
					break;
				}
			}
		}
	}

	return 0;
}

