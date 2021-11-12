// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM()
enum class EnemyState : uint8
{
	INPATROL = 0	UMETA(DisplayName = "Enemy patroling"),
	INPOSITION = 1	UMETA(DisplayName = "Enemy in position"),
	INFIGHT = 2		UMETA(DisplayName = "Enemy fighting"),
};