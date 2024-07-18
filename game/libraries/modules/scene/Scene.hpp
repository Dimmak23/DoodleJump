#pragma once

//* Game
#include "modules/scene/I_LocatableScene.hpp"

//* C++ std
#include <memory>
#include <vector>

//* Forward declarations
class IRelocatableActor;

class Scene : public ILocatableScene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	//@ API

	//* Setters

	void addPlayer(IRelocatableActor* actor);
	void addScoreBoardOrigin(IRelocatableActor* actor);
	virtual void addRelocatableActor(IRelocatableActor* actor) override;
	virtual void addRelocatableEnemy(IRelocatableActor* actor) override;
	virtual void addRelocatableWormhole(IRelocatableActor* actor) override;
	virtual void addRelocatableAmmo(IRelocatableActor* actor) override;
	virtual void removeRelocatableActorAtFront() override;
	virtual void removeRelocatableEnemyAtFront() override;
	virtual void removeRelocatableWormholeAtFront() override;
	virtual void removeRelocatableAmmoAtFront() override;
	virtual void onAmmoMissed(size_t index) override;
	virtual void onEnemyKilled(size_t index) override;
	virtual void onAmmoDestroyed(size_t index) override;

	void addBackground(IRelocatableActor* back);

	//* Manipulators
	//? Tell that camera moved
	virtual void relocateAll(int delta_x, int delta_y) override;

private:
	//@ Methods

	//* Let's prevent copying of the module
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	//@ Members

	//* Actors: moved when camera moving
	IRelocatableActor* _Player;
	// TODO: organize std::shared_ptr from one side and std::shared_ptr/std::weak_ptr from another side
	std::vector<IRelocatableActor*> _StaticActors;
	//* Enemies: moved when camera moving
	std::vector<IRelocatableActor*> _Enemies;
	//* Ammo tiles
	std::vector<IRelocatableActor*> _Ammos;
	//* How score board will found out about scene movement
	IRelocatableActor* _ScoreBoardOrigin{ nullptr };
	//* background is parallax, so relocate it
	IRelocatableActor* _Background{ nullptr };
	//* Wormholes
	std::vector<IRelocatableActor*> _Wormholes;
};
