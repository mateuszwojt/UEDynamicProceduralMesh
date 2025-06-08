// Copyright Mateusz Wojt. All Rights Reserved.

#include "MWDynamicMeshActor.h"

AMWDynamicMeshActor::AMWDynamicMeshActor()
{
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	RootComponent = ProceduralMeshComponent;
}

void AMWDynamicMeshActor::Tick(float DeltaTime)
{
	// Called every frame
	Super::Tick(DeltaTime);
}

void AMWDynamicMeshActor::PostActorCreated()
{
	// Called when actor is spawned at runtime or when you drop it into the world in editor
	Super::PostActorCreated();
	GenerateMesh();
}

void AMWDynamicMeshActor::PostLoad()
{
	// Called when actor is already in the level
	Super::PostLoad();
	GenerateMesh();
}

void AMWDynamicMeshActor::BeginPlay()
{
	Super::BeginPlay();
	GenerateMesh();
}

void AMWDynamicMeshActor::GenerateMesh()
{
	if (IsValid(ProceduralMeshComponent))
	{
		
		ClearMeshData();
		GenerateMeshData();
		
		ProceduralMeshComponent->ClearAllMeshSections();

		// Don't care about the vertex colors and tangents at the moment, just pass empty arrays
		ProceduralMeshComponent->CreateMeshSection(0,
			Vertices,
			Triangles,
			Normals,
			UVs,
			TArray<FColor>(),
			TArray<FProcMeshTangent>(),
			true);
	}
}

void AMWDynamicMeshActor::GenerateMeshData()
{
    const float SegmentHeight = Height / HeightSegments;
    
    // We'll generate vertices separately for each face to have proper normals and UVs
    int32 VertexIndex = 0;
    
    // === SIDE FACES ===
    for (int32 Level = 0; Level < HeightSegments; ++Level)
    {
        float BottomHeight = Level * SegmentHeight;
        float TopHeight = (Level + 1) * SegmentHeight;
        
        float BottomLerpFactor = (float)Level / (float)HeightSegments;
        float TopLerpFactor = (float)(Level + 1) / (float)HeightSegments;
        
        float BottomSize = FMath::Lerp(BottomBaseSize, TopBaseSize, BottomLerpFactor);
        float TopSize = FMath::Lerp(BottomBaseSize, TopBaseSize, TopLerpFactor);
        
        float BottomHalf = BottomSize * 0.5f;
        float TopHalf = TopSize * 0.5f;
        
        // Front face (-Y direction)
        int32 FrontStart = VertexIndex;
        Vertices.Add(FVector(-BottomHalf, -BottomHalf, BottomHeight)); // Bottom-left
        Vertices.Add(FVector(BottomHalf, -BottomHalf, BottomHeight));  // Bottom-right
        Vertices.Add(FVector(TopHalf, -TopHalf, TopHeight));           // Top-right
        Vertices.Add(FVector(-TopHalf, -TopHalf, TopHeight));          // Top-left
        
        FVector FrontNormal = FVector(0, -1, 0);
        for (int32 i = 0; i < 4; ++i) { Normals.Add(FrontNormal); }
        
        UVs.Add(FVector2D(0, 0)); UVs.Add(FVector2D(1, 0)); UVs.Add(FVector2D(1, 1)); UVs.Add(FVector2D(0, 1));
        
        // Counter-clockwise winding for outward-facing normal
        Triangles.Add(FrontStart + 0); Triangles.Add(FrontStart + 2); Triangles.Add(FrontStart + 1);
        Triangles.Add(FrontStart + 0); Triangles.Add(FrontStart + 3); Triangles.Add(FrontStart + 2);
        VertexIndex += 4;
        
        // Right face (+X direction)
        int32 RightStart = VertexIndex;
        Vertices.Add(FVector(BottomHalf, -BottomHalf, BottomHeight));  // Bottom-front
        Vertices.Add(FVector(BottomHalf, BottomHalf, BottomHeight));   // Bottom-back
        Vertices.Add(FVector(TopHalf, TopHalf, TopHeight));            // Top-back
        Vertices.Add(FVector(TopHalf, -TopHalf, TopHeight));           // Top-front
        
        FVector RightNormal = FVector(1, 0, 0);
        for (int32 i = 0; i < 4; ++i) { Normals.Add(RightNormal); }
        
        UVs.Add(FVector2D(0, 0)); UVs.Add(FVector2D(1, 0)); UVs.Add(FVector2D(1, 1)); UVs.Add(FVector2D(0, 1));
        
        // Counter-clockwise winding for outward-facing normal
        Triangles.Add(RightStart + 0); Triangles.Add(RightStart + 2); Triangles.Add(RightStart + 1);
        Triangles.Add(RightStart + 0); Triangles.Add(RightStart + 3); Triangles.Add(RightStart + 2);
        VertexIndex += 4;
        
        // Back face (+Y direction)
        int32 BackStart = VertexIndex;
        Vertices.Add(FVector(BottomHalf, BottomHalf, BottomHeight));   // Bottom-right
        Vertices.Add(FVector(-BottomHalf, BottomHalf, BottomHeight));  // Bottom-left
        Vertices.Add(FVector(-TopHalf, TopHalf, TopHeight));           // Top-left
        Vertices.Add(FVector(TopHalf, TopHalf, TopHeight));            // Top-right
        
        FVector BackNormal = FVector(0, 1, 0);
        for (int32 i = 0; i < 4; ++i) { Normals.Add(BackNormal); }
        
        UVs.Add(FVector2D(0, 0)); UVs.Add(FVector2D(1, 0)); UVs.Add(FVector2D(1, 1)); UVs.Add(FVector2D(0, 1));
        
        // Counter-clockwise winding for outward-facing normal
        Triangles.Add(BackStart + 0); Triangles.Add(BackStart + 2); Triangles.Add(BackStart + 1);
        Triangles.Add(BackStart + 0); Triangles.Add(BackStart + 3); Triangles.Add(BackStart + 2);
        VertexIndex += 4;
        
        // Left face (-X direction)
        int32 LeftStart = VertexIndex;
        Vertices.Add(FVector(-BottomHalf, BottomHalf, BottomHeight));  // Bottom-back
        Vertices.Add(FVector(-BottomHalf, -BottomHalf, BottomHeight)); // Bottom-front
        Vertices.Add(FVector(-TopHalf, -TopHalf, TopHeight));          // Top-front
        Vertices.Add(FVector(-TopHalf, TopHalf, TopHeight));           // Top-back
        
        FVector LeftNormal = FVector(-1, 0, 0);
        for (int32 i = 0; i < 4; ++i) { Normals.Add(LeftNormal); }
        
        UVs.Add(FVector2D(0, 0)); UVs.Add(FVector2D(1, 0)); UVs.Add(FVector2D(1, 1)); UVs.Add(FVector2D(0, 1));
        
        // Counter-clockwise winding for outward-facing normal
        Triangles.Add(LeftStart + 0); Triangles.Add(LeftStart + 2); Triangles.Add(LeftStart + 1);
        Triangles.Add(LeftStart + 0); Triangles.Add(LeftStart + 3); Triangles.Add(LeftStart + 2);
        VertexIndex += 4;
    }
    
    // === BOTTOM FACE (facing down) ===
    float BottomHalf = BottomBaseSize * 0.5f;
    int32 BottomStart = VertexIndex;
    
    Vertices.Add(FVector(-BottomHalf, -BottomHalf, 0)); // Front-left
    Vertices.Add(FVector(-BottomHalf, BottomHalf, 0));  // Back-left
    Vertices.Add(FVector(BottomHalf, BottomHalf, 0));   // Back-right
    Vertices.Add(FVector(BottomHalf, -BottomHalf, 0));  // Front-right
    
    FVector BottomNormal = FVector(0, 0, -1);
    for (int32 i = 0; i < 4; ++i) { Normals.Add(BottomNormal); }
    
    UVs.Add(FVector2D(0, 0)); UVs.Add(FVector2D(0, 1)); UVs.Add(FVector2D(1, 1)); UVs.Add(FVector2D(1, 0));
    
    // Clockwise winding for downward-facing normal
    Triangles.Add(BottomStart + 0); Triangles.Add(BottomStart + 2); Triangles.Add(BottomStart + 1);
    Triangles.Add(BottomStart + 0); Triangles.Add(BottomStart + 3); Triangles.Add(BottomStart + 2);
    VertexIndex += 4;
    
    // === TOP FACE (facing up) ===
    float TopHalf = FMath::Max(TopBaseSize * 0.5f, 0.1f); // Ensure minimum size
    int32 TopStart = VertexIndex;
    
    // Arrange vertices in counter-clockwise order when viewed from above
    Vertices.Add(FVector(-TopHalf, -TopHalf, Height)); // Front-left
    Vertices.Add(FVector(TopHalf, -TopHalf, Height));  // Front-right  
    Vertices.Add(FVector(TopHalf, TopHalf, Height));   // Back-right
    Vertices.Add(FVector(-TopHalf, TopHalf, Height));  // Back-left
    
    FVector TopNormal = FVector(0, 0, 1);
    for (int32 i = 0; i < 4; ++i) { Normals.Add(TopNormal); }
    
    UVs.Add(FVector2D(0, 0)); UVs.Add(FVector2D(1, 0)); UVs.Add(FVector2D(1, 1)); UVs.Add(FVector2D(0, 1));
    
    // Clockwise winding for top face
    Triangles.Add(TopStart + 0); Triangles.Add(TopStart + 2); Triangles.Add(TopStart + 1);
    Triangles.Add(TopStart + 0); Triangles.Add(TopStart + 3); Triangles.Add(TopStart + 2);
}

void AMWDynamicMeshActor::ClearMeshData()
{
	Vertices.Empty();
	Triangles.Empty();
	Normals.Empty();
	UVs.Empty();
}

#if WITH_EDITOR
inline void AMWDynamicMeshActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr)
		? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(AMWDynamicMeshActor, BottomBaseSize) ||
			PropertyName == GET_MEMBER_NAME_CHECKED(AMWDynamicMeshActor, TopBaseSize) ||
			PropertyName == GET_MEMBER_NAME_CHECKED(AMWDynamicMeshActor, Height) ||
			PropertyName == GET_MEMBER_NAME_CHECKED(AMWDynamicMeshActor, HeightSegments))
	{
		GenerateMesh();
	}
}
#endif

