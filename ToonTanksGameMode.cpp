// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::ActorDied(AActor *DeadActor)
{
    if (DeadActor == Tank)
    {
        Tank->HandleDestruction();
        if (ToonTanksPlayerController)
        {
            //DisableInput requires an APlayerController
            //Tank->DisableInput(Tank->GetTankPlayerController());
            //disables mouse cursor upon death
            //Tank->GetTankPlayerController()->bShowMouseCursor = false;

            ToonTanksPlayerController->SetPlayerEnableState(false);
        }
        GameOver(false);
    }
    //need to check and include tower class

    else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
    {
        DestroyedTower->HandleDestruction();
        //this will subtract one from the number array for that level of towers
        --TargetTowers;
        //if zero towers remain then game ends
        if (TargetTowers == 0)
        {
            //ends the game
            GameOver(true);
        }
    }
}

void AToonTanksGameMode::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
    TargetTowers = GetTargetTowerCount();
    //"Kismet/GameplayStatics.h" needed for gameplaystatics
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
                                    //this tank and player 0
                //This gives us a pointer to the tank pawn in the form of ATank pointer

    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if (ToonTanksPlayerController)
    {
        //This will disable the player controller for being able to move at the start
        ToonTanksPlayerController->SetPlayerEnableState(false);

        FTimerHandle PlayerEnableTimerHandle;
        //FTimerDelegate InputDelegate = FTimerDelegate::CreateUObject(           UserObject,               Callback,                                          Inputs);
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerController, &AToonTanksPlayerController::SetPlayerEnableState, true);
        //GetWorldTimerManager().SetTimer(TimerHandle,           TimerDelegate,             PlayRate,   looping);
        GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, PlayerEnableTimerDelegate, StartDelay, false);
    }
}

int32 AToonTanksGameMode::GetTargetTowerCount()
{
    //UGameplayStatics::GetAllActorsOfClass(WorldContextObject, ActorClass, OutActors)
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    //returns the number of towers within that level
    return Towers.Num();
}