// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridGenerator.h"
#include "GridPawn.h"
#include "Blinky.h"
#include "Inky.h"
#include "Clyde.h"
#include "Pinky.h"
#include "Fruit.h"
#include "GameFramework/GameMode.h"
#include "TestGridGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PACMANGRID_API ATestGridGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	// TSubclassOf is a template class that provides UClass type safety.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGridGenerator> GridGeneratorClass;

	// reference to a GameField object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AGridGenerator* GField;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ABlinky> BlinkyClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AInky> InkyClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AClyde> ClydeClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<APinky> PinkyClass;

	UPROPERTY(VisibleAnywhere)
		ABlinky* BlinkyPawn;

	UPROPERTY(VisibleAnywhere)
		AInky* InkyPawn;

	UPROPERTY(VisibleAnywhere)
		AClyde* ClydePawn;

	UPROPERTY(VisibleAnywhere)
		APinky* PinkyPawn;

	UPROPERTY(VisibleAnywhere)
		AFruit* Fruit;

	ATestGridGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
