// Fill out your copyright notice in the Description page of Project Settings.


#include "Inky.h"
#include "PacmanPawn.h"


AGridBaseNode* AInky::GetPlayerRelativeTarget()
{
	return Super::GetPlayerRelativeTarget();
}

AInky::AInky()
{
	CurrentGridCoords = FVector2D(5, 26);
}

void AInky::Tick(float DeltaTime)//
{
	Super::Tick(DeltaTime);
	if (this->GetTargetNode() == nullptr)
	{
		SetGhostTarget();
	}
}

void AInky::SetGhostTarget()
{
	AGridBaseNode* Target = GetPlayerRelativeTarget();

	if (eaten == true)
	{
		Target->SetGridPosition(19, 13);
		if (LastNode->GetGridPosition() == FVector2D(19, 13))
		{
			Home();
		}
	}

	if (PhantomState == Chase) {
		if (!Target)
		{
			Target = GetPlayer()->GetLastNode();
			FVector2D Pacman = GetPlayer()->GetLastNodeCoords();
			FVector2D Blinky_pos = GetBlinky()->GetLastNodeCoords();

			if (GetPlayer()->GetLastValidDirection() == FVector::ForwardVector) //Pacman va sù
			{
				Target->SetGridPosition(Blinky_pos.Y + 2 * (Pacman.Y + 2 - Blinky_pos.Y), Blinky_pos.X + 2 * (Pacman.X - Blinky_pos.X)); //(Y,X)??
			}
			if (GetPlayer()->GetLastValidDirection() == FVector::BackwardVector) //Pacman va giù
			{
				Target->SetGridPosition(Blinky_pos.Y + 2 * (Pacman.Y - 2 - Blinky_pos.Y), Blinky_pos.X + 2 * (Pacman.X - Blinky_pos.X));
			}
			else if (GetPlayer()->GetLastValidDirection() == FVector::RightVector) //Pacman va a sx o dx
			{
				Target->SetGridPosition(Blinky_pos.Y + 2 * (Pacman.Y - Blinky_pos.Y), Blinky_pos.X + 2 * (Pacman.X + 2 - Blinky_pos.X));
			}
			else if (GetPlayer()->GetLastValidDirection() == FVector::LeftVector) //Pacman va a sx o dx
			{
				Target->SetGridPosition(Blinky_pos.Y + 2 * (Pacman.Y - Blinky_pos.Y), Blinky_pos.X + 2 * (Pacman.X - 2 - Blinky_pos.X));
			}
		}
	}

	if(PhantomState == Scatter)
	{
		Target->SetGridPosition(27, 0); //(Y,X)
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
}//