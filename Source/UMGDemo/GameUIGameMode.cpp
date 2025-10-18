#include "GameUIGameMode.h"
#include "GameUICharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameUIPlayerController.h"

AGameUIGameMode::AGameUIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_GameUICharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// Set the default player controller class
	PlayerControllerClass = AGameUIPlayerController::StaticClass();
}
