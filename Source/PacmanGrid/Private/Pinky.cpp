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
		Target->SetGridPosition(19, 13);//!!!!!!!!!!!!!!!!!!!!!!!!
		if (LastNode->GetGridPosition() == FVector2D(19, 13))
		{
			Home();
		}
	}

	if (PhantomState == Chase)
	{
		if (!Target)
		{
			Target = GetPlayer()->GetLastNode();
			FVector2D  Pacman = GetPlayer()->GetLastNodeCoords();
			FVector direzione = GetPlayer()->GetLastValidDirection();

			if (direzione == FVector(0, 1, 0)) //destra
			{
				Target->SetGridPosition(Pacman.Y, Pacman.X + 4); //(Y,X)??
			}

			if (direzione == FVector(0, -1, 0)) //sinistra
			{
				Target->SetGridPosition(Pacman.Y, Pacman.X - 4);
			}

			if (direzione == FVector(1, 0, 0))  //su
			{
				Target->SetGridPosition(Pacman.Y + 4, Pacman.X);
			}

			if (direzione == FVector(-1, 0, 0))  //giù
			{
				Target->SetGridPosition(Pacman.Y - 4, Pacman.X);
			}

		}
	}

	if(PhantomState == Scatter)
	{
		Target->SetGridPosition(1,30); //(Y,X)??
	}

	if (PhantomState == Frightened)
	{
		int x = rand() % 29 - 0;
		int y = rand() % 31 - 0;
		Target->SetGridPosition(y, x);
	}

	AGridBaseNode* PossibleNode = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target->GetGridPosition(), -(this->GetLastValidDirection()));

	const FVector Dimensions(60, 60, 20);
	DrawDebugBox(GetWorld(), PossibleNode->GetTileCoordinates(), Dimensions, FColor::Red);

	if (PossibleNode)
	{
		this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode->GetGridPosition() - this->GetLastNodeCoords()), true);
	}
}
