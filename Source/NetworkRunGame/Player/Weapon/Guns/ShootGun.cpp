#include "NetworkRunGame.h"
#include "ShootGun.h"

AShootGun::AShootGun()
{
	SetAssets(
		"/Game/game/Weapon/Meshes/White_GrenadeLauncher",
		"/Game/game/Weapon/Blueprints/GrenadeMissle_BP",
		"/Game/game/Weapon/Particles/P_RocketLauncher_Explosion_Light");
}
