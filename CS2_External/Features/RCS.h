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
    inline void StandaloneRCS(const CEntity& Local, Vec3 LocalPos)
	{
	if (MenuConfig::ShowMenu)
		return;
       if (Local.Pawn.ShotsFired < RCSBullet)
	       return;
       float Yaw, Pitch;
        float Distance, Norm, Length;
        Vec3 AimPos, OppPos;
        Vec2 Angles{ 0,0 };
        int ScreenCenterX = Gui.Window.Size.x / 2;
        int ScreenCenterY = Gui.Window.Size.y / 2;
        float TargetX = 0.f;
        float TargetY = 0.f;
        float yawf = Local.Pawn.ViewAngle.y * M_PI / 180;        
        float pitchf = Local.Pawn.ViewAngle.x * M_PI / 180;
                // RCS by @Tairitsu modi. @_ukia_
        RCS::UpdateAngles(Local, Angles);
		if (Angles.y == 0)
			return;
AimPos.x = 15 * cosf(yawf) * sinf(pitchf);
AimPos.y = 15 * sinf(yawf) * sinf(pitchf);
AimPos.z = 15 * cosf(pitchf);


        OppPos = AimPos;

        Distance = sqrt(pow(OppPos.x, 2) + pow(OppPos.y, 2));

        Length = sqrt(Distance * Distance + OppPos.z * OppPos.z);
            float rad = Angles.x / 180.f * M_PI;
            float si = sinf(rad);
            float co = cosf(rad);

            float z = OppPos.z * co + Distance * si;
            float d = (Distance * co - OppPos.z * si) / Distance;

            rad = -Angles.y / 180.f * M_PI;
            si = sinf(rad);
            co = cosf(rad);

            float x = (OppPos.x * co - OppPos.y * si) * d;
            float y = (OppPos.x * si + OppPos.y * co) * d;

            OppPos = Vec3{ x, y, z };

            AimPos = LocalPos + OppPos;

        Vec2 ScreenPos;
        gGame.View.WorldToScreen(Vec3(AimPos), ScreenPos);
        // Shake Fixed by @Sweely
            if (ScreenPos.x != ScreenCenterX)
            {
                TargetX = (ScreenPos.x > ScreenCenterX) ? -(ScreenCenterX - ScreenPos.x) : ScreenPos.x - ScreenCenterX;
                TargetX /= 1.4f;
                TargetX = (TargetX + ScreenCenterX > ScreenCenterX * 2 || TargetX + ScreenCenterX < 0) ? 0 : TargetX;
            }

            if (ScreenPos.y != 0)
            {
                if (ScreenPos.y != ScreenCenterY)
                {
                    TargetY = (ScreenPos.y > ScreenCenterY) ? -(ScreenCenterY - ScreenPos.y) : ScreenPos.y - ScreenCenterY;
                    TargetY /= 1.4f;
                    TargetY = (TargetY + ScreenCenterY > ScreenCenterY * 2 || TargetY + ScreenCenterY < 0) ? 0 : TargetY;
                }
            }
        mouse_event(MOUSEEVENTF_MOVE, TargetX, TargetY, NULL, NULL);
        return;

    }
}
