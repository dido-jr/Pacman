// Fill out your copyright notice in the Description page of Project Settings.


#include "Pinky.h"
#include "PacmanPawn.h"

AGridBaseNode* APinky::GetPlayerRelativeTarget()
{
	return Super::GetPlayerRelativeTarget();
}

APinky::APinky()
{
	CurrentGridCoords = FVector2D(2, 26);
}

void APinky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->GetTargetNode() == nullptr)
	{
		SetGhostTarget();
	}
}

void APinky::SetGhostTarget()
{
	AGridBaseNode* Target = GetPlayerRelativeTarget();
	
	if (eaten == true)
	{
		//CurrentMovementSpeed = 1000.0f;
		Target = *(CustomTileMap.Find(FVector2D(19, 13)));
		if (LastNode->GetGridPosition() == FVector2D(19, 13))
		{
			Home();
			this->PhantomState = this->PrevPhantomState;
		}
	}

	if (PhantomState == Chase)
	{
		if (!Target)
		{
			Target = GetPlayer()->GetLastNode();
			FVector2D  Pacman = GetPlayer()->GetLastNodeCoords();
			FVector direzione = GetPlayer()->GetLastValidDirection();
			FVector2D TargetProva;

			if (direzione == FVector(0, 1, 0)) //destra
			{
				TargetProva = FVector2D(Pacman.X + 4, Pacman.Y);
				if (TargetProva.X > 28)
				{
					TargetProva.X = 28;
				}
				Target = *(CustomTileMap.Find(FVector2D(TargetProva)));
			}

			if (direzione == FVector(0, -1, 0)) //sinistra
			{
				TargetProva = FVector2D(Pacman.X - 4, Pacman.Y);
				if (TargetProva.X < 0)
				{
					TargetProva.X = 0;
				}
				Target = *(CustomTileMap.Find(TargetProva));
			}

			if (direzione == FVector(1, 0, 0))  //su
			{
				TargetProva = FVector2D(Pacman.X, Pacman.Y + 4);
				if (TargetProva.Y > 30)
				{
					TargetProva.Y = 30;
				}
				Target = *(CustomTileMap.Find(FVector2D(TargetProva)));
			}

			if (direzione == FVector(-1, 0, 0))  //giù
			{
				TargetProva = FVector2D(Pacman.X, Pacman.Y - 4);
				if (TargetProva.Y < 0)
				{
					TargetProva.Y = 0;
				}
				Target = *(CustomTileMap.Find(FVector2D(TargetProva)));
			}
		}
	}

	if(PhantomState == Scatter)
	{
		Target = *(CustomTileMap.Find(FVector2D(30, 1)));
	}

	if (PhantomState == Frightened)
	{
		int x = rand() % 27 - 0;
		int y = rand() % 27 - 0;
		Target = *(CustomTileMap.Find(FVector2D(x, y)));
	}

	AGridBaseNode* PossibleNode = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target->GetGridPosition(), -(this->GetLastValidDirection()));

	const FVector Dimensions(60, 60, 20);
	DrawDebugBox(GetWorld(), PossibleNode->GetTileCoordinates(), Dimensions, FColor::Red);

	if (PossibleNode)
	{
		this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode->GetGridPosition() - this->GetLastNodeCoords()), true);
	}
}
