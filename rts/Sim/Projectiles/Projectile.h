/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <vector>

#ifdef _MSC_VER
#pragma warning(disable:4291)
#endif

#include "ExplosionGenerator.h"
#include "System/float3.h"
#include "System/type2.h"

class CUnit;
class CFeature;
class CVertexArray;
class CMatrix44f;


class CProjectile: public CExpGenSpawnable
{
	CR_DECLARE(CProjectile)

	/// used only by creg
	CProjectile();

public:
	CProjectile(
		const float3& pos,
		const float3& spd,
		const CUnit* owner,
		bool isSynced,
		bool isWeapon,
		bool isPiece,
		bool isHitScan = false
	);
	virtual ~CProjectile();

	virtual void Collision();
	virtual void Collision(CUnit* unit);
	virtual void Collision(CFeature* feature);
	virtual void Update();
	virtual void Init(const CUnit* owner, const float3& offset);

	virtual void Draw() {}
	virtual void DrawOnMinimap(CVertexArray& lines, CVertexArray& points);
	virtual void DrawCallback() {}

	virtual int GetProjectilesCount() const
	{
		assert(false); // this method should be pure virtual, but cause of CREG we can't do so
		return 0;
	}

	// override WorldObject::SetVelocityAndSpeed so
	// we can keep <dir> in sync with speed-vector
	// (unlike other world objects, projectiles must
	// always point directly along their vel-vector)
	//
	// should be called when speed-vector is changed
	// s.t. both speed.w and dir need to be updated
	void SetVelocityAndSpeed(const float3& vel) {
		CWorldObject::SetVelocityAndSpeed(vel);

		if (speed.w > 0.0f) {
			dir = speed / speed.w;
		}
	}

	void SetDirectionAndSpeed(const float3& _dir, float _spd) {
		dir = _dir;
		speed.w = _spd;

		// keep speed-vector in sync with <dir>
		CWorldObject::SetVelocity(dir * _spd);
	}

	CUnit* owner() const;

	unsigned int GetOwnerID() const { return ownerID; }
	unsigned int GetTeamID() const { return teamID; }
	int GetAllyteamID() const { return allyteamID; }

	unsigned int GetProjectileType() const { return projectileType; }
	unsigned int GetCollisionFlags() const { return collisionFlags; }

	void SetCustomExplosionGeneratorID(unsigned int id) { cegID = id; }

	// UNSYNCED ONLY
	CMatrix44f GetTransformMatrix(bool offsetPos) const;

	float GetSortDist() const { return sortDist; }
	void SetSortDist(float d) { sortDist = d; }

public:
	static bool inArray;
	static CVertexArray* va;
	static void DrawArray();

	bool synced;  ///< is this projectile part of the simulation?
	bool weapon;  ///< is this a weapon projectile? (true implies synced true)
	bool piece;   ///< is this a piece projectile? (true implies synced true)
	bool hitscan; ///< is this a hit-scan projectile?

	bool luaMoveCtrl;
	bool checkCol;
	bool ignoreWater;
	bool deleteMe;

	bool castShadow;
	bool drawSorted;

	float3 dir;
	float3 drawPos;


	float mygravity;
	float sortDist; ///< distance used for z-sorting when rendering

protected:
	unsigned int ownerID;
	unsigned int teamID;
	int allyteamID;
	unsigned int cegID;

	unsigned int projectileType;
	unsigned int collisionFlags;

public:
	std::vector<int> quads;
};

#endif /* PROJECTILE_H */

