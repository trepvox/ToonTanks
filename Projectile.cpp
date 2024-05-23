// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/PlayerController.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 1300.f;
	ProjectileMovementComponent->InitialSpeed = 1300.f;

	//requires #include "Particles/ParticleSystemComponent.h"
	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	TrailParticles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (FiringSound)
	{
						   //(World Context object, sound, location)
		UGameplayStatics::PlaySoundAtLocation(this, FiringSound, GetActorLocation());
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	/**UE_LOG(LogTemp, Warning, TEXT("OnHIT"));
	UE_LOG(LogTemp, Warning, TEXT("HitComp: %s"), *HitComp->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s"), *OtherActor->GetName());
	UE_LOG(LogTemp, Warning, TEXT("OtherComp: %s"), *OtherComp->GetName()); **/
	//UE_LOG(LogTemp, Warning, TEXT("On a hit, Other Actor %s, OtherComp: %s"), *OtherActor->GetName(), *OtherComp->GetName());

	if (HitSound)
	{
						   //(World Context object, sound,    location)
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
	AActor* MyOwner = GetOwner();
					//nullptr is null pointer
	if (MyOwner == nullptr) 
	{
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("MyOwner: %s returning null"), *MyOwner->GetName());
		return;
	}

	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Inside the if for damaging"));
		//this will generate the damage event.
		//UE_LOG(LogTemp, Warning, TEXT("--If damage OtherActor: %s"), *OtherActor->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), Damage);
		//UE_LOG(LogTemp, Warning, TEXT("MyOwnerInstigator: %s"), *MyOwnerInstigator->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("Event Instigator: %s"), *this->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("DamageTypeClass: %s"), *DamageTypeClass->GetName());

		//ApplyDamage(AActor* DamagedActor, float BaseDamage, AController* EventInstigator, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass)
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		//The destroy function will delete the projectile after contact with something.

		if (HitParticles)
		{
			//                      (world context object, particle system, location,        rotation)
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
		}

		if (HitCameraShakeClass)
		{
			//Calling get world to call first player controller to call client camera shake, and ignore everything insid and just call for HitCameraShakeClass
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
		}
	}
	//Moved projectile outside of the if check to remove it no matter what at the end of it's shot cycle
	Destroy();
}