// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridPawn.h"
#include "PacmanPawn.h"
#include "PhantomPawn.generated.h"

/**
 * 
 */

 UENUM()
 enum EPhantomState { Chase, Scatter, Frightened };


UCLASS()
class PACMANGRID_API APhantomPawn : public AGridPawn
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	APhantomPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnNodeReached() override;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BlueMesh;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* WhiteMesh;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* EatenMesh;

private:
	UPROPERTY(VisibleAnywhere)
		class APacmanPawn* Player;
	UPROPERTY(VisibleAnywhere)
		class APhantomPawn* Blinky;
	UPROPERTY(VisibleAnywhere)
		class APhantomPawn* Clyde;
	int ScatterRound = 1;

public:
	//virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		APacmanPawn* GetPlayer() const;
	UFUNCTION()
		APhantomPawn* GetBlinky() const;
	UFUNCTION()
		APhantomPawn* GetClyde() const;
	UFUNCTION()
		void SetSpeed(float Speed);
	UFUNCTION()
		virtual AGridBaseNode* GetPlayerRelativeTarget();
	UFUNCTION()
		void SetScatterState();
	UFUNCTION()
		void SetChaseState();
	UFUNCTION()
		void SetFrightenedState();
	UFUNCTION()
		void InvertDirection();
	//void SetGhostTarget();
	UPROPERTY(EditAnywhere)
		TEnumAsByte<EPhantomState> PhantomState = Scatter;
	UPROPERTY(EditAnywhere)
		TEnumAsByte<EPhantomState> PrevPhantomState;
	FTimerHandle Timer;
	FTimerHandle MeshTimer;
	FTimerHandle HomeTimer;
	void SetBlueMesh();
	void SetWhiteMesh();
	void Eat();
	int flashes = 0;
	bool eaten;
	void Home();
	void exit();
};
