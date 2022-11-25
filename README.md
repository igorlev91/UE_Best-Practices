# UE_Best-Practices
 I wrote down some Unreal Engine best practices I've found in the past few years working in it professionally.

- [Setup](#Setup)
- [Style Guide - Project Organization and Naming](#Style-Guide-Project-Organization-and-Naming)
- [Derived Data Cache (DDC)](#DDC)
- [Team Setup (while in office)](#Team-Setup)
- [Locally](#Locally)
- [Perforce](#Perforce)
- [Rider](#Rider)
- [Visual Studio](#Visual-Studio)
- [Engineers](#Engineers)
- [Reflection System and C++ Coding Standard](#Reflection-System)
- [Built In Data Types](#Built-In-Data-Types)
- [TArray](#TArray)
- [TSet](#TSet)
- [TMap](#TMap)
- [Class Creation](#Class-Creation)
- [Polymorphic Components](#Polymorphic-Components)
- [UCLASS](#UCLASS)
- [HideCategories](#HideCategories)
- [UFUNCTION](#UFUNCTION)
- [BlueprintCallable](#BlueprintCallable)
- [BlueprintPure (Implies BlueprintCallable)](#BlueprintPure)
- [BlueprintGetter](#BlueprintGetter)
- [BlueprintImplementableEvent](#BlueprintImplementableEvent)
- [BlueprintNativeEvent](#BlueprintNativeEvent)
-)

## Setup
Style Guide - Project Organization and Naming
It is imperative to set up the project properly and have everyone agree to use a naming and style guide. I suggest having everyone on the project read/understand this and follow it:

GitHub - Allar/ue5-style-guide: An attempt to make Unreal Engine 4 projects more consistent

* [- GitHub - Allar/ue5-style-guide: An attempt to make Unreal Engine 4 projects more consistent](https://github.com/hollsteinm/ReasonablePlanningAI)

## DDC
This cache allows sharing of the cost to compile shaders and other operations. It’s a must when working in a team to prevent hours of waiting when working locally. It doesn’t make sense to use when working remotely because it would take longer to download than to generate, however you can specify the location of a local and shared DDC to prevent the warning messages from popping up.

Here’s Epic’s documentation:

* [Derived Data Cache](https://docs.unrealengine.com/5.1/en-US/derived-data-cache/)


## Team-Setup
The project should be set up to use a shared cache. An example setup used in Slingshot (DefaultEngine.ini):

[DerivedDataBackendGraph]
MinimumDaysToKeepFile=7
Root=(Type=KeyLength, Length=120, Inner=AsyncPut)
AsyncPut=(Type=AsyncPut, Inner=Hierarchy)
Hierarchy=(Type=Hierarchical, Inner=Boot, Inner=Pak, Inner=EnginePak, Inner=Local, Inner=Shared)
Boot=(Type=Boot, Filename="%GAMEDIR%DerivedDataCache/Boot.ddc", MaxCacheSize=512)
Local=(Type=FileSystem, ReadOnly=false, Clean=false, Flush=false, PurgeTransient=true, DeleteUnused=true, UnusedFileAge=34, FoldersToClean=-1, Path=%ENGINEDIR%DerivedDataCache, EnvPathOverride=UE-LocalDataCachePath, EditorOverrideSetting=LocalDerivedDataCache)
Shared=(Type=FileSystem, ReadOnly=false, Clean=false, Flush=false, DeleteUnused=true, UnusedFileAge=10, FoldersToClean=10, MaxFileChecksPerSec=1, Path=\\Slingshot\ddc, EnvPathOverride=UE-SharedDataCachePath, EditorOverrideSetting=SharedDerivedDataCache)
AltShared=(Type=FileSystem, ReadOnly=true, Clean=false, Flush=false, DeleteUnused=true, UnusedFileAge=23, FoldersToClean=10, MaxFileChecksPerSec=1, Path=\\Slingshot\ddc2, EnvPathOverride=UE-SharedDataCachePath2)
Pak=(Type=ReadPak, Filename="%GAMEDIR%DerivedDataCache/DDC.ddp")
EnginePak=(Type=ReadPak, Filename=%ENGINEDIR%DerivedDataCache/DDC.ddp)

Set up a machine on the network so that it has shared folders with everyone read/write access, and set the path in the DefaultEngine.ini as shown.

##Locally (remote work)
When working remotely, it doesn’t make sense to try and use a shared DDC, because downloading it through the VPN would be slower than generating it yourself locally. We will disable the shader DDC to prevent duplicating data in the following steps.
By default, the engine will set up a local DDC in Users\username\AppData\Local\UnrealEngine\Common. I prefer having it somewhere easy to find (and delete), but you can leave it there if you don’t mind that location.
Working from home, I suggest using the following environment variables, which specifies the location of the local DDC and has no shared DDC for all projects. You can leave out the local DDC one if you are fine with where it’s currently located.

Built In Data Types

##TArray
* [TArray](https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/TArrays/)
TArray: Arrays in Unreal Engine

A TArray is an array that is templated so that it can contain most any type, including UOBJECT based types.

Use a TArray when you care about the ordering of objects.
You can Add/Remove from the array using one function call, where remove will search the array with O(N) to find all elements that are that object and remove it.
You can sort the array using the sort function, and use a predicate if necessary.
You can access elements using [x] where x is the element number.

// Sort descending.
Scores.Sort([](const int& First, const int& Second) {return First > Second; });

## Engineers
Unreal Engine 4 extends C++ by using a tool called the Unreal Header tool, along with a reflection system. Read this first:
* [Reflection System and C++ Coding Standard](https://www.unrealengine.com/en-US/blog/unreal-property-system-reflection)

Follow Epic’s coding standard, which is found here:
* [Coding Standard | Unreal Engine Documentation](
https://docs.unrealengine.com/5.1/en-US/epic-cplusplus-coding-standard-for-unreal-engine/)
Resharper C++, which is a Visual Studio plugin, has Unreal Engine 4 features that help you to follow the standard.
* [Features - ReSharper C++](https://www.jetbrains.com/resharper-cpp/features/)
Perforce makes a Visual Studio plugin. This makes it much easier to check out files automatically while you are working, or use Rider's perforce integration.
* [Download Helix Plugin for Visual Studio (P4VS) | Perforce](https://www.perforce.com/downloads/helix-plugin-visual-studio-p4vs)
Use braces on separate lines for everything. This allows using breakpoints anywhere.
Where a class is large, use #pragma region and #pragma endregion to separate out functionality as necessary. For each region be sure to use public/private specifiers in case the region is moved.
Group UFUNCTIONS first and UPROPERTIES second after the private/public/protected specifiers.
In the main header file for a class, there should be a minimum of included header files to speed compilation when there are changes. Use ‘class’ to forward define class types that aren’t truly needed in the header file.
