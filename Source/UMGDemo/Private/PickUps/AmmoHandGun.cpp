#include "PickUps/AmmoHandGun.h"
#include "Kismet/GameplayStatics.h"
#include "../GameUICharacter.h"
#include "../GameUIPlayerController.h"

AAmmoHandGun::AAmmoHandGun()
{
    // Load the mesh/ set struct data / bind function
    static ConstructorHelpers::FObjectFinder<UStaticMesh> AmmoHandGunObj(TEXT("/Game/Meshes/SM_AmmoHandGun"));

    if (AmmoHandGunObj.Succeeded())
    {
        PickUpMesh->SetStaticMesh(AmmoHandGunObj.Object);
        PickUpStruct.PickUpAmount = 6;
        PickUpStruct.PickUpName = "AmmoHandGun";
        PickUpStruct.PickUpType = EPickUpType::EAmmoHandGun;
    }
    
    BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AAmmoHandGun::OnOverlapBegin);
}

// If it is the player/ set the weapon ammo / update weapon widget ammo text/ destroy
void AAmmoHandGun::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        if (OtherActor->ActorHasTag("Player"))
        {
           AGameUICharacter* GameUICharacter = Cast<AGameUICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
           AGameUIPlayerController* GameUIPlayerController = Cast<AGameUIPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

           for (int i = 0; i < GameUICharacter->Weapons.Num(); i++)
           {
               if (GameUICharacter->Weapons[i]->WeaponStruct.WeaponType == EWeaponType::EHandGun)
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