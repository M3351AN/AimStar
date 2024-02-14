#pragma once
#include <math.h>
#include "..\Entity.h"
#include "..\View.hpp"
#include "..\MenuConfig.hpp"

namespace RCS
{
	inline int RCSBullet = 1;

	inline void UpdateAngles(const CEntity& Local, Vec2& Angles)
	{
		auto oldPunch = Vec2{ };
		auto shotsFired = Local.Pawn.ShotsFired;

		int ScreenCenterX = Gui.Window.Size.x / 2;
		int ScreenCenterY = Gui.Window.Size.y / 2;

		if (shotsFired)
		{
			uintptr_t clientState;
			auto viewAngles = Local.Pawn.ViewAngle;
			auto aimPunch = Local.Pawn.AimPunchAngle;

			auto newAngles = Vec2
			{
				viewAngles.x + oldPunch.x - aimPunch.x * 2.f,
				viewAngles.y + oldPunch.y - aimPunch.y * 2.f,
			};

			if (newAngles.x > 89.f)
				newAngles.x = 89.f;

			if (newAngles.x < -89.f)
				newAngles.x = -89.f;

			while (newAngles.y > 180.f)
				newAngles.y -= 360.f;

			while (newAngles.y < -180.f)
				newAngles.y += 360.f;

			newAngles.x += ScreenCenterX;
			newAngles.y += ScreenCenterY;
			Angles = newAngles;
		}
		else
		{
			oldPunch.x = oldPunch.y = 0.f;
		}

		if (Local.Pawn.ShotsFired > RCSBullet)
		{
			Vec2 PunchAngle;
			if (Local.Pawn.AimPunchCache.Count <= 0 && Local.Pawn.AimPunchCache.Count > 0xFFFF)
				return;
			if (!ProcessMgr.ReadMemory<Vec2>(Local.Pawn.AimPunchCache.Data + (Local.Pawn.AimPunchCache.Count - 1) * sizeof(Vec3), PunchAngle))
				return;

			Angles.x = PunchAngle.x;
			Angles.y = PunchAngle.y;
		}
	}
inline void StandaloneRCS(const CEntity& Local) {
    static Vec3 OldPunch;
    if (Local.Pawn.ShotsFired > RCSBullet) {
 
        Vec3 viewAngles = Local.Pawn.ViewAngle;
        Vec3 delta = viewAngles - ( viewAngles + ( OldPunch - ( Local.Pawn.AimPunchAngle * 2.0f ) ) ) // 2.0f for perfect RCS
 
        int mouse_angle_x = (int)(delta.y / (1.25 * 0.022f));
        int mouse_angle_y = (int)(delta.x / (1.25 * 0.022f));
        mouse_event(MOUSEEVENTF_MOVE, mouse_angle_x, -mouse_angle_y, 0, 0);
 
        OldPunch = Local.Pawn.AimPunchAngle * 2.0f;
    }
    else // reset old punch
        OldPunch = Vector3(0, 0, 0);
}
}
