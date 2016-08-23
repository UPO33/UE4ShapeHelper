#include "UPOLibPrivatePCH.h"
#include "ShapeHelperComponent.h"

//////////////////////////////////////////////////////////////////////////
bool FHelperSphere::IsPointInside(const FVector& pointWS, const FTransform& componentToWorld /*= FTransform::Identity*/) const
{
	FVector v = componentToWorld.TransformPosition(Location);
	float r = componentToWorld.GetScale3D().GetAbsMin() * Radius;
	return FVector::DistSquared(v, pointWS) < (r * r);
}
//////////////////////////////////////////////////////////////////////////
bool FHelperBox::IsPointInside(const FVector& pointWS, const FTransform& componentToWorld /*= FTransform::Identity*/) const
{
	FTransform trs = FTransform(Rotation, Location) * componentToWorld;
	FVector v = trs.InverseTransformPosition(pointWS);

	return ((v.X > -Extent.X && v.X < Extent.X)
		&& (v.Y > -Extent.Y && v.Y < Extent.Y)
		&& (v.Z > -Extent.Z && v.Z < Extent.Z));

}
//////////////////////////////////////////////////////////////////////////
bool FHelperCapsule::IsPointInside(const FVector& pointWS, const FTransform& componentToWorld /*= FTransform::Identity*/) const
{
	FTransform localToWorld = FTransform(Rotation, Location) * componentToWorld;
	FVector scale = localToWorld.GetScale3D().GetAbs();
	const float CapsuleHalfHeight = HalfHeight * scale.Z;
	const float CapsuleRadius = Radius * FMath::Max(scale.X, scale.Y);

	if (CapsuleHalfHeight <= CapsuleRadius)
	{
		return FVector::Dist(localToWorld.GetTranslation(), pointWS) <= CapsuleHalfHeight;
	}
	else
	{
		const FVector PointOffset = (CapsuleHalfHeight - CapsuleRadius) * localToWorld.GetRotation().GetAxisZ();
		const FVector StartPoint = localToWorld.GetTranslation() + PointOffset;
		const FVector EndPoint = localToWorld.GetTranslation() - PointOffset;

		return FMath::PointDistToSegment(pointWS, StartPoint, EndPoint) <= CapsuleRadius;
	}
}

//////////////////////////////////////////////////////////////////////////
UShapeHelperComponent::UShapeHelperComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

//////////////////////////////////////////////////////////////////////////
bool UShapeHelperComponent::IsPointInAnySpheres(const FVector& point, int32& outIndex) const
{
	for (int32 i = 0; i < Spheres.Num(); i++)
	{
		if (Spheres[i].IsPointInside(point, ComponentToWorld))
		{
			outIndex = i;
			return true;
		}
	}

	outIndex = -1;
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool UShapeHelperComponent::IsPointInAnyBoxes(const FVector& point, int32& outIndex) const
{
	for (int32 i = 0; i < Boxes.Num(); i++)
	{
		if (Boxes[i].IsPointInside(point, ComponentToWorld))
		{
			outIndex = i;
			return true;
		}

	}

	outIndex = -1;
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool UShapeHelperComponent::IsPointInAnyCapsules(const FVector& point, int32& outIndex) const
{
	for (int32 i = 0; i < Capsules.Num(); i++)
	{
		if (Capsules[i].IsPointInside(point, ComponentToWorld))
		{
			outIndex = i;
			return true;
		}
	}
	outIndex = -1;
	return false;
}


#if WITH_EDITOR
//////////////////////////////////////////////////////////////////////////
void UShapeHelperComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property)
	{
		FName name = PropertyChangedEvent.Property->GetFName();
		if (name == TEXT("HalfHeight") || name == TEXT("Radius"))
		{
			for (int32 i = 0; i < Capsules.Num(); i++)
			{
				Capsules[i].HalfHeight = FMath::Max(Capsules[i].HalfHeight, Capsules[i].Radius);
				Capsules[i].Radius = FMath::Clamp(Capsules[i].Radius, 0.0f, Capsules[i].HalfHeight);
			}
		}
	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif

