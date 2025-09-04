/*
 * UE4 SDK Implementation - Main SDK class
 * Self-contained, vibe coder style implementation
 */

#include "UE4SDK.h"
#include "Core/Types.h"
#include "Core/Utils.h"
#include "Core/UObject.h"
#include "Core/AI.h"
#include "Core/Navigation.h"
#include "Core/Blackboard.h"
#include "Core/Replication.h"

namespace UE4SDK
{
    SDK& SDK::Get()
    {
        static SDK Instance;
        return Instance;
    }
    
    bool SDK::Initialize()
    {
        UE_LOG(LogUE4SDK, Log, TEXT("Initializing UE4 SDK v%s"), TEXT(UE4_SDK_VERSION));
        
        try
        {
            // Initialize utils first
            m_Utils = std::make_unique<Utils>();
            m_Utils->Initialize();
            
            // Initialize engine
            m_Engine = std::make_unique<UEngine>();
            m_Engine->Initialize();
            
            UE_LOG(LogUE4SDK, Log, TEXT("UE4 SDK initialized successfully"));
            return true;
        }
        catch (const std::exception& e)
        {
            UE_LOG(LogUE4SDK, Error, TEXT("Failed to initialize UE4 SDK: %s"), e.what());
            return false;
        }
    }
    
    void SDK::Shutdown()
    {
        UE_LOG(LogUE4SDK, Log, TEXT("Shutting down UE4 SDK"));
        
        if (m_Engine)
        {
            m_Engine->Shutdown();
            m_Engine.reset();
        }
        
        if (m_Utils)
        {
            m_Utils->Shutdown();
            m_Utils.reset();
        }
        
        UE_LOG(LogUE4SDK, Log, TEXT("UE4 SDK shutdown complete"));
    }
    
    UEngine& SDK::GetEngine()
    {
        if (!m_Engine)
        {
            UE_LOG(LogUE4SDK, Error, TEXT("Engine not initialized"));
            static UEngine DummyEngine;
            return DummyEngine;
        }
        return *m_Engine;
    }
    
    UWorld& SDK::GetWorld()
    {
        return GetEngine().GetWorld();
    }
    
    Utils& SDK::GetUtils()
    {
        if (!m_Utils)
        {
            UE_LOG(LogUE4SDK, Error, TEXT("Utils not initialized"));
            static Utils DummyUtils;
            return DummyUtils;
        }
        return *m_Utils;
    }
}
