#pragma once
#include "FPointPriceDataTableRow.h"

#include "FInteractionDataTableRow.generated.h"

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
	FString KeyDetails;

	UPROPERTY(EditAnywhere)
	FString KeyName;
	
	UPROPERTY(EditAnywhere)
	FString Connection;
	
	UPROPERTY(EditAnywhere)
	FString InteractionDetails;
	
	/** enables interaction conditions */
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EInteractionType> InteractionType;
	
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle PriceData;
};
