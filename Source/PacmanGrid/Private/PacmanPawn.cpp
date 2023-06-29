// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanPawn.h"
#include "PowerNode.h"
#include "TestGridGameMode.h"
#include "TimerManager.h"

APacmanPawn::APacmanPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	//// vettore di direzione inizializzato con zero (il pawn non si muove allo start del gioco fino a che non
	//   viene premuto uno dei tasti W-A-S-D )
	LastInputDirection = FVector(0, 0, 0);
	LastValidInputDirection = FVector(0, 0, 0);
	////posizione iniziale  del pawn nelle coordinate di griglia (1,1)
	CurrentGridCoords = FVector2D(1, 1);
}

void APacmanPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APacmanPawn::BeginPlay()
{
	Super::BeginPlay();
	//// posizione iniziale del pawn
	FVector2D StartNode = TheGridGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = TheGridGen->TileMap[StartNode];
	CurrentMovementSpeed = 400.f;
	score = 0;

}

void APacmanPawn::SetVerticalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorForwardVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir.GetSafeNormal();
	SetNextNodeByDir(LastInputDirection);
}

void APacmanPawn::SetHorizontalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorRightVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir;
	SetNextNodeByDir(LastInputDirection);
}

void APacmanPawn::OnClick()
{
	FHitResult Hit = FHitResult(ForceInit);
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	if (AGridBaseNode* CurrTile = Cast<AGridBaseNode>(Hit.GetActor()))
	{
		FVector2D CurrCoords = CurrTile->GetGridPosition();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Position x=%f  Y=%f "), CurrCoords.X, CurrCoords.Y));
	}
}

void APacmanPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Bind Actions here
	InputComponent->BindAxis(TEXT("MoveForward"), this, &APacmanPawn::SetVerticalInput);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &APacmanPawn::SetHorizontalInput);
	// bind the OnClick function to InputComponent for test purpose
	InputComponent->BindAction("Click", IE_Pressed, this, &APacmanPawn::OnClick);
}



void APacmanPawn::HandleMovement()
{
	Super::HandleMovement();
	// si muove autonomamente fino a che il next node è walkable
	if (!TargetNode && !NextNode)
	{
		if (TheGridGen->IsNodeValidForWalk(TheGridGen->GetNextNode(CurrentGridCoords, LastInputDirection)))
		{
			SetLastValidDirection(LastInputDirection);
		}
		SetNodeGeneric(LastValidInputDirection);
	}
}


void APacmanPawn::SetTargetNode(AGridBaseNode* Node)
{
	Super::SetTargetNode(Node);
}

void APacmanPawn::LifeCounter()
{
	life -= 1;
	if (life == 0)
	{
		Destroy();
	}
	else
	{
		PacmanSpawn();
	}
}

void APacmanPawn::EndFrightenMode()
{
	GameMode->BlinkyPawn->PhantomState = Chase;
	GameMode->InkyPawn->PhantomState = Chase;
	GameMode->PinkyPawn->PhantomState = Chase;
	GameMode->ClydePawn->PhantomState = Chase;
	CurrentMovementSpeed = 400.0f;
	GetWorld()->GetTimerManager().ClearTimer(FrightenedTimer);
	GetWorldTimerManager().UnPauseTimer(GameMode->BlinkyPawn->Timer);
	GetWorldTimerManager().UnPauseTimer(GameMode->InkyPawn->Timer);
	GetWorldTimerManager().UnPauseTimer(GameMode->PinkyPawn->Timer);
	GetWorldTimerManager().UnPauseTimer(GameMode->ClydePawn->Timer);
}

void APacmanPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto PointNode = Cast<AGridBaseNode>(OtherActor);

	if (PointNode && PointNode->CanBeEat()) {

		if (PointNode->Powered()) {
			PointNode->Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PointNode->StaticMesh->SetVisibility(false);
			PointNode->IsEatable = false;
			score += 10;
			CurrentMovementSpeed = 450.0f;
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("PACMAN ha mangiato power node")));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(score));
			GameMode->BlinkyPawn->SetFrightenedState();
			GameMode->InkyPawn->SetFrightenedState();
			GameMode->PinkyPawn->SetFrightenedState();
			GameMode->ClydePawn->SetFrightenedState();
			GetWorldTimerManager().PauseTimer(GameMode->BlinkyPawn->Timer);
			GetWorldTimerManager().PauseTimer(GameMode->InkyPawn->Timer);
			GetWorldTimerManager().PauseTimer(GameMode->PinkyPawn->Timer);
			GetWorldTimerManager().PauseTimer(GameMode->ClydePawn->Timer);
			if(GameMode->BlinkyPawn->PhantomState==Frightened){ GetWorld()->GetTimerManager().ClearTimer(FrightenedTimer);}
			GetWorldTimerManager().SetTimer(FrightenedTimer, this, &APacmanPawn::EndFrightenMode, 6.0f, false);
		}

		if (PointNode->Fruit())
		{
			PointNode->Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PointNode->StaticMesh->SetVisibility(false);
			PointNode->IsEatable = false;
			score += 100;
		}

		else
		{
			PointNode->Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PointNode->StaticMesh->SetVisibility(false);
			PointNode->IsEatable = false;
			score += 1;
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(score));

		}
		if (score == 1)
		{
			GameMode->PinkyPawn->exit();
		}
		if (score == 3)
		{
			GameMode->InkyPawn->exit();
		}
		if (score == 6) {
			GameMode->ClydePawn->exit();
		}
		/*if (score == total_score)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(score));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Hai VINTO!!")));
			Destroy();
		}*/
	}

};

void APacmanPawn::PacmanSpawn()
{
		CurrentGridCoords = FVector2D(13, 14);
		SetNextNode(*(CustomTileMap.Find(FVector2D(13, 14))));
		SetTargetNode(NextNode);
		LastNode = *(CustomTileMap.Find(FVector2D(13, 14)));
		const FVector spawn(1450.0f, 1350.0f, GetActorLocation().Z);
		SetActorLocation(spawn);	
}