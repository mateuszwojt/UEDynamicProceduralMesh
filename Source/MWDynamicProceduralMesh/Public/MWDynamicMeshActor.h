// Copyright Mateusz Wojt. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "MWDynamicMeshActor.generated.h"

UCLASS()
class MWDYNAMICPROCEDURALMESH_API AMWDynamicMeshActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMWDynamicMeshActor();

	virtual void Tick(float DeltaTime) override;
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProceduralMeshComponent* ProceduralMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters", meta = (ClampMin = "0.1", ClampMax = "100.0"))
	float BottomBaseSize = 60.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters", meta = (ClampMin = "0.1", ClampMax = "100.0"))
	float TopBaseSize = 40.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters", meta = (ClampMin = "0.1", ClampMax = "100.0"))
	float Height = 50.0f;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Parameters", meta = (ClampMin = "3", ClampMax = "10"))
	int32 HeightSegments = 4;

	UFUNCTION(BlueprintCallable, Category = "Mesh Generation")
	void GenerateMesh();

protected:
	UPROPERTY()
	TArray<FVector> Vertices;
	UPROPERTY()
	TArray<int32> Triangles;
	UPROPERTY()
	TArray<FVector> Normals;
	UPROPERTY()
	TArray<FVector2D> UVs;

private:
	void GenerateMeshData();
	void ClearMeshData();
};
