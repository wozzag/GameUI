#include "PickUps/AmmoSniperRifle.h"
#include "Kismet/GameplayStatics.h"
#include "../GameUICharacter.h"
#include "../GameUIPlayerController.h"

AAmmoSniperRifle::AAmmoSniperRifle()
{
    // Load the mesh/ set struct data / bind function
    static ConstructorHelpers::FObjectFinder<UStaticMesh> AmmoSniperRifleObj(TEXT("/Game/Meshes/SM_AmmoSniperRifle"));

    if (AmmoSniperRifleObj.Succeeded())
    {
        PickUpMesh->SetStaticMesh(AmmoSniperRifleObj.Object);
        PickUpStruct.PickUpAmount = 2;
        PickUpStruct.PickUpName = "SniperRifle";
        PickUpStruct.PickUpType = EPickUpType::EAmmoSniperRifle;
    }

    BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AAmmoSniperRifle::OnOverlapBegin);
}

// If it is the player/ set the weapon ammo / update weapon widget ammo text/ destroy
void AAmmoSniperRifle::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        if (OtherActor->ActorHasTag("Player"))
        {
            AGameUICharacter* GameUICharacter = Cast<AGameUICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
            AGameUIPlayerController* GameUIPlayerController = Cast<AGameUIPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

            for (int i = 0; i < GameUICharacter->Weapons.Num(); i++)
            {
                if (GameUICharacter->Weapons[i]->WeaponStruct.WeaponType == EWeaponType::ESniperRifle)
                {
                    if (GameUICharacter->Weapons[i]->WeaponStruct.GetCurrentAmmo() < GameUICharacter->Weapons[i]->WeaponStruct.GetMaxAmmo())
                    {
                        GameUICharacter->Weapons[i]->WeaponStruct.SetCurrentAmmo(GameUICharacter->Weapons[i]->WeaponStruct.GetCurrentAmmo() + this->PickUpStruct.PickUpAmount);
                        FText AmmoText = FText::AsNumber(GameUICharacter->Weapons[i]->WeaponStruct.GetCurrentAmmo());

                        GameUIPlayerController->InGameHUDInstance->WeaponWidgets[i]->TxtCurrentAmmo->SetText(AmmoText);
                        GameUIPlayerController->InGameHUDInstance->WeaponWidgets[i]->PlayAnimationByName("AnimCurrentAmmo",
                            0.0f,
                            1,
                            EUMGSequencePlayMode::PingPong,
                            1.0f);
                        Destroy();
                        UE_LOG(LogTemp, Warning, TEXT("Ammo Sniper Rifle"));
                        break;
                    }
                }
                UE_LOG(LogTemp, Warning, TEXT("Ammo Sniper Rifle LOOOOOOP"));
            }
        }
    }
}