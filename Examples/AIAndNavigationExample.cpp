/*
 * UE4 SDK AI and Navigation Example
 * Shows UObject, GObjects, AI Modules, Navigation System, and BlackBoards
 * Clean, vibe coder style example ✨
 */

#include "../UE4SDK.h"

// Example showing UObject and GObjects system
void UObjectSystemExample()
{
    UE_LOG(LogUE4SDK, Log, TEXT("=== UObject System Example ==="));
    
    // Create a custom class
    auto* MyClass = GOBJECTS.RegisterClass(TEXT("MyCustomClass"));
    MyClass->AddProperty(TEXT("Health"), TEXT("float"));
    MyClass->AddProperty(TEXT("Name"), TEXT("string"));
    MyClass->AddFunction(TEXT("TakeDamage"), [](UObject* Object, const TArray<FString>& Parameters)
    {
        if (Parameters.Num() > 0)
        {
            float Damage = UE4_UTILS.ToFloat(Parameters[0]);
            UE_LOG(LogUE4SDK, Log, TEXT("Object %s took %s damage"), *Object->GetName(), *UE4_UTILS.ToString(Damage));
        }
    });
    
    // Create objects using the class
    auto* Player1 = GOBJECTS.CreateObject(TEXT("MyCustomClass"), nullptr, TEXT("Player1"));
    auto* Player2 = GOBJECTS.CreateObject(TEXT("MyCustomClass"), nullptr, TEXT("Player2"));
    
    if (Player1 && Player2)
    {
        // Set properties
        Player1->SetProperty(TEXT("Health"), TEXT("100.0"));
        Player1->SetProperty(TEXT("Name"), TEXT("VibeCoder"));
        
        Player2->SetProperty(TEXT("Health"), TEXT("80.0"));
        Player2->SetProperty(TEXT("Name"), TEXT("GameDev"));
        
        // Call functions
        Player1->CallFunction(TEXT("TakeDamage"), {TEXT("25.0")});
        Player2->CallFunction(TEXT("TakeDamage"), {TEXT("15.0")});
        
        // Add tags
        Player1->AddTag(TEXT("Player"));
        Player1->AddTag(TEXT("Alive"));
        Player2->AddTag(TEXT("Player"));
        Player2->AddTag(TEXT("Alive"));
        
        UE_LOG(LogUE4SDK, Log, TEXT("Player1 has %d tags"), Player1->GetAllTags().Num());
        UE_LOG(LogUE4SDK, Log, TEXT("Player2 has %d tags"), Player2->GetAllTags().Num());
        
        // Set custom data
        Player1->SetCustomData(TEXT("Score"), TEXT("1000"));
        Player1->SetCustomData(TEXT("Level"), TEXT("5"));
        
        UE_LOG(LogUE4SDK, Log, TEXT("Player1 Score: %s"), *Player1->GetCustomData(TEXT("Score")));
        UE_LOG(LogUE4SDK, Log, TEXT("Player1 Level: %s"), *Player1->GetCustomData(TEXT("Level")));
    }
    
    // Query objects
    auto AllObjects = GOBJECTS.GetAllObjects();
    UE_LOG(LogUE4SDK, Log, TEXT("Total objects created: %d"), AllObjects.Num());
    
    auto ObjectsOfClass = GOBJECTS.FindObjectsOfClass(TEXT("MyCustomClass"));
    UE_LOG(LogUE4SDK, Log, TEXT("Objects of MyCustomClass: %d"), ObjectsOfClass.Num());
    
    // Find specific object
    auto* FoundPlayer = GOBJECTS.FindObject(TEXT("Player1"));
    if (FoundPlayer)
    {
        UE_LOG(LogUE4SDK, Log, TEXT("Found player: %s"), *FoundPlayer->GetName());
    }
    
    UE_LOG(LogUE4SDK, Log, TEXT("UObject system example completed!"));
}

// Example showing AI System with Blackboards
void AISystemExample()
{
    UE_LOG(LogUE4SDK, Log, TEXT("=== AI System Example ==="));
    
    // Create a blackboard
    auto* EnemyBlackboard = CREATE_BLACKBOARD(TEXT("EnemyBlackboard"));
    if (EnemyBlackboard)
    {
        // Register keys
        EnemyBlackboard->RegisterKey(TEXT("Target"), TEXT("Object"));
        EnemyBlackboard->RegisterKey(TEXT("Health"), TEXT("Float"));
        EnemyBlackboard->RegisterKey(TEXT("IsAlive"), TEXT("Bool"));
        EnemyBlackboard->RegisterKey(TEXT("LastKnownLocation"), TEXT("Vector"));
        EnemyBlackboard->RegisterKey(TEXT("AttackRange"), TEXT("Float"));
        
        // Set values
        EnemyBlackboard->SetValueAsFloat(TEXT("Health"), 100.0f);
        EnemyBlackboard->SetValueAsBool(TEXT("IsAlive"), true);
        EnemyBlackboard->SetValueAsVector(TEXT("LastKnownLocation"), FVector(100, 200, 0));
        EnemyBlackboard->SetValueAsFloat(TEXT("AttackRange"), 150.0f);
        
        UE_LOG(LogUE4SDK, Log, TEXT("Enemy health: %s"), *UE4_UTILS.ToString(EnemyBlackboard->GetValueAsFloat(TEXT("Health"))));
        UE_LOG(LogUE4SDK, Log, TEXT("Enemy is alive: %s"), EnemyBlackboard->GetValueAsBool(TEXT("IsAlive")) ? TEXT("Yes") : TEXT("No"));
        UE_LOG(LogUE4SDK, Log, TEXT("Last known location: %s"), *EnemyBlackboard->GetValueAsVector(TEXT("LastKnownLocation")).ToString());
    }
    
    // Create AI tasks
    class AttackTask : public AITask
    {
    public:
        AttackTask() { SetTaskName(TEXT("AttackTask")); }
        
        EBTNodeResult ExecuteTask(AIController* Controller, Blackboard* Blackboard) override
        {
            UE_LOG(LogUE4SDK, Log, TEXT("Executing attack task"));
            
            float AttackRange = Blackboard->GetValueAsFloat(TEXT("AttackRange"));
            UE_LOG(LogUE4SDK, Log, TEXT("Attack range: %s"), *UE4_UTILS.ToString(AttackRange));
            
            return EBTNodeResult::Succeeded;
        }
    };
    
    class PatrolTask : public AITask
    {
    public:
        PatrolTask() { SetTaskName(TEXT("PatrolTask")); }
        
        EBTNodeResult ExecuteTask(AIController* Controller, Blackboard* Blackboard) override
        {
            UE_LOG(LogUE4SDK, Log, TEXT("Executing patrol task"));
            
            FVector LastLocation = Blackboard->GetValueAsVector(TEXT("LastKnownLocation"));
            UE_LOG(LogUE4SDK, Log, TEXT("Patrolling to: %s"), *LastLocation.ToString());
            
            return EBTNodeResult::Succeeded;
        }
    };
    
    // Create behavior tree
    auto* EnemyBehaviorTree = CREATE_BEHAVIOR_TREE(TEXT("EnemyBehaviorTree"));
    if (EnemyBehaviorTree)
    {
        // Create tasks
        auto* AttackTaskInstance = new AttackTask();
        auto* PatrolTaskInstance = new PatrolTask();
        
        // Add tasks to tree
        EnemyBehaviorTree->AddTask(AttackTaskInstance);
        EnemyBehaviorTree->AddTask(PatrolTaskInstance);
        
        // Set root task
        EnemyBehaviorTree->SetRootTask(PatrolTaskInstance);
        
        // Set blackboard
        EnemyBehaviorTree->SetBlackboard(EnemyBlackboard);
        
        UE_LOG(LogUE4SDK, Log, TEXT("Behavior tree created with %d tasks"), EnemyBehaviorTree->GetAllTasks().Num());
    }
    
    // Create AI controller
    auto* EnemyController = CREATE_AI_CONTROLLER(TEXT("EnemyController"));
    if (EnemyController)
    {
        // Set behavior tree and blackboard
        EnemyController->SetBehaviorTree(EnemyBehaviorTree);
        EnemyController->SetBlackboard(EnemyBlackboard);
        
        // Start AI
        EnemyController->StartAI();
        
        UE_LOG(LogUE4SDK, Log, TEXT("AI Controller started: %s"), EnemyController->IsAIEnabled() ? TEXT("Yes") : TEXT("No"));
        
        // Simulate AI updates
        EnemyController->TickAI(0.016f); // 60 FPS
        
        // Update blackboard values
        EnemyBlackboard->SetValueAsFloat(TEXT("Health"), 75.0f);
        EnemyBlackboard->SetValueAsVector(TEXT("LastKnownLocation"), FVector(150, 250, 0));
        
        UE_LOG(LogUE4SDK, Log, TEXT("Updated enemy health: %s"), *UE4_UTILS.ToString(EnemyBlackboard->GetValueAsFloat(TEXT("Health"))));
        UE_LOG(LogUE4SDK, Log, TEXT("Updated last known location: %s"), *EnemyBlackboard->GetValueAsVector(TEXT("LastKnownLocation")).ToString());
    }
    
    // Query AI system
    auto AllBlackboards = AI_SYSTEM.GetAllBlackboards();
    auto AllBehaviorTrees = AI_SYSTEM.GetAllBehaviorTrees();
    auto AllAIControllers = AI_SYSTEM.GetAllAIControllers();
    
    UE_LOG(LogUE4SDK, Log, TEXT("Total blackboards: %d"), AllBlackboards.Num());
    UE_LOG(LogUE4SDK, Log, TEXT("Total behavior trees: %d"), AllBehaviorTrees.Num());
    UE_LOG(LogUE4SDK, Log, TEXT("Total AI controllers: %d"), AllAIControllers.Num());
    
    UE_LOG(LogUE4SDK, Log, TEXT("AI system example completed!"));
}

// Example showing Navigation System
void NavigationSystemExample()
{
    UE_LOG(LogUE4SDK, Log, TEXT("=== Navigation System Example ==="));
    
    // Create a navigation system
    auto* NavSystem = NAV_SYSTEM_MANAGER.CreateNavigationSystem(TEXT("MainNavigationSystem"));
    if (NavSystem)
    {
        // Set navigation bounds
        NavSystem->SetNavigationBounds(FVector(-1000, -1000, -100), FVector(1000, 1000, 100));
        
        // Create a nav mesh
        auto* NavMesh = NavSystem->CreateNavMesh(TEXT("GroundNavMesh"));
        if (NavMesh)
        {
            // Add vertices to create a simple ground mesh
            NavMesh->AddVertex(FVector(0, 0, 0));
            NavMesh->AddVertex(FVector(100, 0, 0));
            NavMesh->AddVertex(FVector(100, 100, 0));
            NavMesh->AddVertex(FVector(0, 100, 0));
            NavMesh->AddVertex(FVector(200, 0, 0));
            NavMesh->AddVertex(FVector(200, 100, 0));
            
            // Add triangles
            NavMesh->AddTriangle(0, 1, 2, ENavAreaFlag::Walk);
            NavMesh->AddTriangle(0, 2, 3, ENavAreaFlag::Walk);
            NavMesh->AddTriangle(1, 4, 5, ENavAreaFlag::Walk);
            NavMesh->AddTriangle(1, 5, 2, ENavAreaFlag::Walk);
            
            // Set mesh bounds
            NavMesh->SetBounds(FVector(0, 0, -10), FVector(200, 100, 10));
            
            UE_LOG(LogUE4SDK, Log, TEXT("NavMesh created with %d vertices and %d triangles"), 
                   NavMesh->GetVertexCount(), NavMesh->GetTriangleCount());
            
            // Test point queries
            FVector TestPoint = FVector(50, 50, 0);
            bool bInMesh = NavMesh->IsPointInMesh(TestPoint);
            FVector ClosestPoint = NavMesh->FindClosestPointOnMesh(TestPoint);
            
            UE_LOG(LogUE4SDK, Log, TEXT("Point %s is in mesh: %s"), *TestPoint.ToString(), bInMesh ? TEXT("Yes") : TEXT("No"));
            UE_LOG(LogUE4SDK, Log, TEXT("Closest point on mesh: %s"), *ClosestPoint.ToString());
        }
        
        // Create a query filter
        auto* QueryFilter = NavSystem->CreateQueryFilter(TEXT("DefaultFilter"));
        if (QueryFilter)
        {
            // Set area costs
            QueryFilter->SetAreaCost(ENavAreaFlag::Walk, 1.0f);
            QueryFilter->SetAreaCost(ENavAreaFlag::Jump, 2.0f);
            QueryFilter->SetAreaCost(ENavAreaFlag::Crouch, 1.5f);
            
            // Set area flags
            QueryFilter->SetAreaFlags(ENavAreaFlag::Walk, true);
            QueryFilter->SetAreaFlags(ENavAreaFlag::Jump, false);
            QueryFilter->SetAreaFlags(ENavAreaFlag::Crouch, true);
            
            // Set query parameters
            QueryFilter->SetMaxSearchNodes(1000);
            QueryFilter->SetHeuristicScale(1.0f);
            
            UE_LOG(LogUE4SDK, Log, TEXT("Query filter created with walk cost: %s"), 
                   *UE4_UTILS.ToString(QueryFilter->GetAreaCost(ENavAreaFlag::Walk)));
        }
        
        // Test pathfinding
        FVector Start = FVector(10, 10, 0);
        FVector End = FVector(150, 50, 0);
        
        auto* Path = NavSystem->FindPath(Start, End, QueryFilter);
        if (Path && Path->IsValid())
        {
            UE_LOG(LogUE4SDK, Log, TEXT("Path found with %d points"), Path->GetPathPointCount());
            UE_LOG(LogUE4SDK, Log, TEXT("Path length: %s"), *UE4_UTILS.ToString(Path->GetPathLength()));
            
            // Log path points
            for (int32 i = 0; i < Path->GetPathPointCount(); ++i)
            {
                FVector PathPoint = Path->GetPathPoint(i);
                UE_LOG(LogUE4SDK, Log, TEXT("Path point %d: %s"), i, *PathPoint.ToString());
            }
        }
        else
        {
            UE_LOG(LogUE4SDK, Log, TEXT("No valid path found"));
        }
        
        // Test random point generation
        FVector RandomPoint = NavSystem->GetRandomPointInRadius(FVector(50, 50, 0), 50.0f, QueryFilter);
        UE_LOG(LogUE4SDK, Log, TEXT("Random point in radius: %s"), *RandomPoint.ToString());
        
        // Test reachability
        bool bReachable = NavSystem->IsLocationReachable(Start, End, QueryFilter);
        UE_LOG(LogUE4SDK, Log, TEXT("Location reachable: %s"), bReachable ? TEXT("Yes") : TEXT("No"));
    }
    
    // Query navigation system
    auto AllNavSystems = NAV_SYSTEM_MANAGER.GetAllNavigationSystems();
    UE_LOG(LogUE4SDK, Log, TEXT("Total navigation systems: %d"), AllNavSystems.Num());
    
    UE_LOG(LogUE4SDK, Log, TEXT("Navigation system example completed!"));
}

// Example showing integrated AI and Navigation
void IntegratedAIAndNavigationExample()
{
    UE_LOG(LogUE4SDK, Log, TEXT("=== Integrated AI and Navigation Example ==="));
    
    // Create a player object
    auto* Player = GOBJECTS.CreateObject(TEXT("Player"), nullptr, TEXT("Player"));
    if (Player)
    {
        Player->SetProperty(TEXT("Location"), TEXT("0,0,0"));
        Player->SetProperty(TEXT("Health"), TEXT("100"));
        Player->AddTag(TEXT("Player"));
    }
    
    // Create enemy AI
    auto* EnemyBlackboard = CREATE_BLACKBOARD(TEXT("EnemyAIBlackboard"));
    auto* EnemyController = CREATE_AI_CONTROLLER(TEXT("EnemyAI"));
    
    if (EnemyBlackboard && EnemyController)
    {
        // Set up blackboard
        EnemyBlackboard->RegisterKey(TEXT("Target"), TEXT("Object"));
        EnemyBlackboard->RegisterKey(TEXT("CurrentLocation"), TEXT("Vector"));
        EnemyBlackboard->RegisterKey(TEXT("TargetLocation"), TEXT("Vector"));
        EnemyBlackboard->RegisterKey(TEXT("IsMoving"), TEXT("Bool"));
        
        EnemyBlackboard->SetValueAsObject(TEXT("Target"), Player);
        EnemyBlackboard->SetValueAsVector(TEXT("CurrentLocation"), FVector(100, 100, 0));
        EnemyBlackboard->SetValueAsBool(TEXT("IsMoving"), false);
        
        // Create a movement task that uses navigation
        class MoveToTargetTask : public AITask
        {
        public:
            MoveToTargetTask() { SetTaskName(TEXT("MoveToTargetTask")); }
            
            EBTNodeResult ExecuteTask(AIController* Controller, Blackboard* Blackboard) override
            {
                UObject* Target = Blackboard->GetValueAsObject(TEXT("Target"));
                FVector CurrentLocation = Blackboard->GetValueAsVector(TEXT("CurrentLocation"));
                
                if (Target)
                {
                    // Get target location from target object
                    FString TargetLocationStr = Target->GetProperty(TEXT("Location"));
                    FVector TargetLocation = UE4_UTILS.ToVector(TargetLocationStr);
                    
                    // Find path to target
                    auto* Path = NAV_SYSTEM_MANAGER.FindPath(CurrentLocation, TargetLocation);
                    if (Path && Path->IsValid())
                    {
                        UE_LOG(LogUE4SDK, Log, TEXT("Enemy found path to player with %d points"), Path->GetPathPointCount());
                        
                        // Update blackboard
                        Blackboard->SetValueAsVector(TEXT("TargetLocation"), TargetLocation);
                        Blackboard->SetValueAsBool(TEXT("IsMoving"), true);
                        
                        // Simulate movement along path
                        for (int32 i = 0; i < Path->GetPathPointCount(); ++i)
                        {
                            FVector PathPoint = Path->GetPathPoint(i);
                            UE_LOG(LogUE4SDK, Log, TEXT("Enemy moving to: %s"), *PathPoint.ToString());
                        }
                        
                        return EBTNodeResult::Succeeded;
                    }
                    else
                    {
                        UE_LOG(LogUE4SDK, Log, TEXT("Enemy cannot find path to player"));
                        return EBTNodeResult::Failed;
                    }
                }
                
                return EBTNodeResult::Failed;
            }
        };
        
        // Create behavior tree with movement task
        auto* EnemyBehaviorTree = CREATE_BEHAVIOR_TREE(TEXT("EnemyAIBehaviorTree"));
        if (EnemyBehaviorTree)
        {
            auto* MoveTask = new MoveToTargetTask();
            EnemyBehaviorTree->AddTask(MoveTask);
            EnemyBehaviorTree->SetRootTask(MoveTask);
            EnemyBehaviorTree->SetBlackboard(EnemyBlackboard);
            
            // Set up AI controller
            EnemyController->SetBehaviorTree(EnemyBehaviorTree);
            EnemyController->SetBlackboard(EnemyBlackboard);
            EnemyController->StartAI();
            
            // Execute AI
            EBTNodeResult Result = EnemyBehaviorTree->ExecuteTree(EnemyController);
            UE_LOG(LogUE4SDK, Log, TEXT("AI execution result: %d"), static_cast<int32>(Result));
            
            // Check final blackboard state
            bool bIsMoving = EnemyBlackboard->GetValueAsBool(TEXT("IsMoving"));
            FVector TargetLocation = EnemyBlackboard->GetValueAsVector(TEXT("TargetLocation"));
            
            UE_LOG(LogUE4SDK, Log, TEXT("Enemy is moving: %s"), bIsMoving ? TEXT("Yes") : TEXT("No"));
            UE_LOG(LogUE4SDK, Log, TEXT("Target location: %s"), *TargetLocation.ToString());
        }
    }
    
    UE_LOG(LogUE4SDK, Log, TEXT("Integrated AI and Navigation example completed!"));
}

// Main example function
void AIAndNavigationExample()
{
    UE_LOG(LogUE4SDK, Log, TEXT("Starting AI and Navigation System Examples"));
    
    // Run all examples
    UObjectSystemExample();
    AISystemExample();
    NavigationSystemExample();
    IntegratedAIAndNavigationExample();
    
    UE_LOG(LogUE4SDK, Log, TEXT("All AI and Navigation examples completed!"));
}
