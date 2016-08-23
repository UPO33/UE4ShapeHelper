#pragma once

#include "Components/SceneComponent.h"
#include "ShapeHelperComponent.generated.h"

//////////////////////////////////////////////////////////////////////////
USTRUCT(BlueprintType)
struct FHelperSphere
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector Location = FVector(0);
	UPROPERTY(EditAnywhere, meta=(ClampMin=0))
	float Radius = 32;
	UPROPERTY(EditAnywhere)
	FName	Tag;

	bool IsPointInside(const FVector& pointWS, const FTransform& componentToWorld = FTransform::Identity) const;
};
//////////////////////////////////////////////////////////////////////////
USTRUCT(BlueprintType)
struct FHelperBox
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FVector Location = FVector(0);
	UPROPERTY(EditAnywhere)
	FVector Extent = FVector(32);
	UPROPERTY(EditAnywhere)
	FQuat Rotation = FQuat::Identity;
	UPROPERTY(EditAnywhere)
	FName	Tag;

	bool IsPointInside(const FVector& pointWS, const FTransform& componentToWorld = FTransform::Identity) const;

};
//////////////////////////////////////////////////////////////////////////
USTRUCT(BlueprintType)
struct FHelperCapsule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector Location = FVector(0);
	UPROPERTY(EditAnywhere, meta=(ClampMin=0))
	float HalfHeight = 32;
	UPROPERTY(EditAnywhere, meta=(ClampMin=0))
	float Radius = 16;
	UPROPERTY(EditAnywhere)
	FQuat Rotation = FQuat::Identity;
	UPROPERTY(EditAnywhere)
	FName	Tag;

	bool IsPointInside(const FVector& pointWS, const FTransform& componentToWorld = FTransform::Identity) const;
};


UCLASS(BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class UPOLIB_API UShapeHelperComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FHelperSphere>	Spheres;
	UPROPERTY(EditAnywhere)
	TArray<FHelperBox>	Boxes;
	UPROPERTY(EditAnywhere)
	TArray<FHelperCapsule> Capsules;

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool bDrawTags = true;
	UPROPERTY(EditAnywhere, AdvancedDisplay)
	bool bDrawShapes = true;

#endif

	UShapeHelperComponent();

	UFUNCTION(BlueprintCallable, Category = "ShapeHelper")
	bool IsPointInAnySpheres(const FVector& point, int32& outIndex) const;
	UFUNCTION(BlueprintCallable, Category = "ShapeHelper")
	bool IsPointInAnyBoxes(const FVector& point, int32& outIndex) const;
	UFUNCTION(BlueprintCallable, Category = "ShapeHelper")
	bool IsPointInAnyCapsules(const FVector& point, int32& outIndex) const;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

