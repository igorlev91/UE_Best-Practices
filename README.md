# UE_Best-Practices
 I wrote down some Unreal Engine best practices I've found in the past few years working in it professionally.

<a name="table-of-contents"></a>
## Table of Contents

> 1. [Setup](#Setup)
> 1. [Style Guide - Project Organization and Naming](#Style-Guide-Project-Organization-and-Naming)
> 1. [Derived Data Cache (DDC)](#DDC)
> 1. [Team Setup (while in office)](#Team-Setup)
>    [Locally](#Locally)
>    [Perforce](#Perforce)   
>   - [Rider](#Rider)
> [Visual Studio](#Visual-Studio)
> [Engineers](#Engineers)
> [Reflection System and C++ Coding Standard](#Reflection-System)
> [Built In Data Types](#Built-In-Data-Types)
> [TArray](#TArray)
> [TSet](#TSet)
> [TMap](#TMap)
> [Class Creation](#Class-Creation)
> [Polymorphic Components](#Polymorphic-Components)
> [UCLASS](#UCLASS)
> [HideCategories](#HideCategories)
> [UFUNCTION](#UFUNCTION)
> [BlueprintCallable](#BlueprintCallable)
> [BlueprintPure (Implies BlueprintCallable)](#BlueprintPure)
> [BlueprintGetter](#BlueprintGetter)
> [BlueprintImplementableEvent](#BlueprintImplementableEvent)
> [BlueprintNativeEvent](#BlueprintNativeEvent)

-
<a name="Setup"></a>
## Setup
Style Guide - Project Organization and Naming
It is imperative to set up the project properly and have everyone agree to use a naming and style guide. I suggest having everyone on the project read/understand this and follow it:

* [- GitHub - Allar/ue5-style-guide: An attempt to make Unreal Engine 4 projects more consistent](https://github.com/hollsteinm/ReasonablePlanningAI)

<a name="DDC"></a>
## DDC
This cache allows sharing of the cost to compile shaders and other operations. It’s a must when working in a team to prevent hours of waiting when working locally. It doesn’t make sense to use when working remotely because it would take longer to download than to generate, however you can specify the location of a local and shared DDC to prevent the warning messages from popping up.

Here’s Epic’s documentation:

* [Derived Data Cache](https://docs.unrealengine.com/5.1/en-US/derived-data-cache/)


<a name="Team-Setup"></a>
## Team-Setup

The project should be set up to use a shared cache. An example setup used in Slingshot (DefaultEngine.ini):

>[DerivedDataBackendGraph]
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

<a name="Locally"></a>
##Locally (remote work)
When working remotely, it doesn’t make sense to try and use a shared DDC, because downloading it through the VPN would be slower than generating it yourself locally. We will disable the shader DDC to prevent duplicating data in the following steps.
By default, the engine will set up a local DDC in Users\username\AppData\Local\UnrealEngine\Common. I prefer having it somewhere easy to find (and delete), but you can leave it there if you don’t mind that location.
Working from home, I suggest using the following environment variables, which specifies the location of the local DDC and has no shared DDC for all projects. You can leave out the local DDC one if you are fine with where it’s currently located.

Built In Data Types

<a name="TArray"></a>
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

<a name="Engineers"></a>
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

<a name="Class-Creation"></a>

ALWAYS CREATE A BASE CLASS IN C++.

Mark the class as final if it does not have any classes which inherit from it, this will speed up function calls of virtual functions, as the class can be "de-virtualized". final specifier (since C++11)

Constructors can contain NO RUNTIME CODE and should only be used for constructing components and setting default values in the class. Do NOT assign dynamic delegates in the constructor, these are unpredictable.

Disable tick and start with tick enable in the constructor of the class UNLESS YOU ABSOLUTELY HAVE TO USE TICK. Setting start with tick enabled to false will cause the BP to have “Start with Tick Enabled” off by default, which is a great way to prevent having a ton of BP’s ticking that aren’t doing anything.
For actors:
PrimaryActorTick.bCanEverTick = false;
PrimaryActorTick.bStartWithTickEnabled = false;
For components:
PrimaryComponentTick.bCanEverTick =  false;
PrimaryComponentTick.bStartWithTickEnabled = false;
If you have to use tick:
Set the tick interval to the maximum value you can get away with. Unfortunately this is often per frame for smoothly moving things.
PrimaryActorTick.TickInterval = 0.2f;
PrimaryComponentTick.TickInterval = 0.2f;
Enable/disable tick to only tick when required.
SetActorTickEnabled()
SetComponentTickEnabled()
Components and variables created in the C++ base class:
ALWAYS create components in C++. 
ALWAYS SetupAttachment() to connect them in the constructor the way you desire to other components, assuming they are based on a scene component (ActorComponents have no transform and cannot be attached of course). If you want to connect them to a socket in a skeletal mesh, setup attachment allows that as well if you put the name of the socket in the call. Note that this won’t happen until the object is registered (the socketing), so you can’t set relative location or rotation to that socket until BeginPlay() on the socketed object.
If you don't want the object to be attached to anything else during gameplay, it is still a good idea to attach it in the constructor and detach it in BeginPlay() or at some other time. Alternatively you can set it to use absolute location, rotation, and scale.
If you don't attach it to a parent object, you will get weird results occasionally (it will auto attach it to the root for you, etc), so it's best to create predictable behavior by attaching it and detaching it specifically.
Should be protected, components should not be publicly accessible unless there’s some real reason; consider using friend classes.
Give absolutely minimum access necessary to variables and functions from BP to make it clear how they are used. Don’t just make everything BlueprintReadWrite, it will be hard to know what’s being used in BPs if you want to refactor. See the UFUNCTION and UPROPERTY specifiers section.
For components, use:
UPROPERTY(VisibleAnywhere)
	USomeComponent* SomeComponent;
Pointers to other objects or variables that need to be set will be
UPROPERTY(EditDefaultsOnly)
float SomeFloat = 0.3f;
Variables that are set up in the class ALWAYS NEED A DEFAULT VALUE UNLESS TRANSIENT.
float SomeFloat = 0.0f;
int SomeInt = 0;
bool bSomeBool = false;
Keep in mind that variables that are set in an inherited BP will not be accessible in the constructor. They will not be set until BeginPlay(). 
Variables that point to UOBJECTS such as an actor reference, should use UPROPERTY() to prevent garbage collection. Otherwise, the engine can garbage collect the pointer at any time, invalidating the reference. 

* [Unreal Object Handling | Unreal Engine Documentation](https://docs.unrealengine.com/5.1/en-US/objects-in-unreal-engine/)

UPROPERTY(Transient)
UMaterialInstanceDynamic* DynamicMaterial;

Don’t spawn components conditionally or delete components in the constructor if you want the editor to visualize them properly. UE4 runs the constructor once to create a class default object and doesn’t run the constructor on actors that have been saved into levels once it has done this. Objects that are selected in the editor, if you have deleted components, will flicker in their description pane.
