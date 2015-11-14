#include "NetworkRunGame.h"
#include "BigBangGun.h"
#include "Player/Weapon/Projectiles/BigBangGunProjectile.h"
#include "Player/FitPlayerController.h"
#include "Player/FitCharacter.h"
#include "Player/FitPlayerState.h"
#include "World/FitGameState.h"
#include "Enemy/EnemyCharacter.h"

ABigBangGun::ABigBangGun()
{
	SetAssets(
		"/Game/SciFiWeapDark/Weapons/Darkness_RocketLauncher",
		"/Game/game/Weapon/Blueprints/BFGMissle_BP",
		"/Game/game/Weapon/Particles/P_Impact_Wood_Large_Light");
}
