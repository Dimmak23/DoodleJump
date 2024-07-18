#pragma once

//* Game
//? Interfaces
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

	void setParentEngine(IMechanics* parent);
	void setIsIgnoringBottomFrame(bool new_state);
	void setIsIgnoringBottomActor(bool new_state);
	void setIsIgnoringSidesFrame(bool new_state);
	void setParentLevel(IStoppable* level_instance);
	void setStaticWorldConnection(IAccounting* new_world);

	//* Manipulators: Actors

	//? Tell what frame we are blocking
	void addBlockedFrame(RectangleCore* frame);
	//? Tell what actors we are trying to block
	virtual void addBlockedActor(RectangleCore* actor) override;
	virtual void addBlockedEnemy(RectangleCore* actor) override;
	virtual void addBlockedWormhole(RectangleCore* actor) override;
	virtual void addShooterAmmo(RectangleCore* actor, IMechanics* engine) override;
	//? Tell that actor no longer exist
	virtual void removeBlockedActorAtFront() override;
	virtual void removeEnemyAtFront() override;
	virtual void removeWormholeAtFront() override;
	//?
	virtual void removeAmmoAtFront() override;

	//* Status
	void updateCollisions();

private:
	//@ Methods

	//* Let's prevent copying of the module
	Collider(const Collider&) = delete;
	Collider& operator=(const Collider&) = delete;

	//* Inside frame
	//? Check if owner body are inside of the frame
	void checkInside(IntersectionInfo* info);
	// TODO: optimize, it's dublicated
	void checkInsideAmmo(IntersectionInfo* info, RectangleCore* body);
	//?
	Point resolveCollision(IntersectionInfo* info);

	//* Outside block
	//? Check if owner body are outside of the frame
	bool checkOutside(RectangleCore* frame, bool& bIsOnTopOfPlatform);
	// TODO: optimize, it's dublicated
	bool checkOutsideBlockedActor(RectangleCore* killer, RectangleCore* frame, IntersectionInfo& info,
								  IMechanics* engine);
	void checkOutsideAllBlocked();
	bool doWeJump(RectangleCore* frame, IMechanics* player_engine);
	void checkOnTopOfAnyPlatform();
	void checkFightWithEnemies();
	void checkFallingToWormhole();
	// TODO: optimize, it's dublicated
	bool checkAmmoAimEnemy(RectangleCore* killer, IMechanics* engine);

	//* Events
	void onMoveCallBack();
	void onMessagingOnFrame(bool new_state);
	void onMessagingOnPlatform(bool new_state);
	void OnLevelStop();
	void onEnemyKilled(size_t index);
	void onAmmoDestroyed(size_t index);

	//@ Members

	//* Actors: Blocking
	RectangleCore* _BlockingFrame;
	//? std::pair<Actor, bOwnerIsOnTop>
	std::vector<std::pair<RectangleCore*, bool>> _BlockingActors;
	std::vector<RectangleCore*> _Enemies;
	std::vector<RectangleCore*> _Wormholes;
	//? std::pair<Actor, PhysicsEngine>
	std::vector<std::pair<RectangleCore*, IMechanics*>> _AmmoTiles;

	//* PhysicsEngine collision
	IMechanics* _parentEngine{ nullptr };

	//* Hosts
	RectangleCore* _hostCore{ nullptr };

	//* StaticWorld
	IAccounting* _CallbackWorld{ nullptr };

	//* Utilitites
	std::unique_ptr<IntersectionInfo> _info;
	bool _OnTopOfAnyBody{ false };
	bool _OnTopOfFrame{ false };
	bool _bIsIgnoringBottomFrame{ false };
	bool _bIsIgnoringBottomActor{ false };
	bool _bIsIgnoringSidesFrame{ false };

	//* Level
	IStoppable* _LevelInstance{ nullptr };
};
