#pragma once

//* Game
#include "modules/collider/I_Collidable.hpp"

//* C++ std
#include <memory>
#include <utility>
#include <vector>

//* Forward declarations
class RectangleShape;
class RectangleCore;
class IMechanics;
class IStoppable;
struct Point;
struct IntersectionInfo;
class IMechanics;
class IAccounting;

class Collider : public ICollidable
{
public:
	Collider(RectangleCore* rectangle);
	~Collider();

	//@ API

	//* Setters

	void SetParentEngine(IMechanics* parent);
	void SetIsIgnoringBottomFrame(bool new_state);
	void SetIsIgnoringBottomActor(bool new_state);
	void SetIsIgnoringSidesFrame(bool new_state);
	void SetParentLevel(IStoppable* level_instance);
	void SetStaticWorldConnection(IAccounting* new_world);

	//* Manipulators: Actors

	//? Tell what frame we are blocking
	void AddBlockedFrame(RectangleCore* frame);
	//? Tell what actors we are trying to block
	virtual void AddBlockedActor(RectangleCore* actor) override;
	virtual void AddBlockedEnemy(RectangleCore* actor) override;
	virtual void AddBlockedWormhole(RectangleCore* actor) override;
	virtual void AddShooterAmmo(RectangleCore* actor, IMechanics* engine) override;
	//? Tell that actor no longer exist
	virtual void RemoveBlockedActorAtFront() override;
	virtual void RemoveEnemyAtFront() override;
	virtual void RemoveWormholeAtFront() override;
	//?
	virtual void RemoveAmmoAtFront() override;

	//* Status
	void UpdateCollisions();

private:
	//@ Methods

	//* Let's prevent copying of the module
	Collider(const Collider&) = delete;
	Collider& operator=(const Collider&) = delete;

	//* Inside frame
	//? Check if owner body are inside of the frame
	void CheckInside(IntersectionInfo* info);
	// TODO: optimize, it's dublicated
	void CheckInsideAmmo(IntersectionInfo* info, RectangleCore* body);
	//?
	Point ResolveCollision(IntersectionInfo* info);

	//* Outside block
	//? Check if owner body are outside of the frame
	bool CheckOutside(RectangleCore* frame, bool& bIsOnTopOfPlatform);
	// TODO: optimize, it's dublicated
	bool CheckOutsideBlockedActor(RectangleCore* killer, RectangleCore* frame, IntersectionInfo& info,
								  IMechanics* engine);
	void CheckOutsideAllBlocked();
	bool DoWeJump(RectangleCore* frame, IMechanics* player_engine);
	void CheckOnTopOfAnyPlatform();
	void CheckFightWithEnemies();
	void CheckFallingToWormhole();
	// TODO: optimize, it's dublicated
	bool CheckAmmoAimEnemy(RectangleCore* killer, IMechanics* engine);

	//* Events
	void OnMoveCallBack();
	void OnMessagingOnFrame(bool new_state);
	void OnMessagingOnPlatform(bool new_state);
	void OnLevelStop();
	void OnEnemyKilled(unsigned long int index);
	void OnAmmoDestroyed(unsigned long int index);

	//@ Members

	//? std::pair<Actor, bOwnerIsOnTop>
	std::vector<std::pair<RectangleCore*, bool>> _blockingActors;
	std::vector<RectangleCore*> _enemies;
	std::vector<RectangleCore*> _wormholes;
	//? std::pair<Actor, PhysicsEngine>
	std::vector<std::pair<RectangleCore*, IMechanics*>> _ammoTiles;

	//* Utilitites
	std::unique_ptr<IntersectionInfo> _info;

	//* Actors: Blocking
	RectangleCore* _blockingFrame;

	//* PhysicsEngine collision
	IMechanics* _parentEngine{ nullptr };

	//* Hosts
	RectangleCore* _hostCore{ nullptr };

	//* StaticWorld
	IAccounting* _callbackWorld{ nullptr };

	//* Level
	IStoppable* _levelInstance{ nullptr };

	bool _bOnTopOfAnyBody{ false };
	bool _bOnTopOfFrame{ false };
	bool _bIsIgnoringBottomFrame{ false };
	bool _bIsIgnoringBottomActor{ false };
	bool _bIsIgnoringSidesFrame{ false };
};
