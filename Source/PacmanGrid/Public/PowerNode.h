// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridBaseNode.h"
#include "PowerNode.generated.h"

/**
 * 
 */
UCLASS()
class PACMANGRID_API APowerNode : public AGridBaseNode
{
	GENERATED_BODY()

	protected:
		virtual void BeginPlay() override;

};
