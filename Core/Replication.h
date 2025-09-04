/*
 * UE4 Replication System - Self-contained Replication implementation
 * Vibe coder approved ✨
 */

#pragma once

#include "Types.h"
#include "UObject.h"
#include <memory>
#include <functional>

namespace UE4SDK
{
    // Forward declarations
    class ReplicationInfo;
    class ReplicationManager;
    class NetworkManager;
    class ServerReplication;

    // Replication types
    enum class EReplicationType : uint8
    {
        None = 0,
        Reliable = 1,
        Unreliable = 2,
        ReliableOnLoad = 3,
        ReliableOnReplay = 4
    };

    enum class EReplicationCondition : uint8
    {
        None = 0,
        InitialOnly = 1,
        OwnerOnly = 2,
        SkipOwner = 3,
        SimulatedOnly = 4,
        AutonomousOnly = 5,
        SimulatedOrPhysics = 6,
        InitialOrOwner = 7,
        Custom = 8
    };

    enum class ENetRole : uint8
    {
        None = 0,
        SimulatedProxy = 1,
        AutonomousProxy = 2,
        Authority = 3
    };

    enum class ENetMode : uint8
    {
        Standalone = 0,
        DedicatedServer = 1,
        ListenServer = 2,
        Client = 3
    };

    // Replication info for objects
    class ReplicationInfo
    {
    public:
        ReplicationInfo();
        virtual ~ReplicationInfo() = default;

        // Replication settings
        void SetReplicationType(EReplicationType Type) { m_ReplicationType = Type; }
        EReplicationType GetReplicationType() const { return m_ReplicationType; }
        
        void SetReplicationCondition(EReplicationCondition Condition) { m_ReplicationCondition = Condition; }
        EReplicationCondition GetReplicationCondition() const { return m_ReplicationCondition; }
        
        void SetNetRole(ENetRole Role) { m_NetRole = Role; }
        ENetRole GetNetRole() const { return m_NetRole; }
        
        void SetNetMode(ENetMode Mode) { m_NetMode = Mode; }
        ENetMode GetNetMode() const { return m_NetMode; }

        // Replication state
        void SetReplicated(bool bReplicated) { m_bReplicated = bReplicated; }
        bool IsReplicated() const { return m_bReplicated; }
        
        void SetDirty(bool bDirty) { m_bDirty = bDirty; }
        bool IsDirty() const { return m_bDirty; }
        
        void SetInitialReplication(bool bInitial) { m_bInitialReplication = bInitial; }
        bool IsInitialReplication() const { return m_bInitialReplication; }

        // Replication timing
        void SetReplicationFrequency(float Frequency) { m_ReplicationFrequency = Frequency; }
        float GetReplicationFrequency() const { return m_ReplicationFrequency; }
        
        void SetLastReplicationTime(float Time) { m_LastReplicationTime = Time; }
        float GetLastReplicationTime() const { return m_LastReplicationTime; }

        // Replication data
        void SetReplicationData(const FString& Data) { m_ReplicationData = Data; }
        const FString& GetReplicationData() const { return m_ReplicationData; }
        
        void SetReplicationSize(int32 Size) { m_ReplicationSize = Size; }
        int32 GetReplicationSize() const { return m_ReplicationSize; }

        // Replication validation
        bool IsValidForReplication() const;
        bool ShouldReplicate(float CurrentTime) const;
        bool CanReplicateTo(ENetRole TargetRole) const;

        // Replication callbacks
        void SetOnReplicationCallback(std::function<void()> Callback) { m_OnReplicationCallback = Callback; }
        void SetOnReplicationReceivedCallback(std::function<void(const FString&)> Callback) { m_OnReplicationReceivedCallback = Callback; }

        // Replication helpers
        void MarkDirty();
        void ClearDirty();
        void ResetReplication();

    private:
        EReplicationType m_ReplicationType;
        EReplicationCondition m_ReplicationCondition;
        ENetRole m_NetRole;
        ENetMode m_NetMode;
        
        bool m_bReplicated;
        bool m_bDirty;
        bool m_bInitialReplication;
        
        float m_ReplicationFrequency;
        float m_LastReplicationTime;
        
        FString m_ReplicationData;
        int32 m_ReplicationSize;
        
        std::function<void()> m_OnReplicationCallback;
        std::function<void(const FString&)> m_OnReplicationReceivedCallback;
    };

    // Replication manager - manages replication for objects
    class ReplicationManager
    {
    public:
        ReplicationManager();
        virtual ~ReplicationManager() = default;

        // Object management
        void RegisterObject(UObject* Object, ReplicationInfo* ReplicationInfo);
        void UnregisterObject(UObject* Object);
        bool IsObjectRegistered(UObject* Object) const;
        ReplicationInfo* GetReplicationInfo(UObject* Object) const;

        // Replication processing
        void ProcessReplication(float DeltaTime);
        void ReplicateObject(UObject* Object);
        void ReplicateAllObjects();
        void ReplicateDirtyObjects();

        // Replication queries
        TArray<UObject*> GetReplicatedObjects() const;
        TArray<UObject*> GetDirtyObjects() const;
        TArray<UObject*> GetObjectsForRole(ENetRole Role) const;
        TArray<UObject*> GetObjectsForCondition(EReplicationCondition Condition) const;

        // Replication statistics
        int32 GetReplicatedObjectCount() const;
        int32 GetDirtyObjectCount() const;
        int32 GetTotalReplicationSize() const;
        float GetAverageReplicationFrequency() const;

        // Replication settings
        void SetGlobalReplicationFrequency(float Frequency) { m_GlobalReplicationFrequency = Frequency; }
        float GetGlobalReplicationFrequency() const { return m_GlobalReplicationFrequency; }
        
        void SetMaxReplicationSize(int32 MaxSize) { m_MaxReplicationSize = MaxSize; }
        int32 GetMaxReplicationSize() const { return m_MaxReplicationSize; }
        
        void SetReplicationEnabled(bool bEnabled) { m_bReplicationEnabled = bEnabled; }
        bool IsReplicationEnabled() const { return m_bReplicationEnabled; }

        // Replication callbacks
        void SetOnObjectReplicatedCallback(std::function<void(UObject*)> Callback) { m_OnObjectReplicatedCallback = Callback; }
        void SetOnReplicationErrorCallback(std::function<void(UObject*, const FString&)> Callback) { m_OnReplicationErrorCallback = Callback; }

        // Replication validation
        bool ValidateReplication(UObject* Object) const;
        TArray<FString> ValidateAllReplication() const;

        // Replication cleanup
        void CleanupReplication();
        void ResetReplication();

    private:
        TMap<UObject*, ReplicationInfo*> m_ReplicationInfo;
        TArray<UObject*> m_ReplicatedObjects;
        TArray<UObject*> m_DirtyObjects;
        
        float m_GlobalReplicationFrequency;
        int32 m_MaxReplicationSize;
        bool m_bReplicationEnabled;
        
        std::function<void(UObject*)> m_OnObjectReplicatedCallback;
        std::function<void(UObject*, const FString&)> m_OnReplicationErrorCallback;
        
        // Helper functions
        void AddToDirtyList(UObject* Object);
        void RemoveFromDirtyList(UObject* Object);
        bool ShouldReplicateObject(UObject* Object, float CurrentTime) const;
        void ProcessObjectReplication(UObject* Object);
    };

    // Network manager - manages network connections and replication
    class NetworkManager
    {
    public:
        NetworkManager();
        virtual ~NetworkManager() = default;

        // Network connection management
        void AddConnection(int32 ConnectionID, const FString& Address, int32 Port);
        void RemoveConnection(int32 ConnectionID);
        bool HasConnection(int32 ConnectionID) const;
        TArray<int32> GetAllConnections() const;

        // Network settings
        void SetServerAddress(const FString& Address) { m_ServerAddress = Address; }
        const FString& GetServerAddress() const { return m_ServerAddress; }
        
        void SetServerPort(int32 Port) { m_ServerPort = Port; }
        int32 GetServerPort() const { return m_ServerPort; }
        
        void SetMaxConnections(int32 MaxConnections) { m_MaxConnections = MaxConnections; }
        int32 GetMaxConnections() const { return m_MaxConnections; }

        // Network state
        void SetNetMode(ENetMode Mode) { m_NetMode = Mode; }
        ENetMode GetNetMode() const { return m_NetMode; }
        
        void SetIsServer(bool bIsServer) { m_bIsServer = bIsServer; }
        bool IsServer() const { return m_bIsServer; }
        
        void SetIsClient(bool bIsClient) { m_bIsClient = bIsClient; }
        bool IsClient() const { return m_bIsClient; }

        // Network communication
        void SendData(int32 ConnectionID, const FString& Data);
        void SendDataToAll(const FString& Data);
        void SendDataToAllExcept(int32 ExcludeConnectionID, const FString& Data);
        void BroadcastData(const FString& Data);

        // Network statistics
        int32 GetConnectionCount() const;
        int32 GetTotalBytesSent() const;
        int32 GetTotalBytesReceived() const;
        float GetNetworkLatency(int32 ConnectionID) const;

        // Network callbacks
        void SetOnConnectionCallback(std::function<void(int32)> Callback) { m_OnConnectionCallback = Callback; }
        void SetOnDisconnectionCallback(std::function<void(int32)> Callback) { m_OnDisconnectionCallback = Callback; }
        void SetOnDataReceivedCallback(std::function<void(int32, const FString&)> Callback) { m_OnDataReceivedCallback = Callback; }

        // Network validation
        bool ValidateConnection(int32 ConnectionID) const;
        bool ValidateNetworkSettings() const;

        // Network cleanup
        void DisconnectAll();
        void ShutdownNetwork();

    private:
        TMap<int32, FString> m_Connections;
        FString m_ServerAddress;
        int32 m_ServerPort;
        int32 m_MaxConnections;
        
        ENetMode m_NetMode;
        bool m_bIsServer;
        bool m_bIsClient;
        
        int32 m_TotalBytesSent;
        int32 m_TotalBytesReceived;
        
        std::function<void(int32)> m_OnConnectionCallback;
        std::function<void(int32)> m_OnDisconnectionCallback;
        std::function<void(int32, const FString&)> m_OnDataReceivedCallback;
    };

    // Server replication - handles server-side replication
    class ServerReplication
    {
    public:
        ServerReplication();
        virtual ~ServerReplication() = default;

        // Server replication management
        void SetReplicationManager(ReplicationManager* Manager) { m_ReplicationManager = Manager; }
        ReplicationManager* GetReplicationManager() const { return m_ReplicationManager; }
        
        void SetNetworkManager(NetworkManager* Manager) { m_NetworkManager = Manager; }
        NetworkManager* GetNetworkManager() const { return m_NetworkManager; }

        // Server replication operations
        void ReplicateActors();
        void ReplicateActorsToClient(int32 ClientID);
        void ReplicateActorToAll(UObject* Actor);
        void ReplicateActorToClient(UObject* Actor, int32 ClientID);

        // Server replication settings
        void SetReplicationTickRate(float TickRate) { m_ReplicationTickRate = TickRate; }
        float GetReplicationTickRate() const { return m_ReplicationTickRate; }
        
        void SetMaxReplicationPerTick(int32 MaxReplication) { m_MaxReplicationPerTick = MaxReplication; }
        int32 GetMaxReplicationPerTick() const { return m_MaxReplicationPerTick; }
        
        void SetReplicationBudget(float Budget) { m_ReplicationBudget = Budget; }
        float GetReplicationBudget() const { return m_ReplicationBudget; }

        // Server replication processing
        void ProcessServerReplication(float DeltaTime);
        void UpdateReplicationPriorities();
        void ProcessReplicationQueue();

        // Server replication queries
        TArray<UObject*> GetActorsToReplicate() const;
        TArray<UObject*> GetActorsForClient(int32 ClientID) const;
        int32 GetReplicationQueueSize() const;
        float GetReplicationLoad() const;

        // Server replication statistics
        int32 GetReplicatedActorsCount() const;
        int32 GetTotalReplicationSize() const;
        float GetAverageReplicationTime() const;
        int32 GetReplicationErrors() const;

        // Server replication callbacks
        void SetOnActorReplicatedCallback(std::function<void(UObject*, int32)> Callback) { m_OnActorReplicatedCallback = Callback; }
        void SetOnReplicationErrorCallback(std::function<void(UObject*, const FString&)> Callback) { m_OnReplicationErrorCallback = Callback; }

        // Server replication validation
        bool ValidateActorForReplication(UObject* Actor) const;
        bool ValidateClientForReplication(int32 ClientID) const;
        TArray<FString> ValidateServerReplication() const;

        // Server replication cleanup
        void ClearReplicationQueue();
        void ResetServerReplication();

    private:
        ReplicationManager* m_ReplicationManager;
        NetworkManager* m_NetworkManager;
        
        float m_ReplicationTickRate;
        int32 m_MaxReplicationPerTick;
        float m_ReplicationBudget;
        
        TArray<UObject*> m_ReplicationQueue;
        TMap<UObject*, float> m_ReplicationPriorities;
        
        int32 m_ReplicatedActorsCount;
        int32 m_TotalReplicationSize;
        float m_AverageReplicationTime;
        int32 m_ReplicationErrors;
        
        std::function<void(UObject*, int32)> m_OnActorReplicatedCallback;
        std::function<void(UObject*, const FString&)> m_OnReplicationErrorCallback;
        
        // Helper functions
        void AddToReplicationQueue(UObject* Actor);
        void RemoveFromReplicationQueue(UObject* Actor);
        void CalculateReplicationPriority(UObject* Actor);
        bool ShouldReplicateActor(UObject* Actor) const;
        void ProcessActorReplication(UObject* Actor);
    };

    // Replication system - global replication management
    class ReplicationSystem
    {
    public:
        static ReplicationSystem& Get();

        // System management
        void Initialize();
        void Shutdown();
        void Update(float DeltaTime);

        // Component access
        ReplicationManager* GetReplicationManager() const { return m_ReplicationManager.get(); }
        NetworkManager* GetNetworkManager() const { return m_NetworkManager.get(); }
        ServerReplication* GetServerReplication() const { return m_ServerReplication.get(); }

        // System settings
        void SetReplicationEnabled(bool bEnabled);
        bool IsReplicationEnabled() const { return m_bReplicationEnabled; }
        
        void SetNetworkEnabled(bool bEnabled);
        bool IsNetworkEnabled() const { return m_bNetworkEnabled; }

        // System statistics
        int32 GetTotalReplicatedObjects() const;
        int32 GetTotalConnections() const;
        int32 GetTotalReplicationSize() const;
        float GetReplicationPerformance() const;

        // System validation
        bool ValidateSystem() const;
        TArray<FString> ValidateAllComponents() const;

        // System cleanup
        void Cleanup();
        void Reset();

    private:
        ReplicationSystem() = default;
        ~ReplicationSystem() = default;

        ReplicationSystem(const ReplicationSystem&) = delete;
        ReplicationSystem& operator=(const ReplicationSystem&) = delete;

        std::unique_ptr<ReplicationManager> m_ReplicationManager;
        std::unique_ptr<NetworkManager> m_NetworkManager;
        std::unique_ptr<ServerReplication> m_ServerReplication;
        
        bool m_bReplicationEnabled;
        bool m_bNetworkEnabled;
        
        static ReplicationSystem* s_Instance;
    };

    // Global access macros
    #define REPLICATION_SYSTEM UE4SDK::ReplicationSystem::Get()
    #define REPLICATION_MANAGER REPLICATION_SYSTEM.GetReplicationManager()
    #define NETWORK_MANAGER REPLICATION_SYSTEM.GetNetworkManager()
    #define SERVER_REPLICATION REPLICATION_SYSTEM.GetServerReplication()
}
