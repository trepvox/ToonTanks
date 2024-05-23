// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraShakeBase.h"
#include "Projectile.h"


// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}


void ABasePawn::HandleDestruction()
{
	// TODO:Needs visual and audio effects
	//Visual particle effects
	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}

	if (DeathSound)
	{
							   //(World Context object, sound,    location)
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}

	if (DeathCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
}


void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	//rememeber we only want the yaw or z axis to move
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	//RInterpTo is to stop the jerky rotation of the turret when doing a 180 turn
	TurretMesh->SetWorldRotation(//LookAtRotation);
		FMath::RInterpTo(
		TurretMesh->GetComponentRotation(), 
		LookAtRotation, 
		UGameplayStatics::GetWorldDeltaSeconds(this), 
		5.f
		)
	); 
}

void ABasePawn::Fire()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();

	/**DrawDebugSphere(
		//world needed
		GetWorld(),
		//getting center of sphere
		Location,
		//size radius
		25.f,
		//Segments
		12,
		//color
		FColor::Red,
		//drawing persistent lines
		false,
		3.f); **/

	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();	

	//auto assigns type to the new variable automatically from spawnactor but is bad practice for unreal
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation);
	//to set the projectiles owner as the thing that spawned it 
	if (Projectile)
	{
		Projectile->SetOwner(this);
	}
	/**else 
	{
		UE_LOG(LogTemp, Warning, TEXT("$*Broken Projectile: %p"), *GetName());
	}**/
}
