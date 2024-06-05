//* Source header
#include "Collider.hpp"

//* DL_Framework

//* C++ std
#include <format>
#include <iostream>

//* Custom
//? Modules
//? Inheritance
#include "RectangleShape.hpp"
//? Interfaces
#include "I_Accounting.hpp"
#include "I_Mechanics.hpp"
#include "I_Stoppable.hpp"
//? Level
#include "Level.hpp"

Collider::Collider(RectangleCore* rectangle)
    : _hostCore(rectangle)
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

void Collider::setParentEngine(IMechanics* parent) { _parentEngine = parent; }

void Collider::setIsIgnoringBottomFrame(bool new_state) { _bIsIgnoringBottomFrame = new_state; }

void Collider::setIsIgnoringBottomActor(bool new_state) { _bIsIgnoringBottomActor = new_state; }

void Collider::setIsIgnoringSidesFrame(bool new_state) { _bIsIgnoringSidesFrame = new_state; }

void Collider::setParentLevel(IStoppable* level_instance) { _LevelInstance = level_instance; }

void Collider::setStaticWorldConnection(IAccounting* new_world) { _CallbackWorld = new_world; }

void Collider::addBlockedFrame(RectangleCore* frame) { _BlockingFrame = frame; }

void Collider::addBlockedActor(RectangleCore* actor) { _BlockingActors.push_back(std::make_pair(actor, false)); }

void Collider::addBlockedEnemy(RectangleCore* actor) { _Enemies.push_back(actor); }

void Collider::addBlockedWormhole(RectangleCore* actor) { _Wormholes.push_back(actor); }

void Collider::addShooterAmmo(RectangleCore* actor, IMechanics* engine)
{
    _AmmoTiles.push_back(std::make_pair(actor, engine));
}

void Collider::removeBlockedActorAtFront()
{
    if (!_BlockingActors.empty()) {
        _BlockingActors.erase(_BlockingActors.begin());
    }
}

void Collider::removeEnemyAtFront()
{
    if (!_Enemies.empty()) {
        _Enemies.erase(_Enemies.begin());
    }
}

void Collider::removeWormholeAtFront()
{
    if (!_Wormholes.empty()) {
        _Wormholes.erase(_Wormholes.begin());
    }
}

void Collider::removeAmmoAtFront()
{
    if (!_AmmoTiles.empty()) {
        _AmmoTiles.erase(_AmmoTiles.begin());
    }
}

void Collider::updateCollisions()
{
    //? Maybe player goes to side of the frame, fall off from screen
    if (_BlockingFrame) {
        checkInside(_info.get());
    }

    //? Do we collide any platfrom
    if (!_parentEngine->getIsJumping()) //? Got thru platforms when jumping
    {
        checkOutsideAllBlocked();
    } //? But not: when flying

    if ((_parentEngine->getIsJumping() || _parentEngine->getIsFalling()) && _CallbackWorld) {
        _CallbackWorld->onFlyingMessaging();
    }

    //? Tell that player is on the platform
    checkOnTopOfAnyPlatform();

    //? Maybe game will be restarted here
    checkFightWithEnemies();

    //? Maybe game will be restarted here
    checkFallingToWormhole();

    //? Check that all ammo inside frame
    if (_BlockingFrame) {
        for (auto& Ammo : _AmmoTiles) {
            std::unique_ptr<IntersectionInfo> info = std::make_unique<IntersectionInfo>();
            checkInsideAmmo(info.get(), Ammo.first);
            if (Ammo.second) {
                Ammo.second->move(resolveCollision(info.get()));
            }
        }
    }

    //? Check maybe ammo killed enemy
    auto iterator = std::begin(_AmmoTiles);
    while (iterator != std::end(_AmmoTiles)) {
        if (checkAmmoAimEnemy(iterator->first, iterator->second)) {
            //? Ammo destroyed enemy, now destroy ammo
            onAmmoDestroyed(iterator - std::begin(_AmmoTiles));
            iterator = _AmmoTiles.erase(iterator); // erase returns iterator to the next valid element
        } else {
            ++iterator;
        }
    }
}

void Collider::checkInside(IntersectionInfo* info)
{
    info->direction = IntersectionDirection::NONE;
    info->size = { 0, 0 };

    if (_hostCore->bottom() < _BlockingFrame->bottom()) {
        onMessagingOnFrame(false);
    }

    if (_hostCore->left() < _BlockingFrame->left()) {
        info->direction = IntersectionDirection::LEFT;
        if (_bIsIgnoringSidesFrame) {
            info->size.width = _BlockingFrame->left() - _hostCore->left() + _BlockingFrame->width() - _hostCore->width();
        } else {
            info->size.width = _BlockingFrame->left() - _hostCore->left();
        }

        onMoveCallBack();
    } else if (_hostCore->right() > _BlockingFrame->right()) {
        info->direction = IntersectionDirection::RIGHT;
        if (_bIsIgnoringSidesFrame) {
            info->size.width = _hostCore->right() - _BlockingFrame->right() + _BlockingFrame->width() - _hostCore->width();
        } else {
            info->size.width = _hostCore->right() - _BlockingFrame->right();
        }

        onMoveCallBack();
    } else if (_hostCore->top() < _BlockingFrame->top()) {
        info->direction = IntersectionDirection::TOP;
        info->size.height = _BlockingFrame->top() - _hostCore->top();

        onMoveCallBack();
    } else if ((_hostCore->bottom() >= _BlockingFrame->bottom()) && !_bIsIgnoringBottomFrame) {
        info->direction = IntersectionDirection::BOTTOM;
        info->size.height = _hostCore->bottom() - _BlockingFrame->bottom();

        onMessagingOnFrame(true);

        onMoveCallBack();
    } else if (_hostCore->top() >= _BlockingFrame->bottom()) {
        OnLevelStop();
    }
}

void Collider::checkInsideAmmo(IntersectionInfo* info, RectangleCore* body)
{
    info->direction = IntersectionDirection::NONE;
    info->size = { 0, 0 };

    if (body->left() < _BlockingFrame->left()) {
        info->direction = IntersectionDirection::LEFT;
        info->size.width = _BlockingFrame->left() - body->left() + _BlockingFrame->width() - body->width();
    } else if (body->right() > _BlockingFrame->right()) {
        info->direction = IntersectionDirection::RIGHT;
        info->size.width = body->right() - _BlockingFrame->right() + _BlockingFrame->width() - body->width();
    }
}

Point Collider::resolveCollision(IntersectionInfo* info)
{
    Point result { 0, 0 };
    switch (info->direction) {
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
        break; // TODO: what's up with this
    }
    }
    return result;
}

bool Collider::checkOutside(RectangleCore* frame, bool& bIsOnTopOfPlatform)
{
    if ((_hostCore->bottom() < frame->top()) || //
        (_hostCore->top() > frame->bottom()) || //
        (_hostCore->right() < frame->left()) || //
        (_hostCore->left() > frame->right())) {
        bIsOnTopOfPlatform = false;
    }

    //? small owner intersect big size of platfrom at top
    if ( //
        (((frame->left() < _hostCore->left()) && (_hostCore->left() < frame->right())) //
            || //
            ((frame->left() < _hostCore->right()) && (_hostCore->right() < frame->right()))) //
        && //
        (_hostCore->bottom() > frame->top()) && (_hostCore->top() < frame->top()) //
        // &&																					 //
        // (_parentEngine->getLinearSpeed()._Vy <= 0)	  //* if we floating up don't pull host from bottom to top,
        // 											  //* allow host to float with it is own speed
    ) {
        _info->direction = IntersectionDirection::TOP;
        _info->size.height = -(_hostCore->bottom() - frame->top());

        bIsOnTopOfPlatform = true;
        return true;
    }
    //? small owner intersect big size of platfrom at bottom
    else if ( //
        (((frame->left() < _hostCore->left()) && (_hostCore->left() < frame->right())) //
            || //
            ((frame->left() < _hostCore->right()) && (_hostCore->right() < frame->right()))) //
        && //
        ((_hostCore->top() < frame->bottom()) && (_hostCore->bottom() > frame->bottom())) //
        && //
        !_bIsIgnoringBottomActor //* don't allow to collide when moving from bottom to top
    ) {
        _info->direction = IntersectionDirection::BOTTOM;
        _info->size.height = -(frame->bottom() - _hostCore->top());

        return true;
    }
    //? small owner intersect small size of platfrom at left
    else if ( //
        (((_hostCore->top() < frame->top()) && (frame->top() < _hostCore->bottom())) //
            || //
            ((_hostCore->top() < frame->bottom()) && (frame->bottom() < _hostCore->bottom()))) //
        && //
        ((_hostCore->left() < frame->right()) && (_hostCore->right() > frame->right())) //
        && //
        (_parentEngine->getLinearSpeed()._Vy > 0) //* if we floating up don't pull host from bottom to top,
                                                  //* allow host to float with it is own speed
                                                  // &&											 //
                                                  // !(_parentEngine->getIsJumping())			 //
    ) {
        _info->direction = IntersectionDirection::LEFT;
        _info->size.width = (frame->right() - _hostCore->left());

        return true;
    }
    //? small owner intersect small size of platfrom at right
    else if ( //
        (((_hostCore->top() < frame->top()) && (frame->top() < _hostCore->bottom())) //
            || //
            ((_hostCore->top() < frame->bottom()) && (frame->bottom() < _hostCore->bottom()))) //
        && //
        (_hostCore->right() > frame->left()) && (_hostCore->left() < frame->left()) //
        && //
        (_parentEngine->getLinearSpeed()._Vy > 0) //* if we floating up don't pull host from bottom to top,
                                                  //* allow host to float with it is own speed
                                                  // &&											 //
                                                  // !(_parentEngine->getIsJumping())			 //
    ) {
        _info->direction = IntersectionDirection::RIGHT;
        _info->size.width = (_hostCore->right() - frame->left());

        return true;
    } else {
        return false;
    }
}

bool Collider::checkOutsideBlockedActor(RectangleCore* killer, RectangleCore* frame, IntersectionInfo& info,
    IMechanics* engine)
{
    if ((killer->bottom() < frame->top()) || //
        (killer->top() > frame->bottom()) || //
        (killer->right() < frame->left()) || //
        (killer->left() > frame->right())) {
        return false; //? No fight
    }

    //? small owner intersect big size of platfrom at top
    if ( //
        ((frame->top() < killer->bottom()) && (killer->bottom() < frame->center().y)) //
        && //
        (engine->getLinearSpeed()._Vy > 0) //* help recognize bottom from top
    ) {
        info.direction = IntersectionDirection::TOP; //? Player wins
        return true; //? Got fight
    }
    //? small owner intersect big size of platfrom at bottom
    else if ( //
        (((frame->left() < killer->center().x) && (killer->center().x < frame->right())) //
            && //
            ((frame->center().y < killer->top()) && (killer->top() < frame->bottom()))) //

    ) {
        info.direction = IntersectionDirection::BOTTOM; //? Player loose
        return true; //? Got fight
    }
    //? small owner intersect small size of platfrom at left
    else if ( //
        (((killer->top() < frame->top()) && (frame->top() < killer->bottom())) //
            || //
            ((killer->top() < frame->bottom()) && (frame->bottom() < killer->bottom()))) //
        && //
        ((killer->left() < frame->right()) && (killer->right() > frame->right())) //
    ) {
        info.direction = IntersectionDirection::LEFT; //? Player loose
        return true; //? Got fight
    }
    //? small owner intersect small size of platfrom at right
    else if ( //
        (((killer->top() < frame->top()) && (frame->top() < killer->bottom())) //
            || //
            ((killer->top() < frame->bottom()) && (frame->bottom() < killer->bottom()))) //
        && //
        (killer->right() > frame->left()) && (killer->left() < frame->left()) //
    ) {
        info.direction = IntersectionDirection::RIGHT; //? Player loose
        return true; //? Got fight
    }
    // TODO: This is probably redudant
    else {
        return false; //? No fight
    }
}

void Collider::checkOutsideAllBlocked()
{
    for (auto& Actor : _BlockingActors) {
        if (checkOutside(Actor.first, Actor.second)) {
            onMoveCallBack();
        }
    }
    //? Not a single collision
}

bool Collider::doWeJump(RectangleCore* frame, IMechanics* player_engine)
{
    if ( //
        (_hostCore->left() < frame->left()) && (player_engine->getLinearSpeed()._Vx < 0) //
    ) {
        return true;
    } else if ( //
        (_hostCore->right() > frame->right()) && (player_engine->getLinearSpeed()._Vx > 0) //

    ) {
        return true;
    }

    return false;
}

void Collider::checkOnTopOfAnyPlatform()
{
    size_t Counter {};

    for (auto& Actor : _BlockingActors) {
        if (Actor.second) {
            onMessagingOnPlatform(true);

            if (!_parentEngine->getIsJumping() && !_parentEngine->getIsFalling() && _CallbackWorld) {
                _CallbackWorld->onTopOfPlatformMessaging(Counter);
            }
            // else if ((_parentEngine->getIsJumping() || _parentEngine->getIsFalling()) && _CallbackWorld)
            // {
            // 	_CallbackWorld->onFlyingMessaging();
            // }

            if (!_parentEngine->getIsJumping() && (doWeJump(Actor.first, _parentEngine))) {
                _parentEngine->setIsJumping(true);
            }
            return; //? Don't forget to exit cycle, cause value will be erased after it
        }
        Counter++;
    }
    onMessagingOnPlatform(false);
}

void Collider::checkFightWithEnemies()
{
    for (auto iterator { _Enemies.begin() }; iterator < _Enemies.end(); iterator++) {
        IntersectionInfo info;
        if (checkOutsideBlockedActor(_hostCore, *iterator, info, _parentEngine)) {
            switch (info.direction) {
            case IntersectionDirection::TOP: {
                //? Kill enemy
                onEnemyKilled(iterator - _Enemies.begin()); //? Sending signal to WorldDynamic
                _Enemies.erase(iterator);
                return; //? No need to look for another enemies`
                break;
            }
            case IntersectionDirection::BOTTOM:
            case IntersectionDirection::LEFT:
            case IntersectionDirection::RIGHT: {
                //? Kill player
                OnLevelStop(); //? Immidiately finish level
                return; //? No need to look for another enemies`
                break;
            }
            default: {
                break; // TODO: what's up with this?
            }
            }
        }
    }
    //? Not a single fight
}

void Collider::checkFallingToWormhole()
{
    for (auto iterator { _Wormholes.begin() }; iterator < _Wormholes.end(); iterator++) {
        IntersectionInfo info;
        if (checkOutsideBlockedActor(_hostCore, *iterator, info, _parentEngine)) {
            switch (info.direction) {
            //? In any case level restarts
            case IntersectionDirection::TOP:
            case IntersectionDirection::BOTTOM:
            case IntersectionDirection::LEFT:
            case IntersectionDirection::RIGHT: {
                //? Kill player
                OnLevelStop(); //? Immidiately finish level
                return; //? No need to look for another enemies`
                break;
            }
            default: {
                break; // TODO: what's up with this?
            }
            }
        }
    }
    //? Not a single fall
}

bool Collider::checkAmmoAimEnemy(RectangleCore* killer, IMechanics* engine)
{
    for (auto iterator { _Enemies.begin() }; iterator < _Enemies.end(); iterator++) {
        IntersectionInfo info;
        if (checkOutsideBlockedActor(killer, *iterator, info, engine)) {
            switch (info.direction) {
            case IntersectionDirection::TOP:
            case IntersectionDirection::BOTTOM:
            case IntersectionDirection::LEFT:
            case IntersectionDirection::RIGHT: {
                //? Sending signal to WorldDynamic
                onEnemyKilled(iterator - _Enemies.begin());
                //? Kill enemy
                _Enemies.erase(iterator);
                return true; //? No need to look for another enemies
                break;
            }
            default: {
                return false;
                break; // TODO: what's up with this?
            }
            }
        }
    }

    return false; //? Not a single fight
}

void Collider::onMoveCallBack()
{
    if (_parentEngine) {
        _parentEngine->move(resolveCollision(_info.get()));
    }
}

void Collider::onMessagingOnFrame(bool new_state)
{
    if (_parentEngine) {
        _parentEngine->setOnTopOfFrame(new_state);
    }
}

void Collider::onMessagingOnPlatform(bool new_state)
{
    if (_parentEngine) {
        _parentEngine->setOnTopOfAnyPlatform(new_state);
    }
}

void Collider::OnLevelStop()
{
    if (_LevelInstance) {
        _LevelInstance->onLevelStop();
    }
}

void Collider::onEnemyKilled(size_t index)
{
    if (_LevelInstance) {
        _LevelInstance->onEnemyKilled(index);
    }
}

void Collider::onAmmoDestroyed(size_t index)
{
    if (_LevelInstance) {
        _LevelInstance->onAmmoDestroyed(index);
    }
}
