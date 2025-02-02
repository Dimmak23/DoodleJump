# A Classic Doodle Jump 2D Clone

## Briefly about code base

### What code base is?

Doodle Jump game made in C++ with help of SDL2 libraries.
SDL2 graphic features were putted in a shell - connector.
Basicaly connector hides SDL2 features to allow to be independent from changes in SDL libraries.
Meaning any change in SDL2 or going from SDL2 to SDL3 requires only editing in the connector,
not in the game itself.

There is three main parts:

- Sprite: it holds image data, such as SDL_Texture and destination area rectangle.
- Render: it operates with Sprites. Render's API should be called to create, draw, destroy any Sprite, also to operate
  with Sprite size changes; Render holds SDL_Window* and SDL_Renderer* as static variable,
  cause game needs only one instance of those.
- Launcher: it runs game application. Just pass game application that implements IApplication interface, Launcher
  could run it for you.

### Project architecture

Since it's two main parts, but only one (top level with game itself) needs to be constantly re-configured and rebuilded - it's convinient to split project in two parts.

After code base is cloned to machine you need to build connector once, after that point you done here.

Any changes in game itself requires only to rebuild option (look futher for '--rebuild' flag).

<img src="documentation/pics/project_architecture.png" alt="" data-canonical-src="" width="800" height="" />

### Naming convention

Here is the typical style for header and source files of any library.

```cpp
// Lib.hpp

#pragma once

//* SDL2: Connector
// What we have to include from connector libraries

//* Game
// Game libraries headers

//* C++ std
// Any C++ sta header that need to be included directly in the header

//* Forward declarations

class AnyClass
{
public:
	//@ API's
	AnyClass();

	//@ Methods
	ReturnType MemberFunction(ParamType param_variable, bool bSomeFlag);

private:
	//* Let's prevent copying AnyClass object
	AnyClass(const AnyClass&) = delete;
	AnyClass& operator=(const AnyClass&) = delete;

	//@ Members
	Type _memberVariable;

	//? Treating booleas vaariables in some special way,
	//? by using same style as in the UnrealEngine convention
	bool _bInSomeState{false};
};

// Lib.cpp
AnyClass::AnyClass()
{
...
}

ReturnType AnyClass::MemberFunction(ParamType param_variable, bool bSomeFlag);
{
	ReturnType scope_variable{param_variable + _memberVariable};

	return bSomeFlag && _bInSomeState? scope_variable: ReturnType{};
}
```

### How to build executable?

To build SDL2_connector library and game executable you need to run only one script:

```bat
@REM batch script
scripts\win\build.bat --config CONFIG --generator GENERATOR
@REM or, same command:
scripts\win\build.bat --generator GENERATOR --config CONFIG
```

```bash
bash scripts/linux/build.sh --config CONFIG --generator GENERATOR
bash scripts/linux/build.sh --generator GENERATOR --config CONFIG
```

Specify configuration (Debug or Release) with generator (Ninja, MinGW Makefiles or Unix Makefiles or Mirosoft Visual Studio 17 2022 - however no such option for linux builts)
and script will delete all builds (vendor, connector and game) - if any existed, and configure and build everything for you.

This command is for doing same but generator or config don't specified - meaning would be used same
from previous built:

```bat
scripts\win\build.bat --config CONFIG
scripts\win\build.bat --generator GENERATOR
```

```bash
bash scripts/linux/build.sh --config CONFIG
bash scripts/linux/build.sh --generator GENERATOR
```

<img src="documentation/pics/config_option.png" alt="" data-canonical-src="" width="450" height="" />

<img src="documentation/pics/generator_option.png" alt="" data-canonical-src="" width="700" height="" />

Also when you about to change code base of game or connector,
and you don't need to re-configure and re-build vendor you can simply re-configure and re-build top level code:

```batch
scripts\win\build.bat --rebuild AIM
```

```bash
bash scripts/linux/build.sh --rebuild AIM
```

<img src="documentation/pics/rebuild_option.png" alt="" data-canonical-src="" width="350" height="" />

### How to run executable?

<!-- TODO -->

Build system create ./build and ./executables folders for you.
./executables folder is the destination for installing .exe and .dlls, also data will be copied here.

<!-- TODO -->

### About game:

Welcome! Start your gateway to endless jumping fun! This is a delightful and addictive arcade game that puts you in control of a bouncy character on a mission to reach new heights. Inspired by the timeless classic Doodle Jump, this game brings back the nostalgia of simple yet captivating gameplay.

#### Features:

1. Endless Jumping Excitement: Experience the thrill of endless jumping as you bounce from platform to platform in a vibrant and colorful world.
2. Simple Controls, Addictive Gameplay: With easy-to-learn controls, DoodleJump is accessible to players of all ages. Just tilt your device to guide your character left and right, and watch them leap higher and higher!
3. Challenging Obstacles: Test your reflexes and agility as you encounter various obstacles, enemies and wormholes while climbing. Dodge monsters, avoid traps, and collect power-ups to boost your journey.
4. Compete for High Scores: Challenge yourself to beat your own high score, or compete with friends and players from around the world on the global leaderboard. Can you climb to the top and become the ultimate DoodleJump champion?

#### Controls:

Doodle jump off platform automatically when player move it to the edge of the platform. To move use **LEFT\RIGHT keyboard arrow keys**. Doodle is able to take a long fly: press **UP keyboard arrow key**, Doodle will jump in 10 seconds, after that it would take 2 seconds to recharge fly ability.

_Don't stay too long on the platform: after 2 seconds it will disappear, but you still will be able to jump off from it._

There is enemies in the level. Jump on them to kill. If you go to the bottom, left, right side of the enemy - you will die. You also able to kill enemies by shooting at them ammo projectiles. Press **LEFT mouse button** to shoot. Ammo will be aim to the point where cursor when shoot.

If Doodle suddenly go below screen - he will die. If Doodle go thru right border of the screen it appears at the left, and vice versa.

There is also wormholes on the level that will suck Doodle into it, changes his trajectory.
When Doodle will stuck in the wormhole it will die.

After any death level reloads.

To close game press **Esc** key.

You will achive points from: distance you pass and platforms you fly.

![image](about/gifs/doodlejump_gameplay.gif)
