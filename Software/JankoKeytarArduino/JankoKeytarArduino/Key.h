#pragma once

#include "arduino.h"
#include "MathUtils.h"
#include "MidiUtils.h"
#include "StateMachine.h"

struct KeySettings
{
public:
	unsigned long period;

	int OnThreshold;

	unsigned long VelocityTimeMin;
	unsigned long VelocityTimeMax;
	int VelocityThresholdA;
	int VelocityThresholdB;

	int ThresholdAftertouchA;
	int ThresholdAftertouchB;
};

class Key : public StateMachine
{
private:
	KeySettings* settings;

	int pin;
	int pitch;
	int force;

	class KeyStateOff : public State
	{
	private:
		Key* key = 0;


	public:
		KeyStateOff(Key* key)
		{
			this->key = key;
		}

		void Enter()
		{
			Serial.println("Entering KeyStateOff");
		}
		void Exit() {}

		void Update()
		{
			if (analogRead(key->pin) > key->settings->OnThreshold)
				key->ChangeState(&key->stateVelocity);
		}
	} stateOff = 0;
	class KeyStateVelocity : public State
	{
	private:
		Key* key = 0;
		unsigned long timestamp;

	public:
		KeyStateVelocity(Key* key)
		{
			this->key = key;
		}

		void Enter()
		{
			Serial.println("Entering KeyStateVelocity");
			timestamp = micros();
		}
		void Exit() {}
		void Update()
		{
			int input = analogRead(key->pin);
			if (input < key->settings->VelocityThresholdA)
				key->ChangeState(&key->stateOff);
			else if (input > key->settings->VelocityThresholdB)
			{
				unsigned long timestampB = micros();
				unsigned long delta = micros() - timestamp;
				Serial.print(timestampB);
				Serial.print(" - ");
				Serial.print(timestamp);
				Serial.print(" = ");
				Serial.println(delta);
				
				int velocity = mapClamped(delta, key->settings->VelocityTimeMin, key->settings->VelocityTimeMax, 127, 0);
				velocity *= velocity;
				velocity /= 127;
				Serial.print("Velocity: ");
				Serial.println(velocity);

				SendMidi(NoteOn(0, key->pitch, velocity));
				key->ChangeState(&key->stateAftertouch);
			}
		}
	} stateVelocity = 0;
	class KeyStateAftertouch : public State
	{
	private:
		Key* key = 0;
		int frame;
		static const int framemod = 1500;

	public:
		KeyStateAftertouch(Key* key)
		{
			this->key = key;
		}

		void Enter()
		{
			Serial.println("Entering KeyStateAftertouch");
		}
		void Exit() {}
		void Update()
		{
			int input = analogRead(key->pin);
			if (input < key->settings->VelocityThresholdA)
			{
				SendMidi(NoteOff(0, key->pitch, 0));
				key->ChangeState(&key->stateOff);
			}
			else if (input > key->settings->ThresholdAftertouchA)
			{
				int aftertouch = mapClamped(input, key->settings->ThresholdAftertouchA, key->settings->ThresholdAftertouchB, 0, 127);

				if (frame++ % framemod == 0)
				{
					Serial.print("Aftertouch: ");
					Serial.println(aftertouch);
				}

				SendMidi(NoteAftertouch(0, key->pitch, aftertouch));
			}
				
		}
	} stateAftertouch = 0;

public:
	Key(KeySettings* settings, int pin, int pitch)
	{
		this->settings = settings;
		this->pin = pin;
		this->pitch = pitch;

		stateOff = KeyStateOff(this);
		stateVelocity = KeyStateVelocity(this);
		stateAftertouch = KeyStateAftertouch(this);

		ChangeState(&stateOff);
	};
};