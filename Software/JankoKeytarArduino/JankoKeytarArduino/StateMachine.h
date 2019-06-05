#pragma once

class State
{
public:
	virtual void Enter() = 0;
	virtual void Exit() = 0;

	virtual void Update() = 0;
};

class StateMachine
{
private:
	State* state;

public:
	void ChangeState(State* stateNew)
	{
		if (state) state->Exit();
		state = stateNew;
		if (state) state->Enter();
	}

	void UpdateState()
	{
		if (state)
			state->Update();
	}
};