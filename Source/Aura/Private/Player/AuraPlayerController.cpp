// Copyright SHINHEE KIM


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if(!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/**
	 * Line trace from cursor. There are several scenarios:
	 *  A. LastActor is null && ThisActor is null
	 *		- Do nothing
	 *	B. LastActor is null && ThisActor is valid
	 *		- Highlight ThisActor.
	 *	C. LastActor is valid && ThisActor is null
	 *		- UnHighlight LastActor.
	 *	D. Both actors are valid, but LastActor != ThisActor
	 *		- UnHighlight LastActor and Highlight ThisActor.
	 *	E. Both actors are valid and are the same actor
	 *		- Do nothing
	 */

	if(LastActor == nullptr)
	{
		if(ThisActor)
		{
			//Cast B - Highlight ThisActor.
			ThisActor->HighLightActor();
		}else
		{
			// Case A - both are null, do nothing
		}
	}else // LastActor is valid
	{
		if(ThisActor == nullptr)
		{
			// Case C - UnHighlight LastActor.
			LastActor->UnHighLightActor();
		}else // both actor are valid
		{
			if(LastActor != ThisActor)
			{
				// Case D - UnHighlight LastActor and Highlight ThisActor.
				LastActor->UnHighLightActor();
				ThisActor->HighLightActor();
			}else
			{
				// Case E - Do nothing
			}
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	//MoveAction이 발생(ETriggerEvent::Triggered)했을 때 이 객체(this)가 Move함수를 실행
}
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisValue = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);//X축 얻기
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); //Y축 얻기

	if(APawn * ControlledPawn = GetPawn<APawn>())
	{	//AddMovementInput은 월드 방향
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisValue.Y); 
		ControlledPawn->AddMovementInput(RightDirection, InputAxisValue.X);
	}
}


