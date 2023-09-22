// Fill out your copyright notice in the Description page of Project Settings.


#include "FogOfWarManager.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"

AFogOfWarManager* AFogOfWarManager::Instance;

void AFogOfWarManager::BeginPlay()
{
	Super::BeginPlay();

	FogOfWarMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, FogOfWarMaterial, FName(), EMIDCreationFlags::None);
	FogOfWarRevealMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, FogOfWarRevealMaterial, FName(), EMIDCreationFlags::None);

	UKismetRenderingLibrary::ClearRenderTarget2D(this, RevealTexture, FLinearColor::Black);
	UKismetRenderingLibrary::ClearRenderTarget2D(this, RenderTexture, FLinearColor::Black);
}

void AFogOfWarManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UKismetRenderingLibrary::ClearRenderTarget2D(this, RevealTexture, FLinearColor::Black);

	for (TPair<AActor*, float>& Pair : AllFogActors)
	{
		if (Pair.Key == nullptr)
			continue;

		FVector2D OutUV;
		FHitResult OutHit;

		UKismetSystemLibrary::LineTraceSingle(this, Pair.Key->GetActorLocation() + FVector(0, 0, 500), Pair.Key->GetActorLocation() - FVector(0, 0, 500), FogTraceChannel, true, TArray<AActor*>(), EDrawDebugTrace::None, OutHit, false);

		if (!UGameplayStatics::FindCollisionUV(OutHit, 0, OutUV))
			continue;

		FogOfWarMaterialInstance->SetVectorParameterValue(FName("Location"), FLinearColor(OutUV.X, OutUV.Y, 0, 0));
		FogOfWarRevealMaterialInstance->SetVectorParameterValue(FName("Location"), FLinearColor(OutUV.X, OutUV.Y, 0, 0));

		FogOfWarMaterialInstance->SetScalarParameterValue(FName("Size"), WorldRadiusToFogRadius(Pair.Value));
		FogOfWarRevealMaterialInstance->SetScalarParameterValue(FName("Size"), WorldRadiusToFogRadius(Pair.Value));

		UKismetRenderingLibrary::DrawMaterialToRenderTarget(this, RevealTexture, FogOfWarRevealMaterialInstance);
		UKismetRenderingLibrary::DrawMaterialToRenderTarget(this, RenderTexture, FogOfWarMaterialInstance);
	}
}

float AFogOfWarManager::WorldRadiusToFogRadius(float InRadius)
{
	return InRadius / GetActorScale3D().X * 0.009082;
}

AFogOfWarManager* AFogOfWarManager::GetFogOfWarManager(const UObject* WorldContextObject)
{
	if (Instance != nullptr)
		return Instance;

	TArray<AActor*> Out;
	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, AFogOfWarManager::StaticClass(), Out);

	Instance = static_cast<AFogOfWarManager*>(Out[0]);
	return Instance;
}

void AFogOfWarManager::RegisterActorToFog(AActor* InActor, float InRadius)
{
	AllFogActors.Add(InActor, InRadius);
}

void AFogOfWarManager::RemoveActorFromFog(AActor* InActor)
{
	AllFogActors.Remove(InActor);
}
