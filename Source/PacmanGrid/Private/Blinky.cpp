// Fill out your copyright notice in the Description page of Project Settings.


#include "Blinky.h"
#include "PacmanPawn.h"

AGridBaseNode* ABlinky::GetPlayerRelativeTarget()
{
	return Super::GetPlayerRelativeTarget();
}

ABlinky::ABlinky()
{
	CurrentGridCoords = FVector2D(27, 26);
}

void ABlinky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->GetTargetNode() == nullptr)
	{
		SetGhostTarget();
	}
}

void ABlinky::SetGhostTarget()
{
	
	AGridBaseNode* Target = GetPlayerRelativeTarget();

	if (eaten == true)
	{
		//CurrentMovementSpeed = 1000.0f;
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
		}
	}
	if(PhantomState == Scatter)
	{
		//cambio direzione
		InvertDirection();
		Target->SetGridPosition(26, 30); //(Y,X)??
	}
	if (PhantomState == Frightened)
	{
		InvertDirection();
		int x = rand()%29-0;
		int y = rand()%31-0;
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