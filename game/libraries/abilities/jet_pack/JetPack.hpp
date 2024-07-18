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

	void initialize();
	void setPhysicsEngineConnection(IMechanics* parent_engine);
	void tick(float delta_t);
	//? Call with 'true' when button clicked
	//? and call with 'false' when button released
	void enable(bool new_state);
	void clear();

private:
	//@ Methods

	//* Let's prevent copying of the module
	JetPack(const JetPack&) = delete;
	JetPack& operator=(const JetPack&) = delete;

	//@ Members

	IMechanics* _hostEngine{ nullptr };

	//* Timers
	float _JumpTimer{ 0.f };
	const float _JumpDuration{ 10000.f };	 // ms
	float _LoadTimer{ 0.f };
	const float _LoadDuration{ 2000.f };	// ms

	//* States
	bool _bIsEnabled{ false };
	bool _bIsLoaded{ true };
	bool _bJumping{ false };
};
