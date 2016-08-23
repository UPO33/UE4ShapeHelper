// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "UPOLibPrivatePCH.h"
#include "UPOLibBPLibrary.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"


bool UUPOLibBPLibrary::IsPoinInShapeComponent(class UShapeComponent* shape, const FVector& pointWS)
{
	if (shape)
	{
		if (shape->IsA(UBoxComponent::StaticClass()))
		{
			FVector extent = ((UBoxComponent*)shape)->GetUnscaledBoxExtent();
			FVector p = shape->ComponentToWorld.InverseTransformPosition(pointWS);

			return (p.X <= extent.X && p.X >= -extent.X)
				&& (p.Y <= extent.Y && p.Y >= -extent.Y)
				&& (p.Z <= extent.Z && p.Z >= -extent.Z);
		}
		else if (shape->IsA(USphereComponent::StaticClass()))
		{
			float dist = (shape->GetComponentLocation() - pointWS).Size();
			return dist <= ((USphereComponent*)shape)->GetScaledSphereRadius();

		}
		else if (shape->IsA(UCapsuleComponent::StaticClass()))
		{
			UCapsuleComponent* capsule = (UCapsuleComponent*)shape;

			FTransform localToWorld = capsule->ComponentToWorld;
			FVector scale = localToWorld.GetScale3D().GetAbs();
			const float CapsuleHalfHeight = capsule->GetUnscaledCapsuleHalfHeight() * scale.Z;
			const float CapsuleRadius = capsule->GetUnscaledCapsuleRadius() * FMath::Max(scale.X, scale.Y);

			if (CapsuleHalfHeight <= CapsuleRadius)
			{
				return FVector::Dist(localToWorld.GetTranslation(), pointWS) <= CapsuleHalfHeight;
			}
			else
			{
				const FVector PointOffset = (CapsuleHalfHeight - CapsuleRadius) * localToWorld.GetUnitAxis(EAxis::Z);
				const FVector StartPoint = localToWorld.GetTranslation() + PointOffset;
				const FVector EndPoint = localToWorld.GetTranslation() - PointOffset;

				return FMath::PointDistToSegment(pointWS, StartPoint, EndPoint) <= CapsuleRadius;
			}
		}
	}
	return false;

}
