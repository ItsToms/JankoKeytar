#pragma once

#include "arduino.h"
#include <MIDIUSB.h>

void SendMidi(midiEventPacket_t midi)
{
	MidiUSB.sendMIDI(midi);
	MidiUSB.flush();
}

midiEventPacket_t NoteOn(byte channel, byte pitch, byte velocity)
{
	return midiEventPacket_t{ 0x09, 0x90 | channel, pitch, velocity };
}

midiEventPacket_t NoteOff(byte channel, byte pitch, byte velocity)
{
	return midiEventPacket_t{ 0x08, 0x80 | channel, pitch, velocity };
}

midiEventPacket_t NoteAftertouch(byte channel, byte pitch, byte pressure)
{
	return midiEventPacket_t{ 0x0A, 0xA0 | channel, pitch, pressure };
}

midiEventPacket_t ControlChange(byte channel, byte control, byte value)
{
	return midiEventPacket_t{ 0x0B, 0xB0 | channel, control, value };
}