// Fill out your copyright notice in the Description page of Project Settings.


#include "ElistriaAbilitySystemComponent.h"

#include "NativeGameplayTags.h"
#include "PlayerGameplayAbilitiesDataAsset.h"

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Input_Confirm, "Input.Confirm");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Input_Cancel, "Input.Cancel");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Input_SelfCast, "Input.SelfCast");

void UElistriaAbilitySystemComponent::GrantAbilitiesFromDataAsset(const UPlayerGameplayAbilitiesDataAsset* AbilitiesDataAsset)
{
	 if (!AbilitiesDataAsset) return;
	for (const FGameplayInputAbilityInfo& AbilityInfo : AbilitiesDataAsset->GetMappings())
	 {
		 if (!AbilityInfo.IsValid()||!AbilityInfo.bAutoGive) continue;

		FGameplayAbilitySpec Spec(AbilityInfo.AbilityClass,1,INDEX_NONE);

		FGameplayAbilitySpecHandle Handle = GiveAbility(Spec);

		if (FGameplayAbilitySpec* Found = FindAbilitySpecFromHandle(Handle))
		{
			if (AbilityInfo.InputTag.IsValid()&&!Found->GetDynamicSpecSourceTags().HasTagExact(AbilityInfo.InputTag))
			{
		 			Found->GetDynamicSpecSourceTags().AddTagFast(AbilityInfo.InputTag);
		 			MarkAbilitySpecDirty(*Found);
			}
			TagToSpecHandleMap.Add(AbilityInfo.InputTag, Handle);
		}
	 }
	OnAbilitiesChanged.Broadcast(this);
}

void UElistriaAbilitySystemComponent::HandleInputTag(ETriggerEvent EventType, FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;

	if (InputTag == TAG_Input_Confirm)
	{
		TargetConfirm();
		return;
	}
	if (InputTag == TAG_Input_Cancel)
	{
		TargetCancel();
		return;
	}
	if (InputTag == TAG_Input_SelfCast)
	{
		return;
	}

	if (EventType == ETriggerEvent::Started||EventType == ETriggerEvent::Triggered)
	{
		FGameplayTagContainer Tags(InputTag);
		TryActivateAbilitiesByTag(Tags, true);
	}
}

