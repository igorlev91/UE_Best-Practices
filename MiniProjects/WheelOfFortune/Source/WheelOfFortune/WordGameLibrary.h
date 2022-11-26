#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WordGameLibrary.generated.h"

USTRUCT(BlueprintType)
struct FStringArray
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> Strings;
};

UCLASS()
class WHEELOFFORTUNE_API UWordGameLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "WordGameLibrary")
	static bool LoadWordsFromFile(FString FileName, int32 RequireWordLength, TMap<int32, FStringArray>& Words);
};
