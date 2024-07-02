#include "ChoppingBoard.h"
#include "Grabbable.h"
#include "RecipeProvider.h"
#include "GameFramework/GameModeBase.h"

ULockPoint* AChoppingBoard::GetLockPoint()
{
	return GetComponentByClass<ULockPoint>();
}

// Sets default values
AChoppingBoard::AChoppingBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(root);
	root->SetMobility(EComponentMobility::Static);
	leftBread = CreateDefaultSubobject<USceneComponent>(TEXT("Left Bread Spawn"));
	leftBread->SetMobility(EComponentMobility::Static);
	leftBread->SetupAttachment(root);
	rightBread = CreateDefaultSubobject<USceneComponent>(TEXT("Right Bread Spawn"));
	rightBread->SetMobility(EComponentMobility::Static);
	rightBread->SetupAttachment(root);
}

// Called when the game starts or when spawned
void AChoppingBoard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChoppingBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AChoppingBoard::CanChop(const AActor* target, FChopResult& result)
{
	//Get chop recipes
	auto* gamemode = GetWorld()->GetAuthGameMode();
	if (!gamemode->GetClass()->ImplementsInterface(URecipeProvider::StaticClass())) return false;
	UChopRecipes* chopRecipes = IRecipeProvider::Execute_GetChopRecipes(gamemode);
	//Check
	auto* value = chopRecipes->ChopRecipes.Find(target->GetClass());
	if (!value) return false;
	result = *value;
	return true;
}

void AChoppingBoard::TryChop()
{
	auto* lockPoint = GetLockPoint();
	if (!lockPoint->InUse) return;
	auto* chopTarget = lockPoint->LockedItem->GetOwner();
	FChopResult result;
	if (!CanChop(chopTarget, result)) return;
	FTransform transform = chopTarget->GetActorTransform();
	lockPoint->UnlockItem();
	chopTarget->Destroy();
	FActorSpawnParameters parameters;
	parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	parameters.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	if (result.Type == EChopResultType::Single) {
		transform.SetScale3D({ 1, 1, 1 });
		auto* actor = GetWorld()->SpawnActor(result.ResultClass.Get(), &transform, parameters);
		if (auto* grabbable = actor->GetComponentByClass<UGrabbable>()) {
			lockPoint->LockItem(grabbable);
		}
	}
	else if (result.Type == EChopResultType::Double) {
		FTransform transformLeft = leftBread->GetComponentTransform();
		FTransform transformRight = rightBread->GetComponentTransform();
		transformLeft.SetScale3D({ 1, 1, 1 });
		transformRight.SetScale3D({ 1, 1, 1 });
		GetWorld()->SpawnActor(result.ResultClass.Get(), &transformLeft, parameters);
		GetWorld()->SpawnActor(result.ResultClass.Get(), &transformRight, parameters);
	}
	OnChop();
}
