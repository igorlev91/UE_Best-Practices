#include "WordGameLibrary.h"
#include "Kismet/KismetStringLibrary.h"

static bool IsASCIILetter(char c)
{
	return (c >= 65 && c <= 90);
}

 bool UWordGameLibrary::LoadWordsFromFile(FString FileName, int32 RequireWordLength, TMap<int32, FStringArray>& Words)
{
	 //Load the raw text from the file
	 FString parsedText;
	 if (FFileHelper::LoadFileToString(parsedText, *(FPaths::ProjectContentDir() + FileName)) == false)
		 return false;

	 //parse the text so we have on entry per word
	 TArray<FString> parsedWords = UKismetStringLibrary::ParseIntoArray(parsedText, "\n");

	 for (FString& word : parsedWords)
	 {
		 if (word.Len() < RequireWordLength)
			 continue;

		 word.ToUpperInline();

		 bool containsOnlyValidCharacter = true;
		 for (const auto& letter : word.GetCharArray())
		 {
			 if (letter != '\0' && IsASCIILetter(letter) == false)
			 {
				 containsOnlyValidCharacter = false;
				 break;
			 }
		 }

		 //Add the word to map if it contains only valid letter
		 if (containsOnlyValidCharacter == true)
			 Words.FindOrAdd(word.Len()).Strings.Add(word);
	 }

	 return true;

}