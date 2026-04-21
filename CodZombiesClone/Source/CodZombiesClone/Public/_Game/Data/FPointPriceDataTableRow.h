#pragma once

#include "FPointPriceDataTableRow.generated.h"

USTRUCT(BlueprintType)
struct FPointPriceDataTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	int Cost = 0;
};
