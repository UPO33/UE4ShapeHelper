#pragma once

#include "UPOLibEditorPrivatePCH.h"
#include "ShapeHelperComponentVis.h"
#include "ScopedTransaction.h"

/**Base class for clickable targeting editing proxies*/
struct HShapeVisProxy : public HComponentVisProxy
{
	DECLARE_HIT_PROXY();

	int Index;
	int TypeID;

	static const int TYPE_BOX = 0;
	static const int TYPE_SHPERE = 1;
	static const int TYPE_CAPSULE = 2;
	static const int TYPE_TESTPOINT = 3;

	HShapeVisProxy(const UActorComponent* InComponent, int index, int typeID)
		: HComponentVisProxy(InComponent, HPP_Wireframe), Index(index), TypeID(typeID)
	{}
};



IMPLEMENT_HIT_PROXY(HShapeVisProxy, HComponentVisProxy);



//////////////////////////////////////////////////////////////////////////
void FShapeHelperComponentVis::Reg()
{
	if(GUnrealEd)
		GUnrealEd->RegisterComponentVisualizer(UShapeHelperComponent::StaticClass()->GetFName(), MakeShareable(new FShapeHelperComponentVis));

}
//////////////////////////////////////////////////////////////////////////
void FShapeHelperComponentVis::Unreg()
{
	if(GUnrealEd)
		GUnrealEd->UnregisterComponentVisualizer(UShapeHelperComponent::StaticClass()->GetFName());

}
//////////////////////////////////////////////////////////////////////////
void FShapeHelperComponentVis::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const FLinearColor SelectedColor = FLinearColor::Red;
	const FLinearColor NormalColor = FLinearColor::Green;

	if (const UShapeHelperComponent* shapeComp = Cast<UShapeHelperComponent>(Component))
	{
		if (!shapeComp->bDrawShapes)
		{
			SelectedIndex = SelectedTypeID = -1;
			return;
		}

		bool bIsSelectedComp = true;
		if (GetEditingComp() && shapeComp != GetEditingComp())
			bIsSelectedComp = false;

		/////////////draw boxes
		for (int32 i = 0; i < shapeComp->Boxes.Num(); i++)
		{
			const FHelperBox& box = shapeComp->Boxes[i];

			FTransform transform = FTransform(box.Rotation, box.Location) * shapeComp->ComponentToWorld;

			FLinearColor color = NormalColor;
			if (bIsSelectedComp && SelectedIndex == i && SelectedTypeID == HShapeVisProxy::TYPE_BOX)
				color = SelectedColor;

			PDI->SetHitProxy(new HShapeVisProxy(Component, i, HShapeVisProxy::TYPE_BOX));
			DrawWireBox(PDI, transform.ToMatrixWithScale(), FBox(-box.Extent, box.Extent), color, SDPG_World);
		}

		////////////drw shperes
		for (int32 i = 0; i < shapeComp->Spheres.Num(); i++)
		{
			FLinearColor color = NormalColor;
			if (bIsSelectedComp && SelectedIndex == i && SelectedTypeID == HShapeVisProxy::TYPE_SHPERE)
				color = SelectedColor;

			FTransform transform = FTransform(shapeComp->Spheres[i].Location) * shapeComp->ComponentToWorld;
			float radius = shapeComp->Spheres[i].Radius * transform.GetScale3D().GetAbsMin();
			PDI->SetHitProxy(new HShapeVisProxy(Component, i, HShapeVisProxy::TYPE_SHPERE));
			DrawWireSphere(PDI, transform.GetLocation(), color, radius, 32, SDPG_World);
		}

		/////////draw capsules
		for (int32 i = 0; i < shapeComp->Capsules.Num(); i++)
		{
			FLinearColor color = NormalColor;
			if (bIsSelectedComp && SelectedIndex == i && SelectedTypeID == HShapeVisProxy::TYPE_CAPSULE)
				color = SelectedColor;

			FTransform transform = FTransform(shapeComp->Capsules[i].Rotation, shapeComp->Capsules[i].Location) * shapeComp->ComponentToWorld;
			FVector absScale = transform.GetScale3D().GetAbs();
			float halfHeight = shapeComp->Capsules[i].HalfHeight * absScale.Z;
			float radius = shapeComp->Capsules[i].Radius * FMath::Max(absScale.X, absScale.Y);
			
			PDI->SetHitProxy(new HShapeVisProxy(Component, i, HShapeVisProxy::TYPE_CAPSULE));
			DrawWireCapsule(PDI, transform.GetLocation(), transform.GetUnitAxis(EAxis::X), transform.GetUnitAxis(EAxis::Y)
				, transform.GetUnitAxis(EAxis::Z), color, radius, halfHeight, 32, SDPG_World);
		}
		
		PDI->SetHitProxy(nullptr);
	}
}
//////////////////////////////////////////////////////////////////////////
void FShapeHelperComponentVis::DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas)
{
	if (!Component || !Viewport || !View || !Canvas) return;

	if (const UShapeHelperComponent* shapeComp = Cast<UShapeHelperComponent>(Component))
	{
		if (!shapeComp->bDrawTags) return;

		UFont* defaultFont = GEngine->GetMediumFont();
		FLinearColor textColor = FLinearColor::White;

		///////draw spheres
		for (int32 i = 0; i < shapeComp->Spheres.Num(); i++)
		{
			const FHelperSphere& sphere = shapeComp->Spheres[i];
			FVector pointWS = shapeComp->ComponentToWorld.TransformPosition(sphere.Location);
			FVector2D screenPos;
			if (FSceneView::ProjectWorldToScreen(pointWS, View->ViewRect, View->ViewMatrices.GetViewProjMatrix(), screenPos))
			{
				FCanvasTextItem textItem = FCanvasTextItem(screenPos, FText::FromName(sphere.Tag), defaultFont, textColor);
				Canvas->DrawItem(textItem);
			}
		}
		/////draw boxes
		for (int32 i = 0; i < shapeComp->Boxes.Num(); i++)
		{
			const FHelperBox& box = shapeComp->Boxes[i];
			FVector pointWS = shapeComp->ComponentToWorld.TransformPosition(box.Location);
			FVector2D screenPos;
			if (FSceneView::ProjectWorldToScreen(pointWS, View->ViewRect, View->ViewMatrices.GetViewProjMatrix(), screenPos))
			{
				FCanvasTextItem textItem = FCanvasTextItem(screenPos, FText::FromName(box.Tag), defaultFont, textColor);
				Canvas->DrawItem(textItem);
			}
		}
		///////draw capsules
		for (int32 i = 0; i < shapeComp->Capsules.Num(); i++)
		{
			const FHelperCapsule& capsule = shapeComp->Capsules[i];
			FVector pointWS = shapeComp->ComponentToWorld.TransformPosition(capsule.Location);
			FVector2D screenPos;
			if (FSceneView::ProjectWorldToScreen(pointWS, View->ViewRect, View->ViewMatrices.GetViewProjMatrix(), screenPos))
			{
				FCanvasTextItem textItem = FCanvasTextItem(screenPos, FText::FromName(capsule.Tag), defaultFont, textColor);
				Canvas->DrawItem(textItem);
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
bool FShapeHelperComponentVis::VisProxyHandleClick(FLevelEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
	SelectedIndex = -1;
	SelectedTypeID = -1;
	if (VisProxy && VisProxy->Component.IsValid())
	{
		OwningActor = VisProxy->Component->GetOwner();
		EditingComp = Cast<UShapeHelperComponent>(VisProxy->Component.Get());
		PropertyName = GetComponentPropertyName(VisProxy->Component.Get());

		if (HShapeVisProxy* proxy = HitProxyCast<HShapeVisProxy>(VisProxy))
		{
			SelectedIndex = proxy->Index;
			SelectedTypeID = proxy->TypeID;
			return true;
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool FShapeHelperComponentVis::GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const
{
	if (auto EditingComponent = GetEditingComp())
	{
		if (SelectedTypeID == HShapeVisProxy::TYPE_BOX)
		{
			if (EditingComponent->Boxes.IsValidIndex(SelectedIndex))
			{
				OutLocation = EditingComponent->ComponentToWorld.TransformPosition(EditingComponent->Boxes[SelectedIndex].Location);
				return true;
			}
			
		}
		else if (SelectedTypeID == HShapeVisProxy::TYPE_SHPERE)
		{
			if (EditingComponent->Spheres.IsValidIndex(SelectedIndex))
			{
				OutLocation = EditingComponent->ComponentToWorld.TransformPosition(EditingComponent->Spheres[SelectedIndex].Location);
				return true;
			}
			
		}
		else if (SelectedTypeID == HShapeVisProxy::TYPE_CAPSULE)
		{
			if (EditingComponent->Capsules.IsValidIndex(SelectedIndex))
			{
				OutLocation = EditingComponent->ComponentToWorld.TransformPosition(EditingComponent->Capsules[SelectedIndex].Location);
				return true;
			}
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool FShapeHelperComponentVis::HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltalRotate, FVector& DeltaScale)
{
	const float SCALE_MUL = 3;

	auto EditingComponent = GetEditingComp();
	if (EditingComponent)
	{
		if (SelectedTypeID == HShapeVisProxy::TYPE_BOX)
		{
			if (EditingComponent->Boxes.IsValidIndex(SelectedIndex))
			{
				FTransform localToWorld = EditingComponent->ComponentToWorld;
				FHelperBox& box = EditingComponent->Boxes[SelectedIndex];
				box.Location += localToWorld.GetRotation().UnrotateVector(DeltaTranslate) / localToWorld.GetScale3D();
				box.Rotation =  DeltalRotate.Quaternion() * box.Rotation;
				box.Extent = FVector(box.Extent + DeltaScale * SCALE_MUL).ComponentMax(FVector(0));
				return true;
			}
		}
		else if (SelectedTypeID == HShapeVisProxy::TYPE_SHPERE)
		{
			if (EditingComponent->Spheres.IsValidIndex(SelectedIndex))
			{
				FTransform localToWorld = EditingComponent->ComponentToWorld;
				FHelperSphere& sphere = EditingComponent->Spheres[SelectedIndex];
				sphere.Location += localToWorld.GetRotation().UnrotateVector(DeltaTranslate) / localToWorld.GetScale3D();
				sphere.Radius = FMath::Max(sphere.Radius + DeltaScale.GetMax() * SCALE_MUL, 0.0f);
				return true;
			}
		}
		else if (SelectedTypeID == HShapeVisProxy::TYPE_CAPSULE)
		{
			if (EditingComponent->Capsules.IsValidIndex(SelectedIndex))
			{
				FTransform localToWorld = EditingComponent->ComponentToWorld;
				FHelperCapsule& capsule = EditingComponent->Capsules[SelectedIndex];
				capsule.Location += localToWorld.GetRotation().UnrotateVector(DeltaTranslate) / localToWorld.GetScale3D();
				capsule.Rotation = DeltalRotate.Quaternion() * capsule.Rotation;

				capsule.HalfHeight = FMath::Abs(capsule.HalfHeight + DeltaScale.Z * SCALE_MUL);
				float radScale = DeltaScale.X == 0 ? DeltaScale.Y : DeltaScale.X;
				capsule.Radius = FMath::Abs(capsule.Radius + radScale * SCALE_MUL);

				capsule.HalfHeight = FMath::Max(capsule.HalfHeight, capsule.Radius);
				capsule.Radius = FMath::Clamp(capsule.Radius, 0.0f, capsule.HalfHeight);
				return true;
			}
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool FShapeHelperComponentVis::GetCustomInputCoordinateSystem(const FEditorViewportClient* ViewportClient, FMatrix& OutMatrix) const
{
	auto EditingComponent = GetEditingComp();

	if (!EditingComponent) return false;


	if (SelectedTypeID == HShapeVisProxy::TYPE_BOX)
	{
		if (ViewportClient->GetWidgetCoordSystemSpace() == COORD_Local)
		{
			if (EditingComponent->Boxes.IsValidIndex(SelectedIndex))
			{
				OutMatrix = FRotationMatrix::Make(EditingComponent->GetComponentQuat() * EditingComponent->Boxes[SelectedIndex].Rotation);
				return true;
			}
			return false;
		}
		else
		{
			OutMatrix = FMatrix::Identity;
			return true;
		}
	}
	else if(SelectedTypeID == HShapeVisProxy::TYPE_SHPERE)
	{
		if (ViewportClient->GetWidgetCoordSystemSpace() == COORD_Local)
		{
			if (EditingComponent->Spheres.IsValidIndex(SelectedIndex))
			{
				OutMatrix = FRotationMatrix::Make(EditingComponent->GetComponentQuat());
				return true;
			}
			return false;
		}
		else
		{
			OutMatrix = FMatrix::Identity;
			return true;
		}
	}
	else if (SelectedTypeID == HShapeVisProxy::TYPE_CAPSULE)
	{
		if (ViewportClient->GetWidgetCoordSystemSpace() == COORD_Local)
		{
			if (EditingComponent->Capsules.IsValidIndex(SelectedIndex))
			{
				OutMatrix = FRotationMatrix::Make(EditingComponent->GetComponentQuat() * EditingComponent->Capsules[SelectedIndex].Rotation);
				return true;
			}
			return false;
		}
		else
		{
			OutMatrix = FMatrix::Identity;
			return true;
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
void FShapeHelperComponentVis::EndEditing()
{
	SelectedIndex = -1;
	SelectedTypeID = -1;
	OwningActor = nullptr;
	EditingComp = nullptr;
	PropertyName.Clear();
	GEditor->RedrawLevelEditingViewports(true);
}
//////////////////////////////////////////////////////////////////////////
bool FShapeHelperComponentVis::HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	return false;
}
//////////////////////////////////////////////////////////////////////////
void FShapeHelperComponentVis::OnDuplicateItem()
{
	if (auto EditingComponent = GetEditingComp())
	{
		if (SelectedTypeID != -1 && SelectedIndex != -1)
		{
			if (SelectedTypeID == HShapeVisProxy::TYPE_BOX)
			{
				if (EditingComponent->Boxes.IsValidIndex(SelectedIndex))
				{
					FHelperBox tmp = EditingComponent->Boxes[SelectedIndex];
					SelectedIndex = EditingComponent->Boxes.Add(tmp);
				}
			}
			else if (SelectedTypeID == HShapeVisProxy::TYPE_SHPERE)
			{
				if (EditingComponent->Spheres.IsValidIndex(SelectedIndex))
				{
					FHelperSphere tmp = EditingComponent->Spheres[SelectedIndex];
					SelectedIndex = EditingComponent->Spheres.Add(tmp);
				}
			}
			else if (SelectedTypeID == HShapeVisProxy::TYPE_CAPSULE)
			{
				if (EditingComponent->Capsules.IsValidIndex(SelectedIndex))
				{
					FHelperCapsule tmp = EditingComponent->Capsules[SelectedIndex];
					SelectedIndex = EditingComponent->Capsules.Add(tmp);
				}
			}
		}
	}

	GEditor->RedrawLevelEditingViewports();
}
//////////////////////////////////////////////////////////////////////////
void FShapeHelperComponentVis::OnRemoveItem()
{
// 	const FScopedTransaction Transaction(NSLOCTEXT("ShapeHealper", "DeleteSplinePoint", "Delete Spline Point"));

	if (auto EditingComponent = GetEditingComp())
	{
		if (SelectedTypeID != -1 && SelectedIndex != -1)
		{
			if (SelectedTypeID == HShapeVisProxy::TYPE_BOX)
			{
				if (EditingComponent->Boxes.IsValidIndex(SelectedIndex))
				{
					EditingComponent->Boxes.RemoveAt(SelectedIndex);
					SelectedIndex = SelectedTypeID = -1;
				}
			}
			else if (SelectedTypeID == HShapeVisProxy::TYPE_SHPERE)
			{
				if (EditingComponent->Spheres.IsValidIndex(SelectedIndex))
				{
					EditingComponent->Spheres.RemoveAt(SelectedIndex);
					SelectedIndex = SelectedTypeID = -1;
				}
			}
			else if (SelectedTypeID == HShapeVisProxy::TYPE_CAPSULE)
			{
				if (EditingComponent->Capsules.IsValidIndex(SelectedIndex))
				{
					EditingComponent->Capsules.RemoveAt(SelectedIndex);
					SelectedIndex = SelectedTypeID = -1;
				}
			}
		}
	}

	GEditor->RedrawLevelEditingViewports();
}
//////////////////////////////////////////////////////////////////////////
bool FShapeHelperComponentVis::CanRemoveItem() const
{
	return (SelectedTypeID != -1 && SelectedIndex != -1);
}
//////////////////////////////////////////////////////////////////////////
bool FShapeHelperComponentVis::CanDuplicateItem() const
{
	return (SelectedTypeID != -1 && SelectedIndex != -1);
}
//////////////////////////////////////////////////////////////////////////
UShapeHelperComponent* FShapeHelperComponentVis::GetEditingComp() const
{
	return EditingComp.Get();
// 	return Cast<UShapeHelperComponent>(GetComponentFromPropertyName(OwningActor.Get(), PropertyName));
}
//////////////////////////////////////////////////////////////////////////
TSharedPtr<SWidget> FShapeHelperComponentVis::GenerateContextMenu() const
{
	FMenuBuilder lMenu = FMenuBuilder(true, nullptr);
// 	lMenu.BeginSection(NAME_None, NSLOCTEXT("SHH", "SHH", "Sec0"));
	


	FUIAction lRemoveAction = FUIAction(
		FExecuteAction::CreateSP(this, &FShapeHelperComponentVis::OnRemoveItem), 
		FCanExecuteAction::CreateSP(this, &FShapeHelperComponentVis::CanRemoveItem));

	FUIAction lDuplicateAction = FUIAction(
		FExecuteAction::CreateSP(this, &FShapeHelperComponentVis::OnDuplicateItem),
		FCanExecuteAction::CreateSP(this, &FShapeHelperComponentVis::CanDuplicateItem));

	lMenu.AddMenuEntry(NSLOCTEXT("ShapeHelper", "Remove", "Remove"), FText::GetEmpty(), FSlateIcon::FSlateIcon(), lRemoveAction);
	lMenu.AddMenuEntry(NSLOCTEXT("ShapeHelper", "Duplicate", "Duplicate"), FText::GetEmpty(), FSlateIcon::FSlateIcon(), lDuplicateAction);
	
// 	lMenu.EndSection();
	return lMenu.MakeWidget();
}
