// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //creating something to detect the range of the tank from the tower
    /** if (Tank)
    {
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
        if (Distance <= FireRange)
        {
            RotateTurret(Tank->GetActorLocation());
        }
    }**/
    if (InFireRange())
    {
        RotateTurret(Tank->GetActorLocation());
    }
    
}

void ATower::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}

void ATower::BeginPlay()
{
    Super::BeginPlay();

    //note worldcontextobject is this and the player index is zero for player one
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    //need to cast to gain access to the tank

    //Start the timer for fire condition right in begin play
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
                                    //TimerHandle,      world,  callback function and address of, float for rate, true to keep checking

}

void ATower::CheckFireCondition()
{
    if (Tank == nullptr)
    {
        return;
    }
    if (InFireRange() && Tank->bAlive)
    {
        Fire();
    }
} 

bool ATower::InFireRange()
{
    if (Tank->bAlive)
    {
        //UE_LOG(LogTemp, Warning, TEXT("Start of Fire Range"));
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
        if (Distance <= FireRange)
        {
            //UE_LOG(LogTemp, Warning, TEXT("if loop for InFireRange"));
            return true;
        }
    }

    return false;
}