/*
 * UE4 AI System - Self-contained AI Modules implementation
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
    class AIController;
    class Blackboard;
    class BehaviorTree;
    class BehaviorTreeComponent;
    class BlackboardComponent;
    class NavMeshQueryFilter;
    class NavPath;

    // AI Task types
    enum class EBTNodeResult : uint8
    {
        Succeeded = 0,
        Failed = 1,
        Aborted = 2,
        InProgress = 3
    };

    enum class EBTDecoratorLogic : uint8
    {
        Invalid = 0,
        Test = 1,
        And = 2,
        Or = 3,
        Not = 4
    };

    enum class EBTFlowAbortMode : uint8
    {
        None = 0,
        LowerPriority = 1,
        Self = 2,
        Both = 3
    };

    // AI Task - Base class for behavior tree tasks
    class AITask : public UObject
    {
    public:
        AITask();
        virtual ~AITask() = default;

        // Task execution
        virtual EBTNodeResult ExecuteTask(AIController* Controller, Blackboard* Blackboard);
        virtual EBTNodeResult AbortTask(AIController* Controller, Blackboard* Blackboard);
        virtual void OnTaskFinished(AIController* Controller, Blackboard* Blackboard, EBTNodeResult Result);

        // Task state
        bool IsRunning() const { return m_bIsRunning; }
        void SetRunning(bool bRunning) { m_bIsRunning = bRunning; }

        // Task properties
        void SetTaskName(const FString& Name) { m_TaskName = Name; }
        const FString& GetTaskName() const { return m_TaskName; }

    protected:
        FString m_TaskName;
        bool m_bIsRunning;
    };

    // AI Decorator - Base class for behavior tree decorators
    class AIDecorator : public UObject
    {
    public:
        AIDecorator();
        virtual ~AIDecorator() = default;

        // Decorator evaluation
        virtual bool CalculateRawConditionValue(AIController* Controller, Blackboard* Blackboard) const;
        virtual EBTFlowAbortMode GetFlowAbortMode() const { return m_FlowAbortMode; }

        // Decorator properties
        void SetDecoratorName(const FString& Name) { m_DecoratorName = Name; }
        const FString& GetDecoratorName() const { return m_DecoratorName; }

    protected:
        FString m_DecoratorName;
        EBTFlowAbortMode m_FlowAbortMode;
    };

    // AI Service - Base class for behavior tree services
    class AIService : public UObject
    {
    public:
        AIService();
        virtual ~AIService() = default;

        // Service execution
        virtual void OnBecomeRelevant(AIController* Controller, Blackboard* Blackboard);
        virtual void OnCeaseRelevant(AIController* Controller, Blackboard* Blackboard);
        virtual void TickNode(AIController* Controller, Blackboard* Blackboard, float DeltaTime);

        // Service properties
        void SetServiceName(const FString& Name) { m_ServiceName = Name; }
        const FString& GetServiceName() const { return m_ServiceName; }

        void SetTickInterval(float Interval) { m_TickInterval = Interval; }
        float GetTickInterval() const { return m_TickInterval; }

    protected:
        FString m_ServiceName;
        float m_TickInterval;
        float m_LastTickTime;
    };

    // Blackboard - AI data storage
    class Blackboard : public UObject
    {
    public:
        Blackboard();
        virtual ~Blackboard() = default;

        // Value management
        void SetValueAsObject(const FString& KeyName, UObject* ObjectValue);
        void SetValueAsClass(const FString& KeyName, UClass* ClassValue);
        void SetValueAsEnum(const FString& KeyName, int32 EnumValue);
        void SetValueAsInt(const FString& KeyName, int32 IntValue);
        void SetValueAsFloat(const FString& KeyName, float FloatValue);
        void SetValueAsBool(const FString& KeyName, bool BoolValue);
        void SetValueAsString(const FString& KeyName, const FString& StringValue);
        void SetValueAsVector(const FString& KeyName, const FVector& VectorValue);
        void SetValueAsRotator(const FString& KeyName, const FRotator& RotatorValue);

        UObject* GetValueAsObject(const FString& KeyName) const;
        UClass* GetValueAsClass(const FString& KeyName) const;
        int32 GetValueAsEnum(const FString& KeyName) const;
        int32 GetValueAsInt(const FString& KeyName) const;
        float GetValueAsFloat(const FString& KeyName) const;
        bool GetValueAsBool(const FString& KeyName) const;
        FString GetValueAsString(const FString& KeyName) const;
        FVector GetValueAsVector(const FString& KeyName) const;
        FRotator GetValueAsRotator(const FString& KeyName) const;

        // Key management
        void RegisterKey(const FString& KeyName, const FString& KeyType);
        bool IsKeyRegistered(const FString& KeyName) const;
        TArray<FString> GetRegisteredKeys() const;
        FString GetKeyType(const FString& KeyName) const;

        // Value existence
        bool IsValidKey(const FString& KeyName) const;
        bool HasValue(const FString& KeyName) const;

        // Clear values
        void ClearValue(const FString& KeyName);
        void ClearAllValues();

        // Blackboard info
        void SetBlackboardName(const FString& Name) { m_BlackboardName = Name; }
        const FString& GetBlackboardName() const { return m_BlackboardName; }

    private:
        FString m_BlackboardName;
        TMap<FString, FString> m_KeyTypes;
        TMap<FString, FString> m_StringValues;
        TMap<FString, int32> m_IntValues;
        TMap<FString, float> m_FloatValues;
        TMap<FString, bool> m_BoolValues;
        TMap<FString, FVector> m_VectorValues;
        TMap<FString, FRotator> m_RotatorValues;
        TMap<FString, UObject*> m_ObjectValues;
        TMap<FString, UClass*> m_ClassValues;
    };

    // BehaviorTree - AI behavior definition
    class BehaviorTree : public UObject
    {
    public:
        BehaviorTree();
        virtual ~BehaviorTree() = default;

        // Tree structure
        void SetRootTask(AITask* RootTask) { m_RootTask = RootTask; }
        AITask* GetRootTask() const { return m_RootTask; }

        // Task management
        void AddTask(AITask* Task);
        void RemoveTask(AITask* Task);
        TArray<AITask*> GetAllTasks() const;
        AITask* FindTask(const FString& TaskName) const;

        // Decorator management
        void AddDecorator(AIDecorator* Decorator);
        void RemoveDecorator(AIDecorator* Decorator);
        TArray<AIDecorator*> GetAllDecorators() const;
        AIDecorator* FindDecorator(const FString& DecoratorName) const;

        // Service management
        void AddService(AIService* Service);
        void RemoveService(AIService* Service);
        TArray<AIService*> GetAllServices() const;
        AIService* FindService(const FString& ServiceName) const;

        // Blackboard
        void SetBlackboard(Blackboard* InBlackboard) { m_Blackboard = InBlackboard; }
        Blackboard* GetBlackboard() const { return m_Blackboard; }

        // Tree info
        void SetTreeName(const FString& Name) { m_TreeName = Name; }
        const FString& GetTreeName() const { return m_TreeName; }

        // Tree execution
        EBTNodeResult ExecuteTree(AIController* Controller);
        void AbortTree(AIController* Controller);

    private:
        FString m_TreeName;
        AITask* m_RootTask;
        TArray<AITask*> m_Tasks;
        TArray<AIDecorator*> m_Decorators;
        TArray<AIService*> m_Services;
        Blackboard* m_Blackboard;
    };

    // BehaviorTreeComponent - Runtime behavior tree execution
    class BehaviorTreeComponent : public UObject
    {
    public:
        BehaviorTreeComponent();
        virtual ~BehaviorTreeComponent() = default;

        // Tree execution
        void StartTree(BehaviorTree* Tree);
        void StopTree();
        void RestartTree();
        void PauseTree();
        void ResumeTree();

        // Tree state
        bool IsRunning() const { return m_bIsRunning; }
        bool IsPaused() const { return m_bIsPaused; }
        BehaviorTree* GetCurrentTree() const { return m_CurrentTree; }

        // Task execution
        EBTNodeResult ExecuteTask(AITask* Task);
        void AbortTask(AITask* Task);
        void OnTaskFinished(AITask* Task, EBTNodeResult Result);

        // Component info
        void SetComponentName(const FString& Name) { m_ComponentName = Name; }
        const FString& GetComponentName() const { return m_ComponentName; }

        // Owner
        void SetOwner(AIController* Owner) { m_Owner = Owner; }
        AIController* GetOwner() const { return m_Owner; }

    private:
        FString m_ComponentName;
        AIController* m_Owner;
        BehaviorTree* m_CurrentTree;
        bool m_bIsRunning;
        bool m_bIsPaused;
        TArray<AITask*> m_RunningTasks;
    };

    // BlackboardComponent - Runtime blackboard access
    class BlackboardComponent : public UObject
    {
    public:
        BlackboardComponent();
        virtual ~BlackboardComponent() = default;

        // Blackboard access
        void SetBlackboard(Blackboard* InBlackboard) { m_Blackboard = InBlackboard; }
        Blackboard* GetBlackboard() const { return m_Blackboard; }

        // Value access (delegates to blackboard)
        void SetValueAsObject(const FString& KeyName, UObject* ObjectValue);
        void SetValueAsInt(const FString& KeyName, int32 IntValue);
        void SetValueAsFloat(const FString& KeyName, float FloatValue);
        void SetValueAsBool(const FString& KeyName, bool BoolValue);
        void SetValueAsString(const FString& KeyName, const FString& StringValue);
        void SetValueAsVector(const FString& KeyName, const FVector& VectorValue);
        void SetValueAsRotator(const FString& KeyName, const FRotator& RotatorValue);

        UObject* GetValueAsObject(const FString& KeyName) const;
        int32 GetValueAsInt(const FString& KeyName) const;
        float GetValueAsFloat(const FString& KeyName) const;
        bool GetValueAsBool(const FString& KeyName) const;
        FString GetValueAsString(const FString& KeyName) const;
        FVector GetValueAsVector(const FString& KeyName) const;
        FRotator GetValueAsRotator(const FString& KeyName) const;

        // Component info
        void SetComponentName(const FString& Name) { m_ComponentName = Name; }
        const FString& GetComponentName() const { return m_ComponentName; }

        // Owner
        void SetOwner(AIController* Owner) { m_Owner = Owner; }
        AIController* GetOwner() const { return m_Owner; }

    private:
        FString m_ComponentName;
        AIController* m_Owner;
        Blackboard* m_Blackboard;
    };

    // AIController - AI controller base class
    class AIController : public UObject
    {
    public:
        AIController();
        virtual ~AIController() = default;

        // Behavior tree management
        void SetBehaviorTree(BehaviorTree* Tree);
        BehaviorTree* GetBehaviorTree() const { return m_BehaviorTree; }
        BehaviorTreeComponent* GetBehaviorTreeComponent() const { return m_BehaviorTreeComponent; }

        // Blackboard management
        void SetBlackboard(Blackboard* InBlackboard);
        Blackboard* GetBlackboard() const { return m_Blackboard; }
        BlackboardComponent* GetBlackboardComponent() const { return m_BlackboardComponent; }

        // AI state
        void StartAI();
        void StopAI();
        void PauseAI();
        void ResumeAI();
        bool IsAIEnabled() const { return m_bAIEnabled; }
        bool IsAIPaused() const { return m_bAIPaused; }

        // Movement
        void MoveToLocation(const FVector& Destination, float AcceptanceRadius = 5.0f);
        void MoveToActor(UObject* Target, float AcceptanceRadius = 5.0f);
        void StopMovement();
        bool IsMoving() const { return m_bIsMoving; }

        // Target
        void SetTarget(UObject* NewTarget);
        UObject* GetTarget() const { return m_Target; }

        // AI updates
        virtual void TickAI(float DeltaTime);

        // Controller info
        void SetControllerName(const FString& Name) { m_ControllerName = Name; }
        const FString& GetControllerName() const { return m_ControllerName; }

    private:
        FString m_ControllerName;
        BehaviorTree* m_BehaviorTree;
        BehaviorTreeComponent* m_BehaviorTreeComponent;
        Blackboard* m_Blackboard;
        BlackboardComponent* m_BlackboardComponent;
        UObject* m_Target;
        bool m_bAIEnabled;
        bool m_bAIPaused;
        bool m_bIsMoving;
        FVector m_MoveDestination;
        float m_MoveAcceptanceRadius;
    };

    // AI System - Global AI management
    class AISystem
    {
    public:
        static AISystem& Get();

        // Blackboard management
        Blackboard* CreateBlackboard(const FString& BlackboardName);
        void DestroyBlackboard(Blackboard* Blackboard);
        Blackboard* FindBlackboard(const FString& BlackboardName) const;
        TArray<Blackboard*> GetAllBlackboards() const;

        // Behavior tree management
        BehaviorTree* CreateBehaviorTree(const FString& TreeName);
        void DestroyBehaviorTree(BehaviorTree* Tree);
        BehaviorTree* FindBehaviorTree(const FString& TreeName) const;
        TArray<BehaviorTree*> GetAllBehaviorTrees() const;

        // AI Controller management
        AIController* CreateAIController(const FString& ControllerName);
        void DestroyAIController(AIController* Controller);
        AIController* FindAIController(const FString& ControllerName) const;
        TArray<AIController*> GetAllAIControllers() const;

        // AI updates
        void TickAI(float DeltaTime);

        // Cleanup
        void Cleanup();

    private:
        AISystem() = default;
        ~AISystem() = default;

        AISystem(const AISystem&) = delete;
        AISystem& operator=(const AISystem&) = delete;

        TArray<std::unique_ptr<Blackboard>> m_Blackboards;
        TArray<std::unique_ptr<BehaviorTree>> m_BehaviorTrees;
        TArray<std::unique_ptr<AIController>> m_AIControllers;

        static AISystem* s_Instance;
    };

    // Global access macros
    #define AI_SYSTEM UE4SDK::AISystem::Get()
    #define CREATE_BLACKBOARD(Name) AI_SYSTEM.CreateBlackboard(Name)
    #define CREATE_BEHAVIOR_TREE(Name) AI_SYSTEM.CreateBehaviorTree(Name)
    #define CREATE_AI_CONTROLLER(Name) AI_SYSTEM.CreateAIController(Name)
}
