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
		Target = *(CustomTileMap.Find(FVector2D(19, 13)));
		if (LastNode->GetGridPosition() == FVector2D(19, 13))
		{
			Home();
			this->PhantomState = this->PrevPhantomState;
		}
	}

	else if (PhantomState == Chase) {
			Target = GetPlayer()->GetLastNode();
	}
	else if(PhantomState == Scatter)
	{ 
		Target = *(CustomTileMap.Find(FVector2D(29, 22)));
	}
	else if (PhantomState == Frightened)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Blinky Frightened")));
		int x = rand()%27-0;
		int y = rand()%27-0;
		Target = *(CustomTileMap.Find(FVector2D(y, x)));
	}

	AGridBaseNode* PossibleNode = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target->GetGridPosition(), -(this->GetLastValidDirection()));

	const FVector Dimensions(60, 60, 20);
	DrawDebugBox(GetWorld(), PossibleNode->GetTileCoordinates(), Dimensions, FColor::Red);

	if (PossibleNode)
	{
		this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode->GetGridPosition() - this->GetLastNodeCoords()), true);
	}
}