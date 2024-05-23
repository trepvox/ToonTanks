// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "ToonTanksGameMode.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("MaxHealth : %f"), MaxHealth);
	Health = MaxHealth;
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

	//GetOwner returns an AActor pointer
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
	
	//borrowed from the ComponentHit in Projectile.cpp

	//gets game mode base
	ToonTanksGameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::DamageTaken(AActor *DamagedActor, float Damage, const UDamageType *DamageType, class AController *Instigator, AActor *DamageCauser)
{
		UE_LOG(LogTemp, Warning, TEXT("**DamagedActor: %s"), *DamagedActor->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), Damage);
		UE_LOG(LogTemp, Warning, TEXT("DamageType: %s"), *DamageType->GetName());
		UE_LOG(LogTemp, Warning, TEXT("Damage Instigator: %s"), *Instigator->GetName());
		UE_LOG(LogTemp, Warning, TEXT("DamageCauser: %s"), *DamageCauser->GetName());
	if (Damage <= 0.f) //or can put the return here
	{
		UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

	//-= will subtract the damage from your current Health
	Health -= Damage;
	UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

	if (Health <= 0.f && ToonTanksGameMode)
	{
		ToonTanksGameMode->ActorDied(DamagedActor);
	}
}