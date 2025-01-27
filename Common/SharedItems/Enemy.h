#pragma once
#include "Actor.h"
class Enemy : public Actor
{
public:
	enum AlarmState
	{
		PATROLLING,
		ALLERT
	};
	Enemy(const std::string& path);
	void Init();
	void StopMoving();
	void ContinueMoving();
	void SetupPatrolling();

	void Update();
	AlarmState GetState() const;
	void SetState(int value);
	void MoveInput();
	void SetIsShooting(bool value);
	bool GetIsShooting() const;
private:
	AlarmState alarmState;
	bool isShooting;
	bool canSaveDir;
	
};

