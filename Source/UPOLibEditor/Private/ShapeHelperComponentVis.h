#pragma once

#include "ComponentVisualizer.h"
#include "Components/SceneComponent.h"
#include "ShapeHelperComponent.h"
#include "HitProxies.h"
#include "UnrealEd.h"

class FShapeHelperComponentVis : public FComponentVisualizer
{

public:

	static void Reg();
	static void Unreg();

	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;

	virtual void DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;

	/*
	 You can receive hits from your hit proxy by overriding the VisProxyHandleClick() function. 
	 This function receives the viewport client and the clicked vis proxy as parameters. 
	 It returns a bool indicating whether you have handled the click or not. For our simple example we just 
	 want to set our SelectedTargetIndex proxy to the clicked target.
	 */
	virtual bool VisProxyHandleClick(FLevelEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;
	/*
	 The first step to doing this is overriding the widget location so it appears on our target instead of the selected actor/component.
	 This can be done overriding the GetWidgetLocation() method. This method returns a bool to indicate
	 if it's provided the location and has an FVector out parameter for providing it. 
	 **/
	virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;

	virtual bool HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltalRotate, FVector& DeltaScale) override;

	virtual bool GetCustomInputCoordinateSystem(const FEditorViewportClient* ViewportClient, FMatrix& OutMatrix) const override;

	virtual void EndEditing() override;

	virtual bool HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;

	bool DupicateItem(int32 typeID, int32 index);
	bool DeleteItem(int32 typeID, int32 index);

	void OnDuplicateItem();
	void OnRemoveItem();
	bool CanRemoveItem() const;
	bool CanDuplicateItem() const;

	class UShapeHelperComponent* GetEditingComp() const;

	virtual TSharedPtr<SWidget> GenerateContextMenu() const override;

	
	int32 SelectedIndex = -1;
	int32 SelectedTypeID = -1;
	FPropertyNameAndIndex PropertyName;
	TWeakObjectPtr<AActor>	OwningActor = nullptr;
	TWeakObjectPtr<UShapeHelperComponent> EditingComp;
};