#pragma once

//* Forward declarations
class IMechanics;

class JetPack
{
public:
	JetPack();
	~JetPack();

	//@ API: for level

	//* Manipulators

	void Init();
	void SetPhysicsEngineConnection(IMechanics* parent_engine);
	void Tick(float delta_t);
	//? Call with 'true' when button clicked
	//? and call with 'false' when button released
	void Enable(bool new_state);
	void Clear();

private:
	//@ Methods

	//* Let's prevent copying of the module
	JetPack(const JetPack&) = delete;
	JetPack& operator=(const JetPack&) = delete;

	//@ Members

	IMechanics* _hostEngine{ nullptr };

	//* Timers
	float _jumpTimer{ 0.f };
	const float _jumpDuration{ 10000.f };	 // ms
	float _loadTimer{ 0.f };
	const float _loadDuration{ 2000.f };	// ms

	//* States
	bool _bIsEnabled{ false };
	bool _bIsLoaded{ true };
	bool _bJumping{ false };
};
