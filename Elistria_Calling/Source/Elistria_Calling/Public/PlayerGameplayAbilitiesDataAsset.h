// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Abilities/GameplayAbility.h"
#include "InputAction.h"
#include "PlayerGameplayAbilitiesDataAsset.generated.h"

/**
 * 
 */
class UInputAction;

USTRUCT(BlueprintType)
struct FGameplayInputAbilityInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Ability")
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Ability", meta = (Categories="Input"))
	FGameplayTag InputTag;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Ability")
	bool bAutoGive = true;

	bool IsValid() const{ return AbilityClass && InputAction && InputTag.IsValid();}
	
	bool operator==(const FGameplayInputAbilityInfo& Other) const
	{
		return AbilityClass == Other.AbilityClass && InputAction == Other.InputAction && InputTag == Other.InputTag;
	}
	friend uint32 GetTypeHash(const FGameplayInputAbilityInfo& Item)
	{
		return HashCombine(GetTypeHash(Item.AbilityClass), GetTypeHash(Item.InputTag));
	}
};

UCLASS()
class ELISTRIA_CALLING_API UPlayerGameplayAbilitiesDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AbilitySystem")
	TArray<FGameplayInputAbilityInfo> AbilityMappings;

	UFUNCTION(BlueprintPure, Category = "AbilitySystem")
	void GetAllMappings(TArray<FGameplayInputAbilityInfo>& Out) const { Out = AbilityMappings; }

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void AddMapping(const FGameplayInputAbilityInfo& Entry)
	{
		if (!Entry.IsValid()) return;
		
		AbilityMappings.Add(Entry);
	}

	UFUNCTION(BlueprintCallable, Category = "AbilitySystem")
	void RemoveByTag(FGameplayTag InputTag)
	{
		AbilityMappings.RemoveAll([&](const FGameplayInputAbilityInfo& E) { return E.InputTag == InputTag; });
	}

	UFUNCTION(BlueprintPure, Category = "AbilitySystem")
	void FindByTag(FGameplayTag InputTag, TArray<FGameplayInputAbilityInfo>& Out) const
	{
		for (const auto& E : AbilityMappings)
		{
			if (E.InputTag == InputTag)
			{
				Out.Add(E);
			}
		}
	}
	const TArray<FGameplayInputAbilityInfo>& GetMappings() const { return AbilityMappings; }
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		// Ensure uniqueness (Set already helps). Could add validation if needed.
	}
#endif
};
