#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankControlIntent.h"
#include "TankPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
struct FInputActionValue;
class ATankPawn;

UCLASS()
class NETWORKTANKARENA_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetPawn(APawn* InPawn) override;
	virtual void FlushPressedKeys() override;
	virtual void AcknowledgePossession(APawn* P) override;
	virtual void Reset() override;

	// Small localhost lab flow, also callable from a Blueprint widget or the console.
	UFUNCTION(Exec, BlueprintCallable, Category = "Tank|Localhost")
	void HostLocalhost();
	UFUNCTION(Exec, BlueprintCallable, Category = "Tank|Localhost")
	void JoinLocalhost();

	UFUNCTION(BlueprintNativeEvent)
	void OnStartChargeCosmetics();
	UFUNCTION(BlueprintNativeEvent)
	void OnStopChargeCosmetics();
	
protected:
	virtual void BeginPlay() override;
	virtual void ReceivedPlayer() override;
	virtual void SetupInputComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	friend class FTankMultiplayerAuthorityTest;
	friend class FTankMultiplayerLifecycleTest;
	friend class FTankMultiplayerContentTest;

	//Movement replication
	UFUNCTION(Server, Reliable)
	void ServerSubmitControlIntent(ATankPawn* TargetTank, FTankControlIntent Intent);
	void LocalSubmitControlIntent(ATankPawn* Tank, const FTankControlIntent& Intent);
	void AcceptControlIntent(ATankPawn* TargetTank, const FTankControlIntent& Intent);
	
	//Charge replication
	UFUNCTION(Server, Reliable)
	void ServerRequestChargeProjectile();
	void LocalRequestChargeProjectile();
	UFUNCTION(Server, Reliable)
	void ServerChargeProjectile();
	UFUNCTION(NetMulticast, Reliable) //Probably should be unreliable buut it feels better when it is
	void StartChargeCosmetics();
	UFUNCTION(NetMulticast, Reliable)
	void StopChargeCosmetics();
	
	//Fire replication
	UFUNCTION(Server, Reliable)
	void ServerRequestFireProjectile(FTransform FireTransform);
	void LocalRequestFireProjectile();
	void AllowFire();
	
	void SetControlledTank();
	void UpdateThrottle(const FInputActionValue& Value);
	void UpdateTurn(const FInputActionValue& Value);
	void ClearThrottle();
	void ClearTurn();
	void ClearCachedInput();
	void ApplyTankInputContext();
	void RemoveTankInputContext();

	//Cast and save on begin play, but use interface or delegates if we want more pawn options.
	UPROPERTY(EditDefaultsOnly, Category = "Tank|Input")
	ATankPawn* ControlledTank;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tank|Input")
	TObjectPtr<UInputAction> ThrottleAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tank|Input")
	TObjectPtr<UInputAction> TurnAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tank|Input")
	TObjectPtr<UInputAction> FireAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tank|Input")
	TObjectPtr<UInputAction> ChargeFireAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Tank|Input")
	TObjectPtr<UInputMappingContext> TankMappingContext;

	FTimerHandle SetTankTimer;
	
	double StartChargeTimeStamp = 0;
	bool bCanFire = true;
	FTimerHandle FireCooldownHandle;
	FTimerHandle ChargeFireHandle;
	
	float CachedThrottle = 0.f;
	float CachedTurn = 0.f;
	static constexpr float InputSendIntervalSeconds = 1.f / 20.f;
	float SecondsUntilInputSend = 0.f;
	TWeakObjectPtr<UEnhancedInputLocalPlayerSubsystem> MappingSubsystem;
	UPROPERTY(Transient)
	TObjectPtr<UInputMappingContext> AppliedMappingContext;
};
