#include "PickUps/AmmoMachineGun.h"
#include "Kismet/GameplayStatics.h"
#include "../GameUICharacter.h"
#include "../GameUIPlayerController.h"

AAmmoMachineGun::AAmmoMachineGun()
{    
    // Load the mesh/ set struct data / bind function
    static ConstructorHelpers::FObjectFinder<UStaticMesh> AmmoMachineGunObj(TEXT("/Game/Meshes/SM_AmmoMachineGun"));

    if (AmmoMachineGunObj.Succeeded())
    {
        PickUpMesh->SetStaticMesh(AmmoMachineGunObj.Object);
        PickUpStruct.PickUpAmount = 12;
        PickUpStruct.PickUpName = "AmmoMachineGun";
        PickUpStruct.PickUpType = EPickUpType::EAmmoMachineGun;
    }

    BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AAmmoMachineGun::OnOverlapBegin);
}

// If it is the player/ set the weapon ammo / update weapon widget ammo text/ destroy
void AAmmoMachineGun::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        if (OtherActor->ActorHasTag("Player"))
        {
            AGameUICharacter* GameUICharacter = Cast<AGameUICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
            AGameUIPlayerController* GameUIPlayerController = Cast<AGameUIPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

            for (int i = 0; i < GameUICharacter->Weapons.Num(); i++)
            {
                if (GameUICharacter->Weapons[i]->WeaponStruct.WeaponType == EWeaponType::EMachineGun)
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
                        break;
                    }
                }
            }
        }
    }
}
