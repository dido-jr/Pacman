// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridBaseNode.h"
#include "Fruit.generated.h"

/**
 * 
 */
UCLASS()
class PACMANGRID_API AFruit : public AGridBaseNode
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	//AFruit();
};
