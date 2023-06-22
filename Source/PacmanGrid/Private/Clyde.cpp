// Fill out your copyright notice in the Description page of Project Settings.


#include "Clyde.h"
#include "PacmanPawn.h"
#include "Math/Vector.h"

AGridBaseNode* AClyde::GetPlayerRelativeTarget()
{
	return Super::GetPlayerRelativeTarget();
}

AClyde::AClyde()
{
	CurrentGridCoords = FVector2D(2, 26);
}

void AClyde::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->GetTargetNode() == nullptr)
	{
		SetGhostTarget();
	}
}

void AClyde::SetGhostTarget()
{
	AGridBaseNode* Target = GetPlayerRelativeTarget();

	if (eaten == true)
	{
		Target->SetGridPosition(19,13);
		if (LastNode->GetGridPosition() == FVector2D(19, 13))
		{
			Home();
		}
	}
	
	if (PhantomState == Chase) {
		if (!Target)
		{
			FVector2D Pacman = GetPlayer()->GetLastNodeCoords();
			FVector2D Clyde_pos = GetClyde()->GetLastNodeCoords();

			double dx = pow((Pacman.X - Clyde_pos.X), 2);
			double dy = pow((Pacman.Y - Clyde_pos.Y), 2);
			double distanza = sqrt(dx + dy);

			if (distanza > 8)
			{
				Target = GetPlayer()->GetLastNode();
			}

			else
			{
				Target->SetGridPosition(0, 0);
			}
		}
	}

	if (PhantomState == Scatter)
	{
		Target->SetGridPosition(0, 0);
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
