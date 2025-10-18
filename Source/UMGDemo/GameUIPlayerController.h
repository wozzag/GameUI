// This class adds the UMG widgets
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/InGameHUD.h"
#include "Widgets/MainMenu.h"
#include "GameUIPlayerController.generated.h"

UCLASS()
class GAMEUI_API AGameUIPlayerController : public APlayerController
{
	GENERATED_BODY()

	AGameUIPlayerController();
	
public:

	// Main menu widget class and instance
	UPROPERTY()
	TSubclassOf<UMainMenu> MainMenuClass;

	UPROPERTY()
	UMainMenu* MainMenuInstance;

	// PlayerHUD widget class and instance
	UPROPERTY()
	TSubclassOf<UInGameHUD> InGameHUDClass;

	UPROPERTY()
	UInGameHUD* InGameHUDInstance;

public:
	virtual void BeginPlay() override;

	// Create widgets and set data
	void CreateMainMenuWidget();

	void CreateInGameWidget();

};
