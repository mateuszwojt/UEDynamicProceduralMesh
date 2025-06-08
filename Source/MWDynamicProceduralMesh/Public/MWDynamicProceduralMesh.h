// Copyright Mateusz Wojt. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FMWDynamicProceduralMeshModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
