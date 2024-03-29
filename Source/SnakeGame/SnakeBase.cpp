// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include "interactable.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 10.f;
	MovementSpeed = 10.f;
	LastMoveDirection = EMovementDerection::DOWN;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(MovementSpeed);
	AddSnakeElement(4);
	//GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, GetActorTransform());
	
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();

}

void ASnakeBase::AddSnakeElement(int ElementsNum)
{
	//SnakeElements.Num()*ElementSize;
	for (int i = 0; i < ElementsNum; ++i)
	{
	FVector NewLocation(SnakeElements.Num() * ElementSize, 0, 0);
	FTransform NewTransform(NewLocation);
	ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
	NewSnakeElem->SnakeOwner = this;
	int32 ElemIndex = SnakeElements.Add(NewSnakeElem);
	if (ElemIndex == 0)
	{
		NewSnakeElem->SetFirstElementType();
		//NewSnakeElem->MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ASnakeElementBase::HandleBeginOverlap);
	}
	}
}

void ASnakeBase::Move()
{
	FVector MovementVector(ForceInitToZero);

	switch (LastMoveDirection)
	{
	case EMovementDerection::UP:
		MovementVector.X += ElementSize;
		break;
	case EMovementDerection::DOWN:
		MovementVector.X -= ElementSize;
		break;
	case EMovementDerection::LEFT:
		MovementVector.Y += ElementSize;
		break;
	case EMovementDerection::RIGHT:
		MovementVector.Y -= ElementSize;
		break;
	}

	//AddActorWorldOffset(MovementVector);

	SnakeElements[0]->ToggleCollision();

	for(int i = SnakeElements.Num()-1; i>0; i--)
		{
		auto CurrentElement = SnakeElements[i];
		auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurrentElement->SetActorLocation(PrevLocation);
		}

	SnakeElements[0]->AddActorWorldOffset(MovementVector);
	SnakeElements[0]->ToggleCollision();

}

void ASnakeBase::SnakeElementOverlap(ASnakeElementBase* OverlappedElement, AActor* Other)
{
	if (IsValid(OverlappedElement))
	{
		int32 ElemIndex;
		SnakeElements.Find(OverlappedElement, ElemIndex);
		bool bIsFirst = ElemIndex == 0;
		Iinteractable* interactableInterface = Cast<Iinteractable>(Other);
		if (interactableInterface)
		{
			interactableInterface->Interact(this, bIsFirst);
		}
	}
}
