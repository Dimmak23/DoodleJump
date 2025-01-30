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

	void AddPlayer(IRelocatableActor* actor);
	void AddScoreBoardOrigin(IRelocatableActor* actor);
	virtual void AddRelocatableActor(IRelocatableActor* actor) override;
	virtual void AddRelocatableEnemy(IRelocatableActor* actor) override;
	virtual void AddRelocatableWormhole(IRelocatableActor* actor) override;
	virtual void AddRelocatableAmmo(IRelocatableActor* actor) override;
	virtual void RemoveRelocatableActorAtFront() override;
	virtual void RemoveRelocatableEnemyAtFront() override;
	virtual void RemoveRelocatableWormholeAtFront() override;
	virtual void RemoveRelocatableAmmoAtFront() override;
	virtual void OnAmmoMissed(long unsigned int index) override;
	virtual void OnEnemyKilled(long unsigned int index) override;
	virtual void OnAmmoDestroyed(long unsigned int index) override;

	void AddBackground(IRelocatableActor* back);

	//* Manipulators
	//? Tell that camera moved
	virtual void RelocateAll(int delta_x, int delta_y) override;

private:
	//@ Methods

	//* Let's prevent copying of the module
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	//@ Members

	// TODO: organize std::shared_ptr from one side and std::shared_ptr/std::weak_ptr from another side
	std::vector<IRelocatableActor*> _staticActors;
	//? Enemies: moved when camera moving
	std::vector<IRelocatableActor*> _enemies;
	//? Ammo tiles
	std::vector<IRelocatableActor*> _ammos;
	//? Wormholes
	std::vector<IRelocatableActor*> _wormholes;

	//? Actors: moved when camera moving
	IRelocatableActor* _player;

	//? How score board will found out about scene movement
	IRelocatableActor* _scoreBoardOrigin{ nullptr };

	//? background is parallax, so relocate it
	IRelocatableActor* _background{ nullptr };
};
