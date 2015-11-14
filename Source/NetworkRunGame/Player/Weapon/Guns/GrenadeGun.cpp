#include "NetworkRunGame.h"
#include "GrenadeGun.h"

AGrenadeGun::AGrenadeGun()
{
	SetAssets(
		"/Game/SciFiWeapDark/Weapons/Darkness_RocketLauncher",
		"/Game/game/Weapon/Blueprints/GrenadeMissle_BP",
		"/Game/game/Weapon/Particles/P_RocketLauncher_Explosion_Light");
}
