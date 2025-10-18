#include "PickUps/Health.h"
#include "Kismet/GameplayStatics.h"
#include "../GameUICharacter.h"
#include "../GameUIPlayerController.h"

AHealth::AHealth()
{
    // Load the mesh/ set struct data / bind function
    static ConstructorHelpers::FObjectFinder<UStaticMesh> HealthObj(TEXT("/Game/Meshes/SM_Health"));

    if (HealthObj.Succeeded())
    {
        PickUpMesh->SetStaticMesh(HealthObj.Object);
        PickUpStruct.PickUpAmount = 20;
        PickUpStruct.PickUpName = "Health";
        PickUpStruct.PickUpType = EPickUpType::EHealth;
    }

    BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AHealth::OnOverlapBegin);
}

// If it is the player/ set health/ update health bar/ destroy
void AHealth::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        if (OtherActor->ActorHasTag("Player"))
        {
            AGameUICharacter* GameUICharacter = Cast<AGameUICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
            AGameUIPlayerController* GameUIPlayerController = Cast<AGameUIPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

           if (GameUICharacter->GetCurrentHealth() >= GameUICharacter->GetMaxHealth())
           {
                return;
           }
           else
           {
                int32 Health = GameUICharacter->GetCurrentHealth() + PickUpStruct.PickUpAmount;
                GameUICharacter->SetCurrentHealth(Health);

                GameUIPlayerController->InGameHUDInstance->WBP_HealthProgressBar->ProgressBar->SetPercent(float(GameUICharacter->GetCurrentHealth() / 100.0f));

                Destroy();
           }
        }
    }
}