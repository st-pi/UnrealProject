// Shoot Them Up Game, All Rights Reserved


#include "Sound/STUSoundFunctionLibrary.h"
#include "Sound/SoundClass.h"


void USTUSoundFunctionLibrary::SetSoundClassVolume(USoundClass* SoundClass, float Volume)
{
	if (SoundClass)
	{
		SoundClass->Properties.Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
	}
}
void USTUSoundFunctionLibrary::ToggleSoundClassVolume(USoundClass* SoundClass)
{
	if (SoundClass)
	{
		SetSoundClassVolume(SoundClass, SoundClass->Properties.Volume > 0.0f ? 0.0f : 1.0f);
	}
}