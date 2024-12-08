// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Shootercharacter.h"

UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return  EBTNodeResult::Failed;
	APawn* Pawn = AIController->GetPawn();

	AShootercharacter* Shooter = Cast<AShootercharacter>(Pawn);
	if (Shooter == nullptr)
		return EBTNodeResult::Failed;
	Shooter->Shot();
	return EBTNodeResult::Succeeded;
}
