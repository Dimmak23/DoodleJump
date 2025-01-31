//* Source header
#include "Collider.hpp"

//* Game
//? Modules
//? Inheritance
#include "rectangle/RectangleShape.hpp"
//? Interfaces
#include "level/level/I_Stoppable.hpp"
#include "level/world/interface/I_Accounting.hpp"
#include "modules/physics_engine/I_Mechanics.hpp"
//? Level
#include "level/level/Level.hpp"

//* C++ std
#include <format>
#include <iostream>

Collider::Collider(RectangleCore* rectangle) : _hostCore(rectangle)
{
	_info = std::make_unique<IntersectionInfo>();
	// std::cout << "Constructed Collider size of: " << sizeof(*this) << '\n';
}

Collider::~Collider()
{
	// delete _hostCore; //! can't do this, it's hurt owner: SpriteWrapper
	_hostCore = nullptr;
	// std::cout << "Deleted Collider...\n";
}

void Collider::SetParentEngine(IMechanics* parent) { _parentEngine = parent; }

void Collider::SetIsIgnoringBottomFrame(bool new_state) { _bIsIgnoringBottomFrame = new_state; }

void Collider::SetIsIgnoringBottomActor(bool new_state) { _bIsIgnoringBottomActor = new_state; }

void Collider::SetIsIgnoringSidesFrame(bool new_state) { _bIsIgnoringSidesFrame = new_state; }

void Collider::SetParentLevel(IStoppable* level_instance) { _levelInstance = level_instance; }

void Collider::SetStaticWorldConnection(IAccounting* new_world) { _callbackWorld = new_world; }

void Collider::AddBlockedFrame(RectangleCore* frame) { _blockingFrame = frame; }

void Collider::AddBlockedActor(RectangleCore* actor) { _blockingActors.push_back(std::make_pair(actor, false)); }

void Collider::AddBlockedEnemy(RectangleCore* actor) { _enemies.push_back(actor); }

void Collider::AddBlockedWormhole(RectangleCore* actor) { _wormholes.push_back(actor); }

void Collider::AddShooterAmmo(RectangleCore* actor, IMechanics* engine)
{
	_ammoTiles.push_back(std::make_pair(actor, engine));
}

void Collider::RemoveBlockedActorAtFront()
{
	if (!_blockingActors.empty())
	{
		_blockingActors.erase(_blockingActors.begin());
	}
}

void Collider::RemoveEnemyAtFront()
{
	if (!_enemies.empty())
	{
		_enemies.erase(_enemies.begin());
	}
}

void Collider::RemoveWormholeAtFront()
{
	if (!_wormholes.empty())
	{
		_wormholes.erase(_wormholes.begin());
	}
}

void Collider::RemoveAmmoAtFront()
{
	if (!_ammoTiles.empty())
	{
		_ammoTiles.erase(_ammoTiles.begin());
	}
}

void Collider::UpdateCollisions()
{
	//? Maybe player goes to side of the frame, fall off from screen
	if (_blockingFrame)
	{
		CheckInside(_info.get());
	}

	//? Do we collide any platfrom
	if (!_parentEngine->GetIsJumping())	   //? Got thru platforms when jumping
	{
		CheckOutsideAllBlocked();
	}	 //? But not: when flying

	if ((_parentEngine->GetIsJumping() || _parentEngine->GetIsFalling()) && _callbackWorld)
	{
		_callbackWorld->OnFlyingMessaging();
	}

	//? Tell that player is on the platform
	CheckOnTopOfAnyPlatform();

	//? Maybe game will be restarted here
	CheckFightWithEnemies();

	//? Maybe game will be restarted here
	CheckFallingToWormhole();

	//? Check that all ammo inside frame
	if (_blockingFrame)
	{
		for (auto& Ammo : _ammoTiles)
		{
			std::unique_ptr<IntersectionInfo> info = std::make_unique<IntersectionInfo>();
			CheckInsideAmmo(info.get(), Ammo.first);
			if (Ammo.second)
			{
				Ammo.second->Move(ResolveCollision(info.get()));
			}
		}
	}

	//? Check maybe ammo killed enemy
	auto iterator = std::begin(_ammoTiles);
	while (iterator != std::end(_ammoTiles))
	{
		if (CheckAmmoAimEnemy(iterator->first, iterator->second))
		{
			//? Ammo destroyed enemy, now destroy ammo
			OnAmmoDestroyed(iterator - std::begin(_ammoTiles));
			iterator = _ammoTiles.erase(iterator);	  // erase returns iterator to the next valid element
		}
		else
		{
			++iterator;
		}
	}
}

void Collider::CheckInside(IntersectionInfo* info)
{
	info->direction = IntersectionDirection::NONE;
	info->size = { 0, 0 };

	if (_hostCore->Bottom() < _blockingFrame->Bottom())
	{
		OnMessagingOnFrame(false);
	}

	if (_hostCore->Left() < _blockingFrame->Left())
	{
		info->direction = IntersectionDirection::LEFT;
		if (_bIsIgnoringSidesFrame)
		{
			info->size.width =
				_blockingFrame->Left() - _hostCore->Left() + _blockingFrame->Width() - _hostCore->Width();
		}
		else
		{
			info->size.width = _blockingFrame->Left() - _hostCore->Left();
		}

		OnMoveCallBack();
	}
	else if (_hostCore->Right() > _blockingFrame->Right())
	{
		info->direction = IntersectionDirection::RIGHT;
		if (_bIsIgnoringSidesFrame)
		{
			info->size.width =
				_hostCore->Right() - _blockingFrame->Right() + _blockingFrame->Width() - _hostCore->Width();
		}
		else
		{
			info->size.width = _hostCore->Right() - _blockingFrame->Right();
		}

		OnMoveCallBack();
	}
	else if (_hostCore->Top() < _blockingFrame->Top())
	{
		info->direction = IntersectionDirection::TOP;
		info->size.height = _blockingFrame->Top() - _hostCore->Top();

		OnMoveCallBack();
	}
	else if ((_hostCore->Bottom() >= _blockingFrame->Bottom()) && !_bIsIgnoringBottomFrame)
	{
		info->direction = IntersectionDirection::BOTTOM;
		info->size.height = _hostCore->Bottom() - _blockingFrame->Bottom();

		OnMessagingOnFrame(true);

		OnMoveCallBack();
	}
	else if (_hostCore->Top() >= _blockingFrame->Bottom())
	{
		OnLevelStop();
	}
}

void Collider::CheckInsideAmmo(IntersectionInfo* info, RectangleCore* body)
{
	info->direction = IntersectionDirection::NONE;
	info->size = { 0, 0 };

	if (body->Left() < _blockingFrame->Left())
	{
		info->direction = IntersectionDirection::LEFT;
		info->size.width = _blockingFrame->Left() - body->Left() + _blockingFrame->Width() - body->Width();
	}
	else if (body->Right() > _blockingFrame->Right())
	{
		info->direction = IntersectionDirection::RIGHT;
		info->size.width = body->Right() - _blockingFrame->Right() + _blockingFrame->Width() - body->Width();
	}
}

Point Collider::ResolveCollision(IntersectionInfo* info)
{
	Point result{ 0, 0 };
	switch (info->direction)
	{
		case IntersectionDirection::TOP: {
			result.y = info->size.height;
			break;
		}
		case IntersectionDirection::BOTTOM: {
			result.y = -info->size.height;
			break;
		}
		case IntersectionDirection::LEFT: {
			result.x = info->size.width;
			break;
		}
		case IntersectionDirection::RIGHT: {
			result.x = -info->size.width;
			break;
		}
		default: {
			break;	  // TODO: what's up with this
		}
	}
	return result;
}

bool Collider::CheckOutside(RectangleCore* frame, bool& bIsOnTopOfPlatform)
{
	if ((_hostCore->Bottom() < frame->Top()) ||	   //
		(_hostCore->Top() > frame->Bottom()) ||	   //
		(_hostCore->Right() < frame->Left()) ||	   //
		(_hostCore->Left() > frame->Right()))
	{
		bIsOnTopOfPlatform = false;
	}

	//? small owner intersect big size of platfrom at top
	if (																					 //
		(((frame->Left() < _hostCore->Left()) && (_hostCore->Left() < frame->Right()))		 //
		 ||																					 //
		 ((frame->Left() < _hostCore->Right()) && (_hostCore->Right() < frame->Right())))	 //
		&&																					 //
		(_hostCore->Bottom() > frame->Top()) && (_hostCore->Top() < frame->Top())			 //
		// &&																					 //
		// (_parentEngine->getLinearSpeed()._Vy <= 0)	  //* if we floating up don't pull host from bottom to top,
		// 											  //* allow host to float with it is own speed
	)
	{
		_info->direction = IntersectionDirection::TOP;
		_info->size.height = -(_hostCore->Bottom() - frame->Top());

		bIsOnTopOfPlatform = true;
		return true;
	}
	//? small owner intersect big size of platfrom at bottom
	else if (																				 //
		(((frame->Left() < _hostCore->Left()) && (_hostCore->Left() < frame->Right()))		 //
		 ||																					 //
		 ((frame->Left() < _hostCore->Right()) && (_hostCore->Right() < frame->Right())))	 //
		&&																					 //
		((_hostCore->Top() < frame->Bottom()) && (_hostCore->Bottom() > frame->Bottom()))	 //
		&&																					 //
		!_bIsIgnoringBottomActor	//* don't allow to collide when moving from bottom to top
	)
	{
		_info->direction = IntersectionDirection::BOTTOM;
		_info->size.height = -(frame->Bottom() - _hostCore->Top());

		return true;
	}
	//? small owner intersect small size of platfrom at left
	else if (																				   //
		(((_hostCore->Top() < frame->Top()) && (frame->Top() < _hostCore->Bottom()))		   //
		 ||																					   //
		 ((_hostCore->Top() < frame->Bottom()) && (frame->Bottom() < _hostCore->Bottom())))	   //
		&&																					   //
		((_hostCore->Left() < frame->Right()) && (_hostCore->Right() > frame->Right()))		   //
		&&																					   //
		(_parentEngine->GetLinearSpeed()._Vy > 0)	 //* if we floating up don't pull host from bottom to top,
													 //* allow host to float with it is own speed
													 // &&											 //
													 // !(_parentEngine->getIsJumping())			 //
	)
	{
		_info->direction = IntersectionDirection::LEFT;
		_info->size.width = (frame->Right() - _hostCore->Left());

		return true;
	}
	//? small owner intersect small size of platfrom at right
	else if (																				   //
		(((_hostCore->Top() < frame->Top()) && (frame->Top() < _hostCore->Bottom()))		   //
		 ||																					   //
		 ((_hostCore->Top() < frame->Bottom()) && (frame->Bottom() < _hostCore->Bottom())))	   //
		&&																					   //
		(_hostCore->Right() > frame->Left()) && (_hostCore->Left() < frame->Left())			   //
		&&																					   //
		(_parentEngine->GetLinearSpeed()._Vy > 0)	 //* if we floating up don't pull host from bottom to top,
													 //* allow host to float with it is own speed
													 // &&											 //
													 // !(_parentEngine->getIsJumping())			 //
	)
	{
		_info->direction = IntersectionDirection::RIGHT;
		_info->size.width = (_hostCore->Right() - frame->Left());

		return true;
	}
	else
	{
		return false;
	}
}

bool Collider::CheckOutsideBlockedActor(RectangleCore* killer, RectangleCore* frame, IntersectionInfo& info,
										IMechanics* engine)
{
	if ((killer->Bottom() < frame->Top()) ||	//
		(killer->Top() > frame->Bottom()) ||	//
		(killer->Right() < frame->Left()) ||	//
		(killer->Left() > frame->Right()))
	{
		return false;	 //? No fight
	}

	//? small owner intersect big size of platfrom at top
	if (																				 //
		((frame->Top() < killer->Bottom()) && (killer->Bottom() < frame->Center().y))	 //
		&&																				 //
		(engine->GetLinearSpeed()._Vy > 0)	  //* help recognize bottom from top
	)
	{
		info.direction = IntersectionDirection::TOP;	//? Player wins
		return true;									//? Got fight
	}
	//? small owner intersect big size of platfrom at bottom
	else if (																				//
		(((frame->Left() < killer->Center().x) && (killer->Center().x < frame->Right()))	//
		 &&																					//
		 ((frame->Center().y < killer->Top()) && (killer->Top() < frame->Bottom())))		//

	)
	{
		info.direction = IntersectionDirection::BOTTOM;	   //? Player loose
		return true;									   //? Got fight
	}
	//? small owner intersect small size of platfrom at left
	else if (																			 //
		(((killer->Top() < frame->Top()) && (frame->Top() < killer->Bottom()))			 //
		 ||																				 //
		 ((killer->Top() < frame->Bottom()) && (frame->Bottom() < killer->Bottom())))	 //
		&&																				 //
		((killer->Left() < frame->Right()) && (killer->Right() > frame->Right()))		 //
	)
	{
		info.direction = IntersectionDirection::LEFT;	 //? Player loose
		return true;									 //? Got fight
	}
	//? small owner intersect small size of platfrom at right
	else if (																			 //
		(((killer->Top() < frame->Top()) && (frame->Top() < killer->Bottom()))			 //
		 ||																				 //
		 ((killer->Top() < frame->Bottom()) && (frame->Bottom() < killer->Bottom())))	 //
		&&																				 //
		(killer->Right() > frame->Left()) && (killer->Left() < frame->Left())			 //
	)
	{
		info.direction = IntersectionDirection::RIGHT;	  //? Player loose
		return true;									  //? Got fight
	}
	// TODO: This is probably redudant
	else
	{
		return false;	 //? No fight
	}
}

void Collider::CheckOutsideAllBlocked()
{
	for (auto& Actor : _blockingActors)
	{
		if (CheckOutside(Actor.first, Actor.second))
		{
			OnMoveCallBack();
		}
	}
	//? Not a single collision
}

bool Collider::DoWeJump(RectangleCore* frame, IMechanics* player_engine)
{
	if (																					//
		(_hostCore->Left() < frame->Left()) && (player_engine->GetLinearSpeed()._Vx < 0)	//
	)
	{
		return true;
	}
	else if (																				  //
		(_hostCore->Right() > frame->Right()) && (player_engine->GetLinearSpeed()._Vx > 0)	  //

	)
	{
		return true;
	}

	return false;
}

void Collider::CheckOnTopOfAnyPlatform()
{
	long unsigned int Counter{};

	for (auto& Actor : _blockingActors)
	{
		if (Actor.second)
		{
			OnMessagingOnPlatform(true);

			if (!_parentEngine->GetIsJumping() && !_parentEngine->GetIsFalling() && _callbackWorld)
			{
				_callbackWorld->OnTopOfPlatformMessaging(Counter);
			}
			// else if ((_parentEngine->getIsJumping() || _parentEngine->getIsFalling()) && _CallbackWorld)
			// {
			// 	_CallbackWorld->onFlyingMessaging();
			// }

			if (!_parentEngine->GetIsJumping() && (DoWeJump(Actor.first, _parentEngine)))
			{
				_parentEngine->SetIsJumping(true);
			}
			return;	   //? Don't forget to exit cycle, cause value will be erased after it
		}
		Counter++;
	}
	OnMessagingOnPlatform(false);
}

void Collider::CheckFightWithEnemies()
{
	for (auto iterator{ _enemies.begin() }; iterator < _enemies.end(); iterator++)
	{
		IntersectionInfo info;
		if (CheckOutsideBlockedActor(_hostCore, *iterator, info, _parentEngine))
		{
			switch (info.direction)
			{
				case IntersectionDirection::TOP: {
					//? Kill enemy
					OnEnemyKilled(iterator - _enemies.begin());	   //? Sending signal to WorldDynamic
					_enemies.erase(iterator);
					return;	   //? No need to look for another enemies`
					break;
				}
				case IntersectionDirection::BOTTOM:
				case IntersectionDirection::LEFT:
				case IntersectionDirection::RIGHT: {
					//? Kill player
					OnLevelStop();	  //? Immidiately finish level
					return;			  //? No need to look for another enemies`
					break;
				}
				default: {
					break;	  // TODO: what's up with this?
				}
			}
		}
	}
	//? Not a single fight
}

void Collider::CheckFallingToWormhole()
{
	for (auto iterator{ _wormholes.begin() }; iterator < _wormholes.end(); iterator++)
	{
		IntersectionInfo info;
		if (CheckOutsideBlockedActor(_hostCore, *iterator, info, _parentEngine))
		{
			switch (info.direction)
			{
				//? In any case level restarts
				case IntersectionDirection::TOP:
				case IntersectionDirection::BOTTOM:
				case IntersectionDirection::LEFT:
				case IntersectionDirection::RIGHT: {
					//? Kill player
					OnLevelStop();	  //? Immidiately finish level
					return;			  //? No need to look for another enemies`
					break;
				}
				default: {
					break;	  // TODO: what's up with this?
				}
			}
		}
	}
	//? Not a single fall
}

bool Collider::CheckAmmoAimEnemy(RectangleCore* killer, IMechanics* engine)
{
	for (auto iterator{ _enemies.begin() }; iterator < _enemies.end(); iterator++)
	{
		IntersectionInfo info;
		if (CheckOutsideBlockedActor(killer, *iterator, info, engine))
		{
			switch (info.direction)
			{
				case IntersectionDirection::TOP:
				case IntersectionDirection::BOTTOM:
				case IntersectionDirection::LEFT:
				case IntersectionDirection::RIGHT: {
					//? Sending signal to WorldDynamic
					OnEnemyKilled(iterator - _enemies.begin());
					//? Kill enemy
					_enemies.erase(iterator);
					return true;	//? No need to look for another enemies
					break;
				}
				default: {
					return false;
					break;	  // TODO: what's up with this?
				}
			}
		}
	}

	return false;	 //? Not a single fight
}

void Collider::OnMoveCallBack()
{
	if (_parentEngine)
	{
		_parentEngine->Move(ResolveCollision(_info.get()));
	}
}

void Collider::OnMessagingOnFrame(bool new_state)
{
	if (_parentEngine)
	{
		_parentEngine->SetOnTopOfFrame(new_state);
	}
}

void Collider::OnMessagingOnPlatform(bool new_state)
{
	if (_parentEngine)
	{
		_parentEngine->SetOnTopOfAnyPlatform(new_state);
	}
}

void Collider::OnLevelStop()
{
	if (_levelInstance)
	{
		_levelInstance->OnLevelStop();
	}
}

void Collider::OnEnemyKilled(unsigned long int index)
{
	if (_levelInstance)
	{
		_levelInstance->OnEnemyKilled(index);
	}
}

void Collider::OnAmmoDestroyed(unsigned long int index)
{
	if (_levelInstance)
	{
		_levelInstance->OnAmmoDestroyed(index);
	}
}
