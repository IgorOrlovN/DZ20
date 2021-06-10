// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 100.f;
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
	int32 ElemIndex = SnakeElements.Add(NewSnakeElem);
	if (ElemIndex == 0)
	{
		NewSnakeElem->SetFirstElementType();
	}
	}
}

void ASnakeBase::Move()
{
	FVector MovementVector(ForceInitToZero);
	MovementSpeed = ElementSize;

	switch (LastMoveDirection)
	{
	case EMovementDerection::UP:
		MovementVector.X += MovementSpeed;
		break;
	case EMovementDerection::DOWN:
		MovementVector.X -= MovementSpeed;
		break;
	case EMovementDerection::LEFT:
		MovementVector.Y += MovementSpeed;
		break;
	case EMovementDerection::RIGHT:
		MovementVector.Y -= MovementSpeed;
		break;
	}

	//AddActorWorldOffset(MovementVector);
	for(int i = SnakeElements.Num()-1; i>0; i--)
		{
		auto CurrentElement = SnakeElements[i];
		auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurrentElement->SetActorLocation(PrevLocation);
		}

	SnakeElements[0]->AddActorWorldOffset(MovementVector);
}

