# UE_Best-Practices
 I wrote down some Unreal Engine best practices I've found in the past few years working in it professionally.

<a name="table-of-contents"></a>
## Table of Contents

> 1. [Setup](#Setup)
> 1. [Style Guide - Project Organization and Naming](#Style-Guide-Project-Organization-and-Naming)
> 1. [Derived Data Cache (DDC)](#DDC)
> 1. [Team Setup (while in office)](#Team-Setup)
> 1. [Locally](#Locally (remote work))
> 1. [Perforce](#Perforce)   
> 1. [Rider](#Rider)
> 1. [Visual Studio](#Visual-Studio)
> 1. [Engineers](#Engineers)
> 1. [Reflection System and C++ Coding Standard](#Reflection-System)
> 1. [Built In Data Types](#Built-In-Data-Types)
> 1. [TArray](#TArray)
> 1. [TSet](#TSet)
> 1. [TMap](#TMap)
> 1. [Class Creation](#Class-Creation)
> 1. [Polymorphic Components](#Polymorphic-Components)
> 1. [UCLASS](#UCLASS)
> 1. [HideCategories](#HideCategories)
> 2. [UFUNCTION](#UFUNCTION)
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 2.1. [BlueprintCallable](#BlueprintCallable)
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 2.2. [BlueprintPure (Implies BlueprintCallable)](#BlueprintPure)
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 2.3. [BlueprintGetter](#BlueprintGetter)
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 2.4. [BlueprintImplementableEvent](#BlueprintImplementableEvent)
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 2.5. [BlueprintNativeEvent](#BlueprintNativeEvent)
> 2. [Multiplayer](#UE4-Multiplayer)
> 2. [Optimizing Performance](#Optimizing-Performance)

-
<a name="Setup"></a>
## Setup
Style Guide - Project Organization and Naming
It is imperative to set up the project properly and have everyone agree to use a naming and style guide. I suggest having everyone on the project read/understand this and follow it:

* [- GitHub - Allar/ue5-style-guide: An attempt to make Unreal Engine 4 projects more consistent](https://github.com/hollsteinm/ReasonablePlanningAI)

<a name="DDC"></a>
## Derived Data Cache
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

<a name="Locally (remote work)"></a>
## Locally (remote work)
* When working remotely, it doesn’t make sense to try and use a shared DDC, because downloading it through the VPN would be slower than generating it yourself locally. We will disable the shader DDC to prevent duplicating data in the following steps.
* By default, the engine will set up a local DDC in Users\username\AppData\Local\UnrealEngine\Common. I prefer having it somewhere easy to find (and delete), but you can leave it there if you don’t mind that location.
* Working from home, I suggest using the following environment variables, which specifies the location of the local DDC and has no shared DDC for all projects. You can leave out the local DDC one if you are fine with where it’s currently located.

<a name="Rider"></a>
## Rider
There is a new IDE that can be used to work in Unreal Engine, Rider!

Rider for Unreal Engine

It has built-in Perforce support, and already has the ReSharper C++ functionality. In my experience it is much faster than Visual Studio and has many Unreal Engine features. Highly recommended and you can get a free trial!


<a name="Visual-Studio"></a>
## Visual Studio
If you use Visual Studio, changing a few settings is imperative:

* [Setting Up Visual Studio for Unreal Engine](https://docs.unrealengine.com/5.1/en-US/setting-up-visual-studio-development-environment-for-cplusplus-projects-in-unreal-engine/)

Using the Perforce plugin for Visual Studio makes it much faster to check out files:

* [Download Helix Plugin for Visual Studio (P4VS)](https://www.perforce.com/downloads/helix-plugin-visual-studio-p4vs)

Resharper C++ is a plugin for Visual Studio which has Unreal Engine 4 functionality built into it. It makes it much easier to follow Epic’s coding standard, as it highlights many things that do not fit it. Additionally it speeds creating function prototypes, describes macros (UFUNCTION, UPROPERTY), and replaces intellisense.

* [ReSharper C++ : The Visual Studio Extension for C++ Development](https://www.jetbrains.com/resharper-cpp/)


## Built In Data Types

<a name="TArray"></a>
## TArray

* [TArray: Arrays in Unreal Engine](https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/TArrays/)

A TArray is an array that is templated so that it can contain most any type, including UOBJECT based types.

* Use a TArray when you care about the ordering of objects.
* You can Add/Remove from the array using one function call, where remove will search the array with O(N) to find all elements that are that object and remove it.
* You can sort the array using the sort function, and use a predicate if necessary.
*  You can access elements using [x] where x is the element number.

```c++
// Sort descending.
Scores.Sort([](const int& First, const int& Second) {return First > Second; });
```

<a name="TSet"></a>
## TSet
* [TSet | Unreal Engine 4.27 Documentation](https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/TSet/)

A TSet is a number of homogeneous objects that are referenced by the hash produced by the object itself. By default it only allows ONE of each object.

Use TSet when you don't care about order, and instead care about the speed of finding and removing objects.

* Add/Remove uses a hash lookup and is much faster than TArray.
* You can create your own hash using GetTypeHash() and HashCombine() if necessary.
* You can't access objects using [x], you must use an iterator. The point of the TSet is not to care about order.
* If you want to get an element from the TSet, you have to use an iterator; you could conceivably bail after getting the "first" element, which only means that it happens to be the first in the set. This order can change any time.


<a name="TMap"></a>
## TMap
* [TMap | Unreal Engine 4.27 Documentation](https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/TMap/)

A TMap is similar to a TSet, except that the key value, that is to say the value that you use to look up the object, can be specified.

* Use TMap when you don't care about order, care about speed in look up, and need to map some kind of key to a value.
* For example, you might want to map FName to UDataAsset, looking up data assets by their name.
* You could also map a uint32 Hash to an object in a map, and generate the hash manually by what is in the object that uniquely identifies it.


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
* Use braces on separate lines for everything. This allows using breakpoints anywhere.
* Where a class is large, use #pragma region and #pragma endregion to separate out functionality as necessary. For each region be sure to use public/private specifiers in case the region is moved.
* Group UFUNCTIONS first and UPROPERTIES second after the private/public/protected specifiers.
* In the main header file for a class, there should be a minimum of included header files to speed compilation when there are changes. Use ‘class’ to forward define class types that aren’t truly needed in the header file.

<a name="Class-Creation"></a>
## Class Creation

* ALWAYS CREATE A BASE CLASS IN C++.

* Mark the class as final if it does not have any classes which inherit from it, this will speed up function calls of virtual functions, as the class can be "de-virtualized". final specifier (since C++11)

* Constructors can contain NO RUNTIME CODE and should only be used for constructing components and setting default values in the class. Do NOT assign dynamic delegates in the constructor, these are unpredictable.

* Disable tick and start with tick enable in the constructor of the class UNLESS YOU ABSOLUTELY HAVE TO USE TICK. Setting start with tick enabled to false will cause the BP to have “Start with Tick Enabled” off by default, which is a great way to prevent having a ton of BP’s ticking that aren’t doing anything.
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

* Don’t spawn components conditionally or delete components in the constructor if you want the editor to visualize them properly. UE4 runs the constructor once to create a class default object and doesn’t run the constructor on actors that have been saved into levels once it has done this. Objects that are selected in the editor, if you have deleted components, will flicker in their description pane.

<a name="#Polymorphic-Components"></a>
## Polymorphic Components

If you want to create a component in a base class, and then have child classes override that class, it can be done using the object initializer.

* Create your new base class.
* Change the constructor that was automatically generated to include a FOBjectInitializer argument in the header and C++ file.

```c++
AMeleeWeaponActor(const class FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
```

Use that constructor to create the base version of the component.

```c++
AMeleeWeaponActor::AMeleeWeaponActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	AttackColliderComponent = CreateDefaultSubobject<UShapeComponent>(TEXT("AttackCollider"));
}

```

Now create your subclass of the actor:

```c++
AMeleeWeaponActor::AMeleeWeaponActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	AttackColliderComponent = CreateDefaultSubobject<UShapeComponent>(TEXT("AttackCollider"));
}

```


In the constructor of this child actor, use the initializer to specify the class for the component.
```c++
AShovelActor::AShovelActor(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UBoxComponent>(TEXT("AttackCollider")))
{
	UBoxComponent* Box = CastChecked<UBoxComponent>(AttackColliderComponent);

	Box->SetupAttachment(GetRootComponent());
	Box->SetCollisionProfileName(Weapon_ProfileName);
	Box->SetGenerateOverlapEvents(false);
	Box->CanCharacterStepUpOn = ECB_No;
}

```

* At this point, the original class will have the shape component, and since a collision box is a subclass of shape, the new class will have a box component. This way you can make multiple subclasses, and each could have a different collision shape (box, capsule, sphere) if you desired.
Good places to look at this in the engine code is ACharacter and ATriggerBase if you want to see examples.

<a name="UFUNCTION"></a>
## UFUNCTION
## BlueprintCallable
Allows a function to be called from BP in general. If the function is setting/getting a variable or doesn’t change the underlying class, use the more specific specifiers below.
```c++
UFUNCTION(BlueprintCallable)
void TriggerSomething();
```
<a name="BlueprintPure"></a>
## BlueprintPure (Implies BlueprintCallable)
Allows a function to be called from BP that doesn’t change the underlying class. If the function is just setting a variable in the underlying class, use BlueprintSetter.
```c++
UFUNCTION(BlueprintPure, Category = "Point Cloud")
TArray<FVector> GetPointsInBox(const FBox& WorldSpaceBox) const;
BlueprintGetter (Implies BlueprintPure and BlueprintCallable) and BlueprintSetter (Implies BlueprintCallable)
Example UPROPERTY:

UPROPERTY(EditAnywhere, BlueprintGetter=GetDisablePostProcessBlueprint, BlueprintSetter=SetDisablePostProcessBlueprint, Category = Animation)
uint8 bDisablePostProcessBlueprint:1;
```

BlueprintGetter is intended for getting a variable from the underlying class:
```c++
UFUNCTION(BlueprintGetter)
bool GetDisablePostProcessBlueprint() const { return bDisablePostProcessBlueprint; }
```

BlueprintSetter is intended for setting a variable in the underlying class:


```c++

UFUNCTION(BlueprintSetter)
void SetDisablePostProcessBlueprint(bool bInDisablePostProcess)
{
	if(!bInDisablePostProcess && bDisablePostProcessBlueprint && PostProcessAnimInstance)
	{
		PostProcessAnimInstance->InitializeAnimation();
	}

	bDisablePostProcessBlueprint = bInDisablePostProcess;
}
```
<a name="BlueprintImplementableEvent"></a>

Defines a function that is overridden in BP, and has no native implementation. See BlueprintNativeEvent to add a native implementation.

```c++

UFUNCTION(BlueprintImplementableEvent)
void PlayerIsHealthy(float Health);
```

In your class Blueprint, right click and type the name of your function to create an event that is the function override.
<a name="BlueprintNativeEvent"></a>
## BlueprintNativeEvent
Define a function that is intended to be overridden in BP, but also has a native implementation specified with the _Implementation label.

```c++
UFUNCTION(BlueprintNativeEvent)
UEnum* GetAnimationStateEnum();
virtual UEnum* GetAnimationStateEnum_Implementation()
{
	return GetAnimationStateEnum_Internal();
}
```
Passing Structs
In order for these to compile and to show up in Blueprints properly, you must pass the structure as a const reference, as seen below.


```c++
// Calls the SetHandPose() function on the animation blueprint.
UFUNCTION(BlueprintNativeEvent)
void SetHandPose(const FPoseSet& Poses, bool bImmediate);
void SetHandPose_Implementation(const FPoseSet& Poses, bool bImmediate);

USTRUCT(BlueprintType)
struct FPoseSet
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UAnimSequence* FemaleLeftPose;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UAnimSequence* FemaleRightPose;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UAnimSequence* MaleLeftPose;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UAnimSequence* MaleRightPose;
};
```

Structs as Function Inputs
Use UPARAM(ref):

```c++
UFUNCTION(BlueprintCallable, Category = "Example Nodes")
static void HandleTargets(UPARAM(ref) TArray<FVector>& InputLocations, TArray<FVector>& OutputLocations);
```

CallInEditor
Allows the function to be called in the editor when selecting the object in the level. This can be done while running the game or not. Very useful for testing functionality; a button appears that is the name of the function in the actor’s details pane.
Exec
Allows executing the function from the console.
