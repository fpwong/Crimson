// Fill out your copyright notice in the Description page of Project Settings.

#include "Crimson.h"
#include "PowerUp.h"
#include "CrimsonPawn.h"


// Sets default values
APowerUp::APowerUp()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Assets/Meshes/1M_Cube.1M_Cube"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(Root);
	Mesh->SetStaticMesh(MeshAsset.Object);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	Collision->SetupAttachment(Mesh);
	Collision->SetCollisionProfileName("OverlapAll");

	bReplicates = true;
}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float Time = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	UE_LOG(LogTemp, Warning, TEXT("Time: %f"), Time);
	Mesh->AddLocalOffset(FVector(0, 0, FMath::Sin(Time) * 100));
}

void APowerUp::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (ACrimsonPawn* Character = Cast<ACrimsonPawn>(OtherActor))
	{
		Server_Activate(Character);
		Destroy();
	}
}

void APowerUp::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Mesh->SetRelativeLocation(FVector(0, 0, Mesh->Bounds.BoxExtent.Z));

	Collision->SetBoxExtent(Mesh->Bounds.BoxExtent);
}

void APowerUp::Multicast_PlayParticleSystem_Implementation()
{
	if (UWorld* World = GetWorld())
	{
		UParticleSystemComponent* ParticleSystem = UGameplayStatics::SpawnEmitterAtLocation(World, ParticleSystemClass, GetActorTransform());
		
		if (ParticleSystem) ParticleSystem->SetColorParameter(FName("Color"), ParticleColor);
	}
}

void APowerUp::Server_Activate_Implementation(ACrimsonPawn* Character)
{
	Multicast_PlayParticleSystem();
	Activate(Character);
}

bool APowerUp::Server_Activate_Validate(ACrimsonPawn* Character)
{
	return true;
}

void APowerUp::Activate_Implementation(ACrimsonPawn* Character)
{
	Destroy();
}