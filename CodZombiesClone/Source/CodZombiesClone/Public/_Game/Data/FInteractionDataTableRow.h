#pragma once
#include "FInteractionDataTableRow.generated.h"

class UInputAction;

UENUM()
enum EInteractionType
{
	Default UMETA(DisplayName="Default"),
	Buyable UMETA(DisplayName="Buy"),
	Gainable UMETA(DisplayName="Gain")
};

USTRUCT(BlueprintType)
struct FInteractionDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString KeyDetails; // todo: E.g. "Press", "Hold", "Double Tap" etc.
	
	UPROPERTY(EditAnywhere)
	FString Connection;
	
	UPROPERTY(EditAnywhere)
	FString InteractionDetails;
	
	/** enables interaction conditions */
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EInteractionType> InteractionType = Default;
	
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle PriceData;
};
