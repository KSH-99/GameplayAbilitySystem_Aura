// Copyright SHINHEE KIM


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController == nullptr)																		// OverlayWidgetController가 null이면
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);	// 새 객체를 생성하고
		OverlayWidgetController->SetWidgetControllerParams(WCParams);											// 변수 설정 후
		
		return OverlayWidgetController;																		// 리턴
	}
	return OverlayWidgetController;	// null이 아니면(이미 존재한다면) 존재하는 OverlayWidgetController 리턴
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class가 초기화되지 않았습니다. BP_AuraHUD를 입력하십시오"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class가 초기화되지 않았습니다. BP_AuraHUD를 입력하십시오"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	
	Widget->AddToViewport();
}

