/*
 * UE4 Replication Implementation - Self-contained Replication system implementation
 * Vibe coder approved ✨
 */

#include "Replication.h"
#include "Utils.h"
#include <algorithm>

namespace UE4SDK
{
    // ReplicationInfo implementation
    ReplicationInfo::ReplicationInfo()
        : m_ReplicationType(EReplicationType::Unreliable)
        , m_ReplicationCondition(EReplicationCondition::None)
        , m_NetRole(ENetRole::SimulatedProxy)
        , m_NetMode(ENetMode::Standalone)
        , m_bReplicated(false)
        , m_bDirty(false)
        , m_bInitialReplication(false)
        , m_ReplicationFrequency(1.0f)
        , m_LastReplicationTime(0.0f)
        , m_ReplicationSize(0)
    {
    }

    bool ReplicationInfo::IsValidForReplication() const
    {
        return m_bReplicated && m_ReplicationType != EReplicationType::None;
    }

    bool ReplicationInfo::ShouldReplicate(float CurrentTime) const
    {
        if (!IsValidForReplication())
        {
            return false;
        }
        
        if (m_bDirty)
        {
            return true;
        }
        
        if (CurrentTime - m_LastReplicationTime >= (1.0f / m_ReplicationFrequency))
        {
            return true;
        }
        
        return false;
    }

    bool ReplicationInfo::CanReplicateTo(ENetRole TargetRole) const
    {
        switch (m_ReplicationCondition)
        {
            case EReplicationCondition::None:
                return true;
            case EReplicationCondition::InitialOnly:
                return m_bInitialReplication;
            case EReplicationCondition::OwnerOnly:
                return TargetRole == ENetRole::Authority;
            case EReplicationCondition::SkipOwner:
                return TargetRole != ENetRole::Authority;
            case EReplicationCondition::SimulatedOnly:
                return TargetRole == ENetRole::SimulatedProxy;
            case EReplicationCondition::AutonomousOnly:
                return TargetRole == ENetRole::AutonomousProxy;
            case EReplicationCondition::SimulatedOrPhysics:
                return TargetRole == ENetRole::SimulatedProxy || TargetRole == ENetRole::Authority;
            case EReplicationCondition::InitialOrOwner:
                return m_bInitialReplication || TargetRole == ENetRole::Authority;
            case EReplicationCondition::Custom:
                return true; // Custom logic would be implemented by the user
            default:
                return false;
        }
    }

    void ReplicationInfo::MarkDirty()
    {
        m_bDirty = true;
    }

    void ReplicationInfo::ClearDirty()
    {
        m_bDirty = false;
    }

    void ReplicationInfo::ResetReplication()
    {
        m_bReplicated = false;
        m_bDirty = false;
        m_bInitialReplication = false;
        m_LastReplicationTime = 0.0f;
        m_ReplicationData = FString();
        m_ReplicationSize = 0;
    }

    // ReplicationManager implementation
    ReplicationManager::ReplicationManager()
        : m_GlobalReplicationFrequency(1.0f)
        , m_MaxReplicationSize(1024)
        , m_bReplicationEnabled(true)
    {
    }

    void ReplicationManager::RegisterObject(UObject* Object, ReplicationInfo* ReplicationInfo)
    {
        if (!Object || !ReplicationInfo)
        {
            Utils::Get().LogError(TEXT("ReplicationManager::RegisterObject - Invalid object or replication info"));
            return;
        }
        
        m_ReplicationInfo.Add(Object, ReplicationInfo);
        m_ReplicatedObjects.Add(Object);
        
        Utils::Get().LogInfo(TEXT("Object registered for replication: ") + Object->GetName());
    }

    void ReplicationManager::UnregisterObject(UObject* Object)
    {
        if (!Object)
        {
            return;
        }
        
        m_ReplicationInfo.Remove(Object);
        m_ReplicatedObjects.Remove(Object);
        RemoveFromDirtyList(Object);
        
        Utils::Get().LogInfo(TEXT("Object unregistered from replication: ") + Object->GetName());
    }

    bool ReplicationManager::IsObjectRegistered(UObject* Object) const
    {
        return m_ReplicationInfo.Contains(Object);
    }

    ReplicationInfo* ReplicationManager::GetReplicationInfo(UObject* Object) const
    {
        auto* Info = m_ReplicationInfo.Find(Object);
        return Info ? *Info : nullptr;
    }

    void ReplicationManager::ProcessReplication(float DeltaTime)
    {
        if (!m_bReplicationEnabled)
        {
            return;
        }
        
        float CurrentTime = Utils::Get().GetTimeSinceStart();
        
        for (UObject* Object : m_ReplicatedObjects)
        {
            if (ShouldReplicateObject(Object, CurrentTime))
            {
                ProcessObjectReplication(Object);
            }
        }
    }

    void ReplicationManager::ReplicateObject(UObject* Object)
    {
        if (!Object || !IsObjectRegistered(Object))
        {
            return;
        }
        
        ProcessObjectReplication(Object);
    }

    void ReplicationManager::ReplicateAllObjects()
    {
        for (UObject* Object : m_ReplicatedObjects)
        {
            ProcessObjectReplication(Object);
        }
    }

    void ReplicationManager::ReplicateDirtyObjects()
    {
        for (UObject* Object : m_DirtyObjects)
        {
            ProcessObjectReplication(Object);
        }
    }

    TArray<UObject*> ReplicationManager::GetReplicatedObjects() const
    {
        return m_ReplicatedObjects;
    }

    TArray<UObject*> ReplicationManager::GetDirtyObjects() const
    {
        return m_DirtyObjects;
    }

    TArray<UObject*> ReplicationManager::GetObjectsForRole(ENetRole Role) const
    {
        TArray<UObject*> Objects;
        for (UObject* Object : m_ReplicatedObjects)
        {
            ReplicationInfo* Info = GetReplicationInfo(Object);
            if (Info && Info->GetNetRole() == Role)
            {
                Objects.Add(Object);
            }
        }
        return Objects;
    }

    TArray<UObject*> ReplicationManager::GetObjectsForCondition(EReplicationCondition Condition) const
    {
        TArray<UObject*> Objects;
        for (UObject* Object : m_ReplicatedObjects)
        {
            ReplicationInfo* Info = GetReplicationInfo(Object);
            if (Info && Info->GetReplicationCondition() == Condition)
            {
                Objects.Add(Object);
            }
        }
        return Objects;
    }

    int32 ReplicationManager::GetReplicatedObjectCount() const
    {
        return static_cast<int32>(m_ReplicatedObjects.Num());
    }

    int32 ReplicationManager::GetDirtyObjectCount() const
    {
        return static_cast<int32>(m_DirtyObjects.Num());
    }

    int32 ReplicationManager::GetTotalReplicationSize() const
    {
        int32 TotalSize = 0;
        for (const auto& pair : m_ReplicationInfo)
        {
            TotalSize += pair.second->GetReplicationSize();
        }
        return TotalSize;
    }

    float ReplicationManager::GetAverageReplicationFrequency() const
    {
        if (m_ReplicationInfo.Num() == 0)
        {
            return 0.0f;
        }
        
        float TotalFrequency = 0.0f;
        for (const auto& pair : m_ReplicationInfo)
        {
            TotalFrequency += pair.second->GetReplicationFrequency();
        }
        
        return TotalFrequency / static_cast<float>(m_ReplicationInfo.Num());
    }

    bool ReplicationManager::ValidateReplication(UObject* Object) const
    {
        if (!Object)
        {
            return false;
        }
        
        ReplicationInfo* Info = GetReplicationInfo(Object);
        if (!Info)
        {
            return false;
        }
        
        return Info->IsValidForReplication();
    }

    TArray<FString> ReplicationManager::ValidateAllReplication() const
    {
        TArray<FString> Errors;
        
        for (const auto& pair : m_ReplicationInfo)
        {
            UObject* Object = pair.first;
            ReplicationInfo* Info = pair.second;
            
            if (!Info->IsValidForReplication())
            {
                Errors.Add(TEXT("Invalid replication info for object: ") + Object->GetName());
            }
            
            if (Info->GetReplicationSize() > m_MaxReplicationSize)
            {
                Errors.Add(TEXT("Replication size exceeds limit for object: ") + Object->GetName());
            }
        }
        
        return Errors;
    }

    void ReplicationManager::CleanupReplication()
    {
        m_ReplicationInfo.Empty();
        m_ReplicatedObjects.Empty();
        m_DirtyObjects.Empty();
    }

    void ReplicationManager::ResetReplication()
    {
        for (const auto& pair : m_ReplicationInfo)
        {
            pair.second->ResetReplication();
        }
        
        m_DirtyObjects.Empty();
    }

    void ReplicationManager::AddToDirtyList(UObject* Object)
    {
        if (!m_DirtyObjects.Contains(Object))
        {
            m_DirtyObjects.Add(Object);
        }
    }

    void ReplicationManager::RemoveFromDirtyList(UObject* Object)
    {
        m_DirtyObjects.Remove(Object);
    }

    bool ReplicationManager::ShouldReplicateObject(UObject* Object, float CurrentTime) const
    {
        ReplicationInfo* Info = GetReplicationInfo(Object);
        if (!Info)
        {
            return false;
        }
        
        return Info->ShouldReplicate(CurrentTime);
    }

    void ReplicationManager::ProcessObjectReplication(UObject* Object)
    {
        ReplicationInfo* Info = GetReplicationInfo(Object);
        if (!Info)
        {
            return;
        }
        
        // Simulate replication data
        FString ReplicationData = TEXT("ReplicationData:") + Object->GetName();
        Info->SetReplicationData(ReplicationData);
        Info->SetReplicationSize(static_cast<int32>(ReplicationData.Len()));
        Info->SetLastReplicationTime(Utils::Get().GetTimeSinceStart());
        Info->ClearDirty();
        
        // Call replication callback
        if (m_OnObjectReplicatedCallback)
        {
            m_OnObjectReplicatedCallback(Object);
        }
        
        Utils::Get().LogDebug(TEXT("Object replicated: ") + Object->GetName());
    }

    // NetworkManager implementation
    NetworkManager::NetworkManager()
        : m_ServerAddress(TEXT("127.0.0.1"))
        , m_ServerPort(7777)
        , m_MaxConnections(100)
        , m_NetMode(ENetMode::Standalone)
        , m_bIsServer(false)
        , m_bIsClient(false)
        , m_TotalBytesSent(0)
        , m_TotalBytesReceived(0)
    {
    }

    void NetworkManager::AddConnection(int32 ConnectionID, const FString& Address, int32 Port)
    {
        if (m_Connections.Num() >= m_MaxConnections)
        {
            Utils::Get().LogWarning(TEXT("NetworkManager::AddConnection - Max connections reached"));
            return;
        }
        
        FString ConnectionInfo = Address + TEXT(":") + FString::FromInt(Port);
        m_Connections.Add(ConnectionID, ConnectionInfo);
        
        if (m_OnConnectionCallback)
        {
            m_OnConnectionCallback(ConnectionID);
        }
        
        Utils::Get().LogInfo(TEXT("Connection added: ") + FString::FromInt(ConnectionID) + TEXT(" - ") + ConnectionInfo);
    }

    void NetworkManager::RemoveConnection(int32 ConnectionID)
    {
        if (m_Connections.Contains(ConnectionID))
        {
            m_Connections.Remove(ConnectionID);
            
            if (m_OnDisconnectionCallback)
            {
                m_OnDisconnectionCallback(ConnectionID);
            }
            
            Utils::Get().LogInfo(TEXT("Connection removed: ") + FString::FromInt(ConnectionID));
        }
    }

    bool NetworkManager::HasConnection(int32 ConnectionID) const
    {
        return m_Connections.Contains(ConnectionID);
    }

    TArray<int32> NetworkManager::GetAllConnections() const
    {
        TArray<int32> Connections;
        for (const auto& pair : m_Connections)
        {
            Connections.Add(pair.first);
        }
        return Connections;
    }

    void NetworkManager::SendData(int32 ConnectionID, const FString& Data)
    {
        if (!HasConnection(ConnectionID))
        {
            Utils::Get().LogWarning(TEXT("NetworkManager::SendData - Connection not found: ") + FString::FromInt(ConnectionID));
            return;
        }
        
        m_TotalBytesSent += static_cast<int32>(Data.Len());
        Utils::Get().LogDebug(TEXT("Data sent to connection ") + FString::FromInt(ConnectionID) + TEXT(": ") + Data);
    }

    void NetworkManager::SendDataToAll(const FString& Data)
    {
        for (const auto& pair : m_Connections)
        {
            SendData(pair.first, Data);
        }
    }

    void NetworkManager::SendDataToAllExcept(int32 ExcludeConnectionID, const FString& Data)
    {
        for (const auto& pair : m_Connections)
        {
            if (pair.first != ExcludeConnectionID)
            {
                SendData(pair.first, Data);
            }
        }
    }

    void NetworkManager::BroadcastData(const FString& Data)
    {
        SendDataToAll(Data);
    }

    int32 NetworkManager::GetConnectionCount() const
    {
        return static_cast<int32>(m_Connections.Num());
    }

    int32 NetworkManager::GetTotalBytesSent() const
    {
        return m_TotalBytesSent;
    }

    int32 NetworkManager::GetTotalBytesReceived() const
    {
        return m_TotalBytesReceived;
    }

    float NetworkManager::GetNetworkLatency(int32 ConnectionID) const
    {
        // Simulate network latency
        return Utils::Get().RandomFloat(0.01f, 0.1f);
    }

    bool NetworkManager::ValidateConnection(int32 ConnectionID) const
    {
        return HasConnection(ConnectionID);
    }

    bool NetworkManager::ValidateNetworkSettings() const
    {
        if (m_ServerPort < 1024 || m_ServerPort > 65535)
        {
            return false;
        }
        
        if (m_MaxConnections <= 0)
        {
            return false;
        }
        
        return true;
    }

    void NetworkManager::DisconnectAll()
    {
        TArray<int32> Connections = GetAllConnections();
        for (int32 ConnectionID : Connections)
        {
            RemoveConnection(ConnectionID);
        }
    }

    void NetworkManager::ShutdownNetwork()
    {
        DisconnectAll();
        m_Connections.Empty();
        m_TotalBytesSent = 0;
        m_TotalBytesReceived = 0;
    }

    // ServerReplication implementation
    ServerReplication::ServerReplication()
        : m_ReplicationManager(nullptr)
        , m_NetworkManager(nullptr)
        , m_ReplicationTickRate(60.0f)
        , m_MaxReplicationPerTick(100)
        , m_ReplicationBudget(1.0f)
        , m_ReplicatedActorsCount(0)
        , m_TotalReplicationSize(0)
        , m_AverageReplicationTime(0.0f)
        , m_ReplicationErrors(0)
    {
    }

    void ServerReplication::ReplicateActors()
    {
        if (!m_ReplicationManager)
        {
            return;
        }
        
        TArray<UObject*> ActorsToReplicate = GetActorsToReplicate();
        
        for (UObject* Actor : ActorsToReplicate)
        {
            ReplicateActorToAll(Actor);
        }
    }

    void ServerReplication::ReplicateActorsToClient(int32 ClientID)
    {
        if (!m_ReplicationManager || !m_NetworkManager)
        {
            return;
        }
        
        if (!m_NetworkManager->HasConnection(ClientID))
        {
            return;
        }
        
        TArray<UObject*> ActorsToReplicate = GetActorsToReplicate();
        
        for (UObject* Actor : ActorsToReplicate)
        {
            ReplicateActorToClient(Actor, ClientID);
        }
    }

    void ServerReplication::ReplicateActorToAll(UObject* Actor)
    {
        if (!m_ReplicationManager || !m_NetworkManager)
        {
            return;
        }
        
        if (!ValidateActorForReplication(Actor))
        {
            return;
        }
        
        TArray<int32> Connections = m_NetworkManager->GetAllConnections();
        
        for (int32 ConnectionID : Connections)
        {
            ReplicateActorToClient(Actor, ConnectionID);
        }
    }

    void ServerReplication::ReplicateActorToClient(UObject* Actor, int32 ClientID)
    {
        if (!m_ReplicationManager || !m_NetworkManager)
        {
            return;
        }
        
        if (!ValidateActorForReplication(Actor) || !ValidateClientForReplication(ClientID))
        {
            return;
        }
        
        ReplicationInfo* Info = m_ReplicationManager->GetReplicationInfo(Actor);
        if (!Info)
        {
            return;
        }
        
        FString ReplicationData = Info->GetReplicationData();
        m_NetworkManager->SendData(ClientID, ReplicationData);
        
        m_ReplicatedActorsCount++;
        m_TotalReplicationSize += Info->GetReplicationSize();
        
        if (m_OnActorReplicatedCallback)
        {
            m_OnActorReplicatedCallback(Actor, ClientID);
        }
        
        Utils::Get().LogDebug(TEXT("Actor replicated to client ") + FString::FromInt(ClientID) + TEXT(": ") + Actor->GetName());
    }

    void ServerReplication::ProcessServerReplication(float DeltaTime)
    {
        if (!m_ReplicationManager)
        {
            return;
        }
        
        m_ReplicationManager->ProcessReplication(DeltaTime);
        UpdateReplicationPriorities();
        ProcessReplicationQueue();
    }

    void ServerReplication::UpdateReplicationPriorities()
    {
        if (!m_ReplicationManager)
        {
            return;
        }
        
        TArray<UObject*> ReplicatedObjects = m_ReplicationManager->GetReplicatedObjects();
        
        for (UObject* Object : ReplicatedObjects)
        {
            CalculateReplicationPriority(Object);
        }
    }

    void ServerReplication::ProcessReplicationQueue()
    {
        int32 ProcessedCount = 0;
        
        for (UObject* Actor : m_ReplicationQueue)
        {
            if (ProcessedCount >= m_MaxReplicationPerTick)
            {
                break;
            }
            
            ProcessActorReplication(Actor);
            ProcessedCount++;
        }
    }

    TArray<UObject*> ServerReplication::GetActorsToReplicate() const
    {
        if (!m_ReplicationManager)
        {
            return TArray<UObject*>();
        }
        
        return m_ReplicationManager->GetReplicatedObjects();
    }

    TArray<UObject*> ServerReplication::GetActorsForClient(int32 ClientID) const
    {
        TArray<UObject*> Actors;
        
        if (!m_ReplicationManager || !m_NetworkManager)
        {
            return Actors;
        }
        
        if (!m_NetworkManager->HasConnection(ClientID))
        {
            return Actors;
        }
        
        TArray<UObject*> AllActors = GetActorsToReplicate();
        
        for (UObject* Actor : AllActors)
        {
            if (ShouldReplicateActor(Actor))
            {
                Actors.Add(Actor);
            }
        }
        
        return Actors;
    }

    int32 ServerReplication::GetReplicationQueueSize() const
    {
        return static_cast<int32>(m_ReplicationQueue.Num());
    }

    float ServerReplication::GetReplicationLoad() const
    {
        if (m_MaxReplicationPerTick == 0)
        {
            return 0.0f;
        }
        
        return static_cast<float>(m_ReplicationQueue.Num()) / static_cast<float>(m_MaxReplicationPerTick);
    }

    int32 ServerReplication::GetReplicatedActorsCount() const
    {
        return m_ReplicatedActorsCount;
    }

    int32 ServerReplication::GetTotalReplicationSize() const
    {
        return m_TotalReplicationSize;
    }

    float ServerReplication::GetAverageReplicationTime() const
    {
        return m_AverageReplicationTime;
    }

    int32 ServerReplication::GetReplicationErrors() const
    {
        return m_ReplicationErrors;
    }

    bool ServerReplication::ValidateActorForReplication(UObject* Actor) const
    {
        if (!Actor)
        {
            return false;
        }
        
        if (!m_ReplicationManager)
        {
            return false;
        }
        
        return m_ReplicationManager->IsObjectRegistered(Actor);
    }

    bool ServerReplication::ValidateClientForReplication(int32 ClientID) const
    {
        if (!m_NetworkManager)
        {
            return false;
        }
        
        return m_NetworkManager->HasConnection(ClientID);
    }

    TArray<FString> ServerReplication::ValidateServerReplication() const
    {
        TArray<FString> Errors;
        
        if (!m_ReplicationManager)
        {
            Errors.Add(TEXT("ServerReplication has no ReplicationManager"));
        }
        
        if (!m_NetworkManager)
        {
            Errors.Add(TEXT("ServerReplication has no NetworkManager"));
        }
        
        if (m_ReplicationTickRate <= 0.0f)
        {
            Errors.Add(TEXT("Invalid replication tick rate"));
        }
        
        if (m_MaxReplicationPerTick <= 0)
        {
            Errors.Add(TEXT("Invalid max replication per tick"));
        }
        
        return Errors;
    }

    void ServerReplication::ClearReplicationQueue()
    {
        m_ReplicationQueue.Empty();
    }

    void ServerReplication::ResetServerReplication()
    {
        ClearReplicationQueue();
        m_ReplicationPriorities.Empty();
        m_ReplicatedActorsCount = 0;
        m_TotalReplicationSize = 0;
        m_AverageReplicationTime = 0.0f;
        m_ReplicationErrors = 0;
    }

    void ServerReplication::AddToReplicationQueue(UObject* Actor)
    {
        if (!m_ReplicationQueue.Contains(Actor))
        {
            m_ReplicationQueue.Add(Actor);
        }
    }

    void ServerReplication::RemoveFromReplicationQueue(UObject* Actor)
    {
        m_ReplicationQueue.Remove(Actor);
    }

    void ServerReplication::CalculateReplicationPriority(UObject* Actor)
    {
        // Simple priority calculation based on object name hash
        uint32 Hash = 0;
        FString Name = Actor->GetName();
        for (int32 i = 0; i < Name.Len(); ++i)
        {
            Hash = Hash * 31 + static_cast<uint32>(Name[i]);
        }
        
        float Priority = static_cast<float>(Hash % 100) / 100.0f;
        m_ReplicationPriorities.Add(Actor, Priority);
    }

    bool ServerReplication::ShouldReplicateActor(UObject* Actor) const
    {
        if (!Actor)
        {
            return false;
        }
        
        if (!m_ReplicationManager)
        {
            return false;
        }
        
        ReplicationInfo* Info = m_ReplicationManager->GetReplicationInfo(Actor);
        if (!Info)
        {
            return false;
        }
        
        return Info->IsValidForReplication();
    }

    void ServerReplication::ProcessActorReplication(UObject* Actor)
    {
        if (!Actor)
        {
            return;
        }
        
        // Simulate actor replication processing
        Utils::Get().LogDebug(TEXT("Processing actor replication: ") + Actor->GetName());
    }

    // ReplicationSystem implementation
    ReplicationSystem* ReplicationSystem::s_Instance = nullptr;

    ReplicationSystem& ReplicationSystem::Get()
    {
        if (!s_Instance)
        {
            s_Instance = new ReplicationSystem();
        }
        return *s_Instance;
    }

    void ReplicationSystem::Initialize()
    {
        m_ReplicationManager = std::make_unique<ReplicationManager>();
        m_NetworkManager = std::make_unique<NetworkManager>();
        m_ServerReplication = std::make_unique<ServerReplication>();
        
        m_ServerReplication->SetReplicationManager(m_ReplicationManager.get());
        m_ServerReplication->SetNetworkManager(m_NetworkManager.get());
        
        m_bReplicationEnabled = true;
        m_bNetworkEnabled = true;
        
        Utils::Get().LogInfo(TEXT("ReplicationSystem initialized"));
    }

    void ReplicationSystem::Shutdown()
    {
        if (m_ServerReplication)
        {
            m_ServerReplication->ResetServerReplication();
        }
        
        if (m_NetworkManager)
        {
            m_NetworkManager->ShutdownNetwork();
        }
        
        if (m_ReplicationManager)
        {
            m_ReplicationManager->CleanupReplication();
        }
        
        m_bReplicationEnabled = false;
        m_bNetworkEnabled = false;
        
        Utils::Get().LogInfo(TEXT("ReplicationSystem shutdown"));
    }

    void ReplicationSystem::Update(float DeltaTime)
    {
        if (m_bReplicationEnabled && m_ReplicationManager)
        {
            m_ReplicationManager->ProcessReplication(DeltaTime);
        }
        
        if (m_bNetworkEnabled && m_ServerReplication)
        {
            m_ServerReplication->ProcessServerReplication(DeltaTime);
        }
    }

    void ReplicationSystem::SetReplicationEnabled(bool bEnabled)
    {
        m_bReplicationEnabled = bEnabled;
        if (m_ReplicationManager)
        {
            m_ReplicationManager->SetReplicationEnabled(bEnabled);
        }
    }

    void ReplicationSystem::SetNetworkEnabled(bool bEnabled)
    {
        m_bNetworkEnabled = bEnabled;
    }

    int32 ReplicationSystem::GetTotalReplicatedObjects() const
    {
        return m_ReplicationManager ? m_ReplicationManager->GetReplicatedObjectCount() : 0;
    }

    int32 ReplicationSystem::GetTotalConnections() const
    {
        return m_NetworkManager ? m_NetworkManager->GetConnectionCount() : 0;
    }

    int32 ReplicationSystem::GetTotalReplicationSize() const
    {
        return m_ReplicationManager ? m_ReplicationManager->GetTotalReplicationSize() : 0;
    }

    float ReplicationSystem::GetReplicationPerformance() const
    {
        if (!m_ServerReplication)
        {
            return 0.0f;
        }
        
        float Load = m_ServerReplication->GetReplicationLoad();
        return 1.0f - Load; // Higher is better
    }

    bool ReplicationSystem::ValidateSystem() const
    {
        if (!m_ReplicationManager || !m_NetworkManager || !m_ServerReplication)
        {
            return false;
        }
        
        return m_ReplicationManager->ValidateAllReplication().Num() == 0;
    }

    TArray<FString> ReplicationSystem::ValidateAllComponents() const
    {
        TArray<FString> Errors;
        
        if (!m_ReplicationManager)
        {
            Errors.Add(TEXT("ReplicationManager is null"));
        }
        else
        {
            TArray<FString> ReplicationErrors = m_ReplicationManager->ValidateAllReplication();
            Errors.Append(ReplicationErrors);
        }
        
        if (!m_NetworkManager)
        {
            Errors.Add(TEXT("NetworkManager is null"));
        }
        else if (!m_NetworkManager->ValidateNetworkSettings())
        {
            Errors.Add(TEXT("NetworkManager has invalid settings"));
        }
        
        if (!m_ServerReplication)
        {
            Errors.Add(TEXT("ServerReplication is null"));
        }
        else
        {
            TArray<FString> ServerErrors = m_ServerReplication->ValidateServerReplication();
            Errors.Append(ServerErrors);
        }
        
        return Errors;
    }

    void ReplicationSystem::Cleanup()
    {
        m_ReplicationManager.reset();
        m_NetworkManager.reset();
        m_ServerReplication.reset();
    }

    void ReplicationSystem::Reset()
    {
        if (m_ReplicationManager)
        {
            m_ReplicationManager->ResetReplication();
        }
        
        if (m_ServerReplication)
        {
            m_ServerReplication->ResetServerReplication();
        }
    }
}
