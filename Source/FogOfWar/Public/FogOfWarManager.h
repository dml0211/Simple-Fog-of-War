// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "FogOfWarManager.generated.h"

class UTextureRenderTarget2D;
class UMaterialInstanceDynamic;

UCLASS()
class FOGOFWAR_API AFogOfWarManager : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	static AFogOfWarManager* Instance;
	AFogOfWarManager() { PrimaryActorTick.bCanEverTick = true; }
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
		TMap<AActor*, float> AllFogActors;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
		UMaterialInterface* FogOfWarMaterial;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
		UMaterialInterface* FogOfWarRevealMaterial;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
		UTextureRenderTarget2D* RevealTexture;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
		UTextureRenderTarget2D* RenderTexture;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Fog Of War")
		TEnumAsByte<ETraceTypeQuery> FogTraceChannel;

	UPROPERTY(BlueprintReadOnly, Category = "Fog Of War")
		UMaterialInstanceDynamic* FogOfWarMaterialInstance;

	UPROPERTY(BlueprintReadOnly, Category = "Fog Of War")
		UMaterialInstanceDynamic* FogOfWarRevealMaterialInstance;

	UFUNCTION(BlueprintCallable, Category = "Fog Of War")
		void RegisterActorToFog(AActor* InActor, float InRadius);

	UFUNCTION(BlueprintCallable, Category = "Fog Of War")
		void RemoveActorFromFog(AActor* InActor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Fog Of War")
		float WorldRadiusToFogRadius(float InRadius);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "Fog Of War")
		static AFogOfWarManager* GetFogOfWarManager(const UObject* WorldContextObject);
};
