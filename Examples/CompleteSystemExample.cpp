/*
 * UE4 SDK Complete System Example - Demonstrates all systems working together
 * Vibe coder approved ✨
 */

#include "UE4SDK.h"
#include <iostream>

using namespace UE4SDK;

int main()
{
    std::cout << "=== UE4 SDK Complete System Example ===" << std::endl;
    
    // Initialize the SDK
    SDK& sdk = SDK::Get();
    sdk.Initialize();
    
    std::cout << "\n1. Core Types Example" << std::endl;
    {
        // Create some vectors and test math operations
        FVector PlayerLocation(100.0f, 200.0f, 300.0f);
        FVector EnemyLocation(150.0f, 250.0f, 350.0f);
        
        FVector Direction = Utils::Get().Direction(PlayerLocation, EnemyLocation);
        float Distance = Utils::Get().Distance(PlayerLocation, EnemyLocation);
        
        std::cout << "Player Location: " << PlayerLocation.ToString().c_str() << std::endl;
        std::cout << "Enemy Location: " << EnemyLocation.ToString().c_str() << std::endl;
        std::cout << "Direction: " << Direction.ToString().c_str() << std::endl;
        std::cout << "Distance: " << Distance << std::endl;
        
        // Test string operations
        FString PlayerName = TEXT("Player1");
        FString FormattedName = Utils::Get().FormatString(TEXT("Player: %s"), PlayerName.c_str());
        std::cout << "Formatted Name: " << FormattedName.c_str() << std::endl;
    }
    
    std::cout << "\n2. UObject System Example" << std::endl;
    {
        // Create some objects
        UObject* Player = GObjects::Get().CreateObject(TEXT("Player"), nullptr, TEXT("Player1"));
        UObject* Enemy = GObjects::Get().CreateObject(TEXT("Enemy"), nullptr, TEXT("Enemy1"));
        
        if (Player && Enemy)
        {
            Player->SetProperty(TEXT("Health"), TEXT("100"));
            Player->SetProperty(TEXT("Score"), TEXT("1500"));
            Enemy->SetProperty(TEXT("Health"), TEXT("75"));
            Enemy->SetProperty(TEXT("Damage"), TEXT("25"));
            
            std::cout << "Player Health: " << Player->GetProperty(TEXT("Health")).c_str() << std::endl;
            std::cout << "Player Score: " << Player->GetProperty(TEXT("Score")).c_str() << std::endl;
            std::cout << "Enemy Health: " << Enemy->GetProperty(TEXT("Health")).c_str() << std::endl;
            std::cout << "Enemy Damage: " << Enemy->GetProperty(TEXT("Damage")).c_str() << std::endl;
            
            // Test object tags
            Player->AddTag(TEXT("Player"));
            Player->AddTag(TEXT("Alive"));
            Enemy->AddTag(TEXT("Enemy"));
            Enemy->AddTag(TEXT("Hostile"));
            
            std::cout << "Player has 'Player' tag: " << (Player->HasTag(TEXT("Player")) ? "Yes" : "No") << std::endl;
            std::cout << "Enemy has 'Hostile' tag: " << (Enemy->HasTag(TEXT("Hostile")) ? "Yes" : "No") << std::endl;
        }
    }
    
    std::cout << "\n3. Blackboard System Example" << std::endl;
    {
        // Create blackboard data
        BlackboardData* AIData = BLACKBOARD_SYSTEM.CreateBlackboardData(TEXT("AIData"));
        AIData->AddKey(TEXT("TargetLocation"), EBlackboardKeyType::Vector, TEXT("Where the AI wants to go"));
        AIData->AddKey(TEXT("HasTarget"), EBlackboardKeyType::Bool, TEXT("Whether AI has a target"));
        AIData->AddKey(TEXT("Health"), EBlackboardKeyType::Int, TEXT("Current health"));
        AIData->AddKey(TEXT("IsAlive"), EBlackboardKeyType::Bool, TEXT("Whether AI is alive"));
        
        // Create blackboard instance
        Blackboard* AIBlackboard = CREATE_BLACKBOARD(TEXT("AIBlackboard"), AIData);
        
        if (AIBlackboard)
        {
            // Set some values
            AIBlackboard->SetValueAsVector(TEXT("TargetLocation"), FVector(500.0f, 600.0f, 700.0f));
            AIBlackboard->SetValueAsBool(TEXT("HasTarget"), true);
            AIBlackboard->SetValueAsInt(TEXT("Health"), 100);
            AIBlackboard->SetValueAsBool(TEXT("IsAlive"), true);
            
            // Read values back
            FVector TargetLoc = AIBlackboard->GetValueAsVector(TEXT("TargetLocation"));
            bool HasTarget = AIBlackboard->GetValueAsBool(TEXT("HasTarget"));
            int32 Health = AIBlackboard->GetValueAsInt(TEXT("Health"));
            bool IsAlive = AIBlackboard->GetValueAsBool(TEXT("IsAlive"));
            
            std::cout << "Target Location: " << TargetLoc.ToString().c_str() << std::endl;
            std::cout << "Has Target: " << (HasTarget ? "Yes" : "No") << std::endl;
            std::cout << "Health: " << Health << std::endl;
            std::cout << "Is Alive: " << (IsAlive ? "Yes" : "No") << std::endl;
            
            // Test value change callback
            AIBlackboard->SetOnValueChangedCallback(TEXT("Health"), []() {
                std::cout << "Health value changed!" << std::endl;
            });
            
            AIBlackboard->SetValueAsInt(TEXT("Health"), 75); // This should trigger the callback
        }
    }
    
    std::cout << "\n4. AI System Example" << std::endl;
    {
        // Create AI controller
        AIController* AIController = new AIController();
        AIController->SetActive(true);
        AIController->SetUpdateFrequency(2.0f);
        
        // Create behavior tree
        BehaviorTree* AITree = new BehaviorTree();
        AITree->SetTreeName(TEXT("EnemyAI"));
        
        // Create behavior tree component
        BehaviorTreeComponent* BTComponent = new BehaviorTreeComponent();
        BTComponent->SetBehaviorTree(AITree);
        BTComponent->SetUpdateFrequency(1.0f);
        
        // Link components
        AIController->SetBehaviorTreeComponent(BTComponent);
        
        // Create AI service
        AIService* HealthService = new AIService();
        HealthService->SetServiceName(TEXT("HealthService"));
        HealthService->SetUpdateFrequency(0.5f);
        HealthService->SetActive(true);
        
        // Register with AI system
        AISystem* AISystem = new AISystem();
        AISystem->Initialize();
        AISystem->RegisterAIController(AIController);
        AISystem->RegisterAIService(HealthService);
        
        std::cout << "AI Controller active: " << (AIController->IsActive() ? "Yes" : "No") << std::endl;
        std::cout << "AI Service active: " << (HealthService->IsActive() ? "Yes" : "No") << std::endl;
        std::cout << "AI System initialized: " << (AISystem->IsInitialized() ? "Yes" : "No") << std::endl;
        
        // Update AI system
        AISystem->Update(0.1f);
        
        // Cleanup
        AISystem->Shutdown();
        delete AISystem;
        delete HealthService;
        delete BTComponent;
        delete AITree;
        delete AIController;
    }
    
    std::cout << "\n5. Navigation System Example" << std::endl;
    {
        // Create navigation system
        NavigationSystem* NavSystem = new NavigationSystem();
        NavSystem->SetSystemName(TEXT("MainNavigation"));
        NavSystem->Initialize();
        
        // Create navmesh
        NavMesh* NavMesh = new NavMesh();
        NavMesh->SetMeshName(TEXT("Level1NavMesh"));
        NavMesh->SetCellSize(100.0f);
        NavMesh->SetAgentRadius(50.0f);
        NavMesh->SetAgentHeight(200.0f);
        
        // Add some polygons to the navmesh
        TArray<FVector> PolygonVertices;
        PolygonVertices.Add(FVector(0.0f, 0.0f, 0.0f));
        PolygonVertices.Add(FVector(100.0f, 0.0f, 0.0f));
        PolygonVertices.Add(FVector(100.0f, 100.0f, 0.0f));
        PolygonVertices.Add(FVector(0.0f, 100.0f, 0.0f));
        
        NavMeshPolygon Polygon(1, PolygonVertices);
        NavMesh->AddNavMeshPolygon(Polygon);
        NavMesh->BuildNavMesh();
        
        // Register navmesh
        NavSystem->RegisterNavMesh(NavMesh);
        
        // Create query filter
        NavQueryFilter* Filter = new NavQueryFilter();
        Filter->SetFilterName(TEXT("PlayerFilter"));
        Filter->SetAgentRadius(50.0f);
        Filter->SetAgentHeight(200.0f);
        Filter->SetMaxSlope(45.0f);
        
        NavSystem->RegisterQueryFilter(Filter);
        
        // Test pathfinding
        FVector Start(10.0f, 10.0f, 0.0f);
        FVector End(90.0f, 90.0f, 0.0f);
        
        NavPath* Path = NavSystem->FindPath(Start, End, Filter);
        if (Path && Path->IsValid())
        {
            std::cout << "Path found! Distance: " << Path->GetTotalDistance() << std::endl;
            std::cout << "Start: " << Path->GetStartPoint().ToString().c_str() << std::endl;
            std::cout << "End: " << Path->GetEndPoint().ToString().c_str() << std::endl;
        }
        else
        {
            std::cout << "No path found" << std::endl;
        }
        
        // Test point queries
        FVector TestPoint(50.0f, 50.0f, 0.0f);
        bool InNavMesh = NavSystem->IsPointInNavMesh(TestPoint);
        std::cout << "Point in navmesh: " << (InNavMesh ? "Yes" : "No") << std::endl;
        
        // Cleanup
        NavSystem->Shutdown();
        delete NavSystem;
        delete NavMesh;
        delete Filter;
        if (Path) delete Path;
    }
    
    std::cout << "\n6. Replication System Example" << std::endl;
    {
        // Initialize replication system
        REPLICATION_SYSTEM.Initialize();
        
        // Create network manager
        NetworkManager* NetManager = NETWORK_MANAGER;
        NetManager->SetServerAddress(TEXT("127.0.0.1"));
        NetManager->SetServerPort(7777);
        NetManager->SetMaxConnections(10);
        NetManager->SetIsServer(true);
        
        // Add some connections
        NetManager->AddConnection(1, TEXT("192.168.1.100"), 7777);
        NetManager->AddConnection(2, TEXT("192.168.1.101"), 7777);
        
        std::cout << "Network connections: " << NetManager->GetConnectionCount() << std::endl;
        std::cout << "Server address: " << NetManager->GetServerAddress().c_str() << std::endl;
        std::cout << "Server port: " << NetManager->GetServerPort() << std::endl;
        
        // Create replication manager
        ReplicationManager* RepManager = REPLICATION_MANAGER;
        RepManager->SetGlobalReplicationFrequency(10.0f);
        RepManager->SetReplicationEnabled(true);
        
        // Create some objects to replicate
        UObject* ReplicatedObject = GObjects::Get().CreateObject(TEXT("ReplicatedObject"), nullptr, TEXT("Obj1"));
        if (ReplicatedObject)
        {
            ReplicationInfo* RepInfo = new ReplicationInfo();
            RepInfo->SetReplicationType(EReplicationType::Reliable);
            RepInfo->SetReplicationCondition(EReplicationCondition::None);
            RepInfo->SetNetRole(ENetRole::Authority);
            RepInfo->SetReplicated(true);
            RepInfo->SetReplicationFrequency(5.0f);
            
            RepManager->RegisterObject(ReplicatedObject, RepInfo);
            
            std::cout << "Replicated objects: " << RepManager->GetReplicatedObjectCount() << std::endl;
            std::cout << "Replication enabled: " << (RepManager->IsReplicationEnabled() ? "Yes" : "No") << std::endl;
        }
        
        // Create server replication
        ServerReplication* ServerRep = SERVER_REPLICATION;
        ServerRep->SetReplicationTickRate(60.0f);
        ServerRep->SetMaxReplicationPerTick(100);
        
        // Process replication
        REPLICATION_SYSTEM.Update(0.1f);
        
        std::cout << "Server replication tick rate: " << ServerRep->GetReplicationTickRate() << std::endl;
        std::cout << "Max replication per tick: " << ServerRep->GetMaxReplicationPerTick() << std::endl;
        
        // Test data sending
        NetManager->SendDataToAll(TEXT("Hello from server!"));
        NetManager->BroadcastData(TEXT("Broadcast message"));
        
        // Cleanup
        REPLICATION_SYSTEM.Shutdown();
    }
    
    std::cout << "\n7. Utility Functions Example" << std::endl;
    {
        Utils& utils = Utils::Get();
        
        // Test random functions
        FVector RandomVec = utils.RandomVector(FVector(-100.0f, -100.0f, 0.0f), FVector(100.0f, 100.0f, 100.0f));
        FRotator RandomRot = utils.RandomRotator();
        FLinearColor RandomColor = utils.RandomColor();
        
        std::cout << "Random Vector: " << RandomVec.ToString().c_str() << std::endl;
        std::cout << "Random Rotator: " << RandomRot.ToString().c_str() << std::endl;
        std::cout << "Random Color: " << RandomColor.ToString().c_str() << std::endl;
        
        // Test interpolation
        FVector A(0.0f, 0.0f, 0.0f);
        FVector B(100.0f, 100.0f, 100.0f);
        FVector Lerped = utils.LerpVector(A, B, 0.5f);
        
        std::cout << "Lerped Vector: " << Lerped.ToString().c_str() << std::endl;
        
        // Test file operations
        FString TestData = TEXT("This is test data for file operations");
        FString FilePath = TEXT("test_file.txt");
        
        bool Saved = utils.SaveStringToFile(TestData, FilePath);
        std::cout << "File saved: " << (Saved ? "Yes" : "No") << std::endl;
        
        if (Saved)
        {
            FString LoadedData = utils.LoadStringFromFile(FilePath);
            std::cout << "File loaded: " << (LoadedData == TestData ? "Yes" : "No") << std::endl;
            std::cout << "Loaded data: " << LoadedData.c_str() << std::endl;
            
            // Clean up
            utils.DeleteFile(FilePath);
        }
        
        // Test timing
        utils.StartTimer(TEXT("TestTimer"));
        utils.Sleep(0.1f); // Sleep for 100ms
        float Elapsed = utils.StopTimer(TEXT("TestTimer"));
        std::cout << "Timer elapsed: " << Elapsed << " seconds" << std::endl;
        
        // Test logging
        utils.LogInfo(TEXT("This is an info message"));
        utils.LogWarning(TEXT("This is a warning message"));
        utils.LogError(TEXT("This is an error message"));
        utils.LogDebug(TEXT("This is a debug message"));
    }
    
    std::cout << "\n8. System Statistics" << std::endl;
    {
        std::cout << "Total Objects: " << GObjects::Get().GetAllObjects().Num() << std::endl;
        std::cout << "Blackboard Data Count: " << BLACKBOARD_SYSTEM.GetBlackboardDataCount() << std::endl;
        std::cout << "Blackboard Count: " << BLACKBOARD_SYSTEM.GetBlackboardCount() << std::endl;
        std::cout << "Replication System Valid: " << (REPLICATION_SYSTEM.ValidateSystem() ? "Yes" : "No") << std::endl;
        std::cout << "Platform: " << Utils::Get().GetPlatformName().c_str() << std::endl;
        std::cout << "Engine Version: " << Utils::Get().GetEngineVersion().c_str() << std::endl;
        std::cout << "Build Configuration: " << Utils::Get().GetBuildConfiguration().c_str() << std::endl;
    }
    
    // Shutdown the SDK
    sdk.Shutdown();
    
    std::cout << "\n=== Example Complete ===" << std::endl;
    return 0;
}
