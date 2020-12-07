// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnTurret.generated.h"


class APawnTank;


UCLASS()
class TOONTANKS_API APawnTurret : public APawnBase
{
	GENERATED_BODY()

private:
	
	
	void CheckFireCondition();
	float ReturnDistanceToPlayer();

	FTimerHandle FireRateTimerHandle;

	APawnTank* PlayerPawn;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowedPrivateAccess = "true"));
	float FireRate = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowedPrivateAccess = "true"));
	float FireRange = 500.0f;

	virtual void HandleDestruction() override;

protected:
	virtual void BeginPlay() override;
	
};
