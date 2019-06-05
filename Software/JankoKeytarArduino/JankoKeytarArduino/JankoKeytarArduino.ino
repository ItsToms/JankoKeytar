#include "Key.h"

KeySettings settings = KeySettings();
Key key = Key(&settings, 0, 48);

void setup()
{
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);

	settings.OnThreshold = 700;

	settings.VelocityTimeMin = 0;
	settings.VelocityTimeMax = 40000;
	settings.VelocityThresholdA = 660;
	settings.VelocityThresholdB = 720;

	settings.ThresholdAftertouchA = 880;
	settings.ThresholdAftertouchB = 980;
}

void loop()
{
	key.UpdateState();
}