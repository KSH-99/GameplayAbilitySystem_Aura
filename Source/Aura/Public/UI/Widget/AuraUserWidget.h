// Copyright SHINHEE KIM

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController); // WidgetController를 설정하는 함수
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController; // AuraWidgetController를 위한 멤버변수

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
