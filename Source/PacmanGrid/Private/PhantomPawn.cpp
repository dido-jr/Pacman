// Fill out your copyright notice in the Description page of Project Settings.


#include "PhantomPawn.h"
#include "PacmanPawn.h"
#include "Blinky.h"
#include "Clyde.h"
#include "time.h"
#include "Kismet/GameplayStatics.h"

APhantomPawn::APhantomPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentMovementSpeed = 370.f;
	BlueMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlueMesh"));
	BlueMesh->SetupAttachment(Collider);
	BlueMesh->SetVisibility(false);
	WhiteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WhiteMesh"));
	WhiteMesh->SetupAttachment(Collider);
	WhiteMesh->SetVisibility(false);
	EatenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EatenMesh"));
	EatenMesh->SetupAttachment(Collider);
	EatenMesh->SetVisibility(false);
}

void APhantomPawn::BeginPlay()
{
	Super::BeginPlay();
	FVector2D StartNode = TheGridGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = TheGridGen->TileMap[StartNode];
	Player = Cast<APacmanPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacmanPawn::StaticClass()));
	Blinky = Cast<ABlinky>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlinky::StaticClass()));
	Clyde = Cast<AClyde>(UGameplayStatics::GetActorOfClass(GetWorld(), AClyde::StaticClass()));
	GetWorldTimerManager().SetTimer(Timer, this, &APhantomPawn::SetChaseState, 7.0f, false);
}

void APhantomPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	const auto PN = Cast<APacmanPawn>(OtherActor);
	if (PN)
	{
		if (PhantomState == Frightened)
		{
			this->Eat();
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("I Reached you..")));
		}
		else
		{
			PN->LifeCounter();//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//PN->Destroy();//ferma il gioco quando pacman vien mangiato
		}
	}

}

void APhantomPawn::Eat()
{
	BlueMesh->SetVisibility(false);
	WhiteMesh->SetVisibility(false);
	EatenMesh->SetVisibility(true);
	eaten = true;
}

void APhantomPawn::OnNodeReached()
{
	Super::OnNodeReached();
}

APacmanPawn* APhantomPawn::GetPlayer() const
{
	return Player;
}

APhantomPawn* APhantomPawn::GetBlinky() const
{
	return Blinky;
}

APhantomPawn* APhantomPawn::GetClyde() const
{
	return Clyde;
}

void APhantomPawn::SetSpeed(float Speed)
{
	CurrentMovementSpeed = Speed;
}

AGridBaseNode* APhantomPawn::GetPlayerRelativeTarget()
{
	return Player->GetLastNode();
}

void APhantomPawn::SetChaseState()
{
	GetWorldTimerManager().ClearTimer(Timer);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Chase")));
	PhantomState = Chase;
	if (ScatterRound < 4)
	{
		GetWorldTimerManager().SetTimer(Timer, this, &APhantomPawn::SetScatterState, 20.0f, false);
	}
}

void APhantomPawn::SetScatterState()
{
	GetWorldTimerManager().ClearTimer(Timer);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Scatter")));
		PhantomState = Scatter;
		ScatterRound += 1;
		if (ScatterRound < 3)
		{
			GetWorldTimerManager().SetTimer(Timer, this, &APhantomPawn::SetChaseState, 7.0f, false);
		}
		else
		{
			GetWorldTimerManager().SetTimer(Timer, this, &APhantomPawn::SetChaseState, 5.0f, false);
		}
}

void APhantomPawn::SetFrightenedState()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Frightened")));
	PhantomState = Frightened;
	InvertDirection();
	SetBlueMesh();
}

void APhantomPawn::SetBlueMesh()
{
	if (flashes < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("BlueMesh")));
		BlueMesh->SetVisibility(true);
		GetWorldTimerManager().SetTimer(MeshTimer, this, &APhantomPawn::SetWhiteMesh, 6.0, false);
		flashes += 1;
	}
	else
	{
		WhiteMesh->SetVisibility(false);
		BlueMesh->SetVisibility(true);
		GetWorldTimerManager().SetTimer(MeshTimer, this, &APhantomPawn::SetWhiteMesh, 0.5f, false);
	}
}


void APhantomPawn::SetWhiteMesh()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("WhiteMesh")));
	if (flashes < 7)
	{
		BlueMesh->SetVisibility(false);
		WhiteMesh->SetVisibility(true);
		flashes += 1;
		GetWorldTimerManager().SetTimer(MeshTimer, this, &APhantomPawn::SetBlueMesh, 0.5f, false);
	}
	else
	{
		flashes = 0;
		BlueMesh->SetVisibility(false);
		WhiteMesh->SetVisibility(false);
	}
}

void APhantomPawn::InvertDirection()
{
	if (GetLastValidDirection() == FVector(1, 0, 0))
	{
		FVector new_direction = FVector(-1, 0, 0);
		SetLastValidDirection(new_direction);
	}
	else if (GetLastValidDirection() == FVector(-1, 0, 0))
	{
		FVector new_direction = FVector(1, 0, 0);
		SetLastValidDirection(new_direction);
	}
	else if (GetLastValidDirection() == FVector(0, 1, 0))
	{
		FVector new_direction = FVector(0, -1, 0);
		SetLastValidDirection(new_direction);
	}
	else if (GetLastValidDirection() == FVector(0, -1, 0))
	{
		FVector new_direction = FVector(0, 1, 0);
		SetLastValidDirection(new_direction);
	}
}