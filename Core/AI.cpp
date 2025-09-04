/*
 * UE4 AI Implementation - Self-contained AI system implementation
 * Vibe coder approved ✨
 */

#include "AI.h"
#include "Utils.h"
#include <algorithm>

namespace UE4SDK
{
    // AIController implementation
    AIController::AIController()
        : m_ControlledPawn(nullptr)
        , m_BlackboardComponent(nullptr)
        , m_BehaviorTreeComponent(nullptr)
        , m_bIsActive(false)
        , m_UpdateFrequency(1.0f)
        , m_LastUpdateTime(0.0f)
    {
    }

    void AIController::SetControlledPawn(APawn* Pawn)
    {
        m_ControlledPawn = Pawn;
        if (Pawn)
        {
            Utils::Get().LogInfo(TEXT("AIController controlling pawn: ") + Pawn->GetName());
        }
    }

    APawn* AIController::GetControlledPawn() const
    {
        return m_ControlledPawn;
    }

    void AIController::SetBlackboardComponent(BlackboardComponent* Component)
    {
        m_BlackboardComponent = Component;
    }

    BlackboardComponent* AIController::GetBlackboardComponent() const
    {
        return m_BlackboardComponent;
    }

    void AIController::SetBehaviorTreeComponent(BehaviorTreeComponent* Component)
    {
        m_BehaviorTreeComponent = Component;
    }

    BehaviorTreeComponent* AIController::GetBehaviorTreeComponent() const
    {
        return m_BehaviorTreeComponent;
    }

    void AIController::SetActive(bool bActive)
    {
        m_bIsActive = bActive;
        if (bActive)
        {
            Utils::Get().LogInfo(TEXT("AIController activated"));
        }
        else
        {
            Utils::Get().LogInfo(TEXT("AIController deactivated"));
        }
    }

    bool AIController::IsActive() const
    {
        return m_bIsActive;
    }

    void AIController::SetUpdateFrequency(float Frequency)
    {
        m_UpdateFrequency = Frequency;
    }

    float AIController::GetUpdateFrequency() const
    {
        return m_UpdateFrequency;
    }

    void AIController::Update(float DeltaTime)
    {
        if (!m_bIsActive)
        {
            return;
        }
        
        float CurrentTime = Utils::Get().GetTimeSinceStart();
        if (CurrentTime - m_LastUpdateTime >= (1.0f / m_UpdateFrequency))
        {
            ProcessAI(DeltaTime);
            m_LastUpdateTime = CurrentTime;
        }
    }

    void AIController::ProcessAI(float DeltaTime)
    {
        if (m_BehaviorTreeComponent)
        {
            m_BehaviorTreeComponent->Update(DeltaTime);
        }
        
        if (m_BlackboardComponent)
        {
            // Update blackboard values based on current state
            UpdateBlackboardValues();
        }
    }

    void AIController::UpdateBlackboardValues()
    {
        if (!m_BlackboardComponent || !m_ControlledPawn)
        {
            return;
        }
        
        // Update common blackboard values
        m_BlackboardComponent->SetValueAsVector(TEXT("SelfLocation"), m_ControlledPawn->GetActorLocation());
        m_BlackboardComponent->SetValueAsRotator(TEXT("SelfRotation"), m_ControlledPawn->GetActorRotation());
        m_BlackboardComponent->SetValueAsBool(TEXT("IsActive"), m_bIsActive);
    }

    // BehaviorTree implementation
    BehaviorTree::BehaviorTree()
        : m_TreeName(TEXT("UnnamedBehaviorTree"))
        , m_RootNode(nullptr)
        , m_bIsValid(false)
    {
    }

    void BehaviorTree::SetTreeName(const FString& Name)
    {
        m_TreeName = Name;
    }

    const FString& BehaviorTree::GetTreeName() const
    {
        return m_TreeName;
    }

    void BehaviorTree::SetRootNode(BehaviorTreeNode* Node)
    {
        m_RootNode = Node;
        m_bIsValid = (Node != nullptr);
    }

    BehaviorTreeNode* BehaviorTree::GetRootNode() const
    {
        return m_RootNode;
    }

    bool BehaviorTree::IsValid() const
    {
        return m_bIsValid;
    }

    void BehaviorTree::AddNode(BehaviorTreeNode* Node)
    {
        if (Node)
        {
            m_Nodes.Add(Node);
        }
    }

    TArray<BehaviorTreeNode*> BehaviorTree::GetAllNodes() const
    {
        return m_Nodes;
    }

    void BehaviorTree::ValidateTree()
    {
        m_bIsValid = (m_RootNode != nullptr);
        
        if (m_bIsValid)
        {
            Utils::Get().LogInfo(TEXT("BehaviorTree validated: ") + m_TreeName);
        }
        else
        {
            Utils::Get().LogWarning(TEXT("BehaviorTree validation failed: ") + m_TreeName);
        }
    }

    // BehaviorTreeComponent implementation
    BehaviorTreeComponent::BehaviorTreeComponent()
        : m_BehaviorTree(nullptr)
        , m_CurrentNode(nullptr)
        , m_bIsRunning(false)
        , m_UpdateFrequency(1.0f)
        , m_LastUpdateTime(0.0f)
    {
    }

    void BehaviorTreeComponent::SetBehaviorTree(BehaviorTree* Tree)
    {
        m_BehaviorTree = Tree;
        if (Tree)
        {
            Utils::Get().LogInfo(TEXT("BehaviorTreeComponent set to tree: ") + Tree->GetTreeName());
        }
    }

    BehaviorTree* BehaviorTreeComponent::GetBehaviorTree() const
    {
        return m_BehaviorTree;
    }

    void BehaviorTreeComponent::StartBehaviorTree()
    {
        if (!m_BehaviorTree || !m_BehaviorTree->IsValid())
        {
            Utils::Get().LogWarning(TEXT("BehaviorTreeComponent::StartBehaviorTree - Invalid behavior tree"));
            return;
        }
        
        m_CurrentNode = m_BehaviorTree->GetRootNode();
        m_bIsRunning = true;
        
        Utils::Get().LogInfo(TEXT("BehaviorTreeComponent started: ") + m_BehaviorTree->GetTreeName());
    }

    void BehaviorTreeComponent::StopBehaviorTree()
    {
        m_bIsRunning = false;
        m_CurrentNode = nullptr;
        
        Utils::Get().LogInfo(TEXT("BehaviorTreeComponent stopped"));
    }

    void BehaviorTreeComponent::PauseBehaviorTree()
    {
        m_bIsRunning = false;
        Utils::Get().LogInfo(TEXT("BehaviorTreeComponent paused"));
    }

    void BehaviorTreeComponent::ResumeBehaviorTree()
    {
        m_bIsRunning = true;
        Utils::Get().LogInfo(TEXT("BehaviorTreeComponent resumed"));
    }

    bool BehaviorTreeComponent::IsRunning() const
    {
        return m_bIsRunning;
    }

    void BehaviorTreeComponent::SetUpdateFrequency(float Frequency)
    {
        m_UpdateFrequency = Frequency;
    }

    float BehaviorTreeComponent::GetUpdateFrequency() const
    {
        return m_UpdateFrequency;
    }

    void BehaviorTreeComponent::Update(float DeltaTime)
    {
        if (!m_bIsRunning || !m_BehaviorTree || !m_CurrentNode)
        {
            return;
        }
        
        float CurrentTime = Utils::Get().GetTimeSinceStart();
        if (CurrentTime - m_LastUpdateTime >= (1.0f / m_UpdateFrequency))
        {
            ProcessCurrentNode(DeltaTime);
            m_LastUpdateTime = CurrentTime;
        }
    }

    void BehaviorTreeComponent::ProcessCurrentNode(float DeltaTime)
    {
        if (!m_CurrentNode)
        {
            return;
        }
        
        // Simulate node execution
        ENodeExecutionResult Result = m_CurrentNode->Execute(DeltaTime);
        
        switch (Result)
        {
            case ENodeExecutionResult::Success:
                OnNodeSuccess();
                break;
            case ENodeExecutionResult::Failure:
                OnNodeFailure();
                break;
            case ENodeExecutionResult::Running:
                // Continue with current node
                break;
        }
    }

    void BehaviorTreeComponent::OnNodeSuccess()
    {
        Utils::Get().LogDebug(TEXT("BehaviorTree node succeeded"));
        // Move to next node or complete tree
        m_CurrentNode = nullptr;
        m_bIsRunning = false;
    }

    void BehaviorTreeComponent::OnNodeFailure()
    {
        Utils::Get().LogDebug(TEXT("BehaviorTree node failed"));
        // Handle failure or move to next node
        m_CurrentNode = nullptr;
        m_bIsRunning = false;
    }

    // BehaviorTreeNode implementation
    BehaviorTreeNode::BehaviorTreeNode()
        : m_NodeName(TEXT("UnnamedNode"))
        , m_NodeType(ENodeType::Action)
        , m_bIsValid(false)
    {
    }

    void BehaviorTreeNode::SetNodeName(const FString& Name)
    {
        m_NodeName = Name;
    }

    const FString& BehaviorTreeNode::GetNodeName() const
    {
        return m_NodeName;
    }

    void BehaviorTreeNode::SetNodeType(ENodeType Type)
    {
        m_NodeType = Type;
    }

    ENodeType BehaviorTreeNode::GetNodeType() const
    {
        return m_NodeType;
    }

    void BehaviorTreeNode::SetIsValid(bool bValid)
    {
        m_bIsValid = bValid;
    }

    bool BehaviorTreeNode::IsValid() const
    {
        return m_bIsValid;
    }

    void BehaviorTreeNode::AddChild(BehaviorTreeNode* Child)
    {
        if (Child)
        {
            m_Children.Add(Child);
        }
    }

    void BehaviorTreeNode::RemoveChild(BehaviorTreeNode* Child)
    {
        m_Children.Remove(Child);
    }

    TArray<BehaviorTreeNode*> BehaviorTreeNode::GetChildren() const
    {
        return m_Children;
    }

    ENodeExecutionResult BehaviorTreeNode::Execute(float DeltaTime)
    {
        if (!m_bIsValid)
        {
            return ENodeExecutionResult::Failure;
        }
        
        // Default implementation - subclasses should override
        return ENodeExecutionResult::Success;
    }

    // AITask implementation
    AITask::AITask()
        : m_TaskName(TEXT("UnnamedTask"))
        , m_bIsRunning(false)
        , m_bIsCompleted(false)
        , m_bIsSuccessful(false)
        , m_ExecutionTime(0.0f)
    {
    }

    void AITask::SetTaskName(const FString& Name)
    {
        m_TaskName = Name;
    }

    const FString& AITask::GetTaskName() const
    {
        return m_TaskName;
    }

    void AITask::StartTask()
    {
        m_bIsRunning = true;
        m_bIsCompleted = false;
        m_bIsSuccessful = false;
        m_ExecutionTime = 0.0f;
        
        Utils::Get().LogInfo(TEXT("AITask started: ") + m_TaskName);
    }

    void AITask::StopTask()
    {
        m_bIsRunning = false;
        m_bIsCompleted = true;
        
        Utils::Get().LogInfo(TEXT("AITask stopped: ") + m_TaskName);
    }

    void AITask::CompleteTask(bool bSuccessful)
    {
        m_bIsRunning = false;
        m_bIsCompleted = true;
        m_bIsSuccessful = bSuccessful;
        
        Utils::Get().LogInfo(TEXT("AITask completed: ") + m_TaskName + TEXT(" - Success: ") + FString::FromBool(bSuccessful));
    }

    bool AITask::IsRunning() const
    {
        return m_bIsRunning;
    }

    bool AITask::IsCompleted() const
    {
        return m_bIsCompleted;
    }

    bool AITask::IsSuccessful() const
    {
        return m_bIsSuccessful;
    }

    void AITask::Update(float DeltaTime)
    {
        if (m_bIsRunning)
        {
            m_ExecutionTime += DeltaTime;
            ExecuteTask(DeltaTime);
        }
    }

    void AITask::ExecuteTask(float DeltaTime)
    {
        // Default implementation - subclasses should override
        CompleteTask(true);
    }

    // AIDecorator implementation
    AIDecorator::AIDecorator()
        : m_DecoratorName(TEXT("UnnamedDecorator"))
        , m_bInvertResult(false)
        , m_bIsValid(false)
    {
    }

    void AIDecorator::SetDecoratorName(const FString& Name)
    {
        m_DecoratorName = Name;
    }

    const FString& AIDecorator::GetDecoratorName() const
    {
        return m_DecoratorName;
    }

    void AIDecorator::SetInvertResult(bool bInvert)
    {
        m_bInvertResult = bInvert;
    }

    bool AIDecorator::GetInvertResult() const
    {
        return m_bInvertResult;
    }

    void AIDecorator::SetIsValid(bool bValid)
    {
        m_bIsValid = bValid;
    }

    bool AIDecorator::IsValid() const
    {
        return m_bIsValid;
    }

    bool AIDecorator::CheckCondition()
    {
        if (!m_bIsValid)
        {
            return false;
        }
        
        bool Result = EvaluateCondition();
        return m_bInvertResult ? !Result : Result;
    }

    bool AIDecorator::EvaluateCondition()
    {
        // Default implementation - subclasses should override
        return true;
    }

    // AIService implementation
    AIService::AIService()
        : m_ServiceName(TEXT("UnnamedService"))
        , m_UpdateFrequency(1.0f)
        , m_LastUpdateTime(0.0f)
        , m_bIsActive(false)
    {
    }

    void AIService::SetServiceName(const FString& Name)
    {
        m_ServiceName = Name;
    }

    const FString& AIService::GetServiceName() const
    {
        return m_ServiceName;
    }

    void AIService::SetUpdateFrequency(float Frequency)
    {
        m_UpdateFrequency = Frequency;
    }

    float AIService::GetUpdateFrequency() const
    {
        return m_UpdateFrequency;
    }

    void AIService::SetActive(bool bActive)
    {
        m_bIsActive = bActive;
        if (bActive)
        {
            Utils::Get().LogInfo(TEXT("AIService activated: ") + m_ServiceName);
        }
        else
        {
            Utils::Get().LogInfo(TEXT("AIService deactivated: ") + m_ServiceName);
        }
    }

    bool AIService::IsActive() const
    {
        return m_bIsActive;
    }

    void AIService::Update(float DeltaTime)
    {
        if (!m_bIsActive)
        {
            return;
        }
        
        float CurrentTime = Utils::Get().GetTimeSinceStart();
        if (CurrentTime - m_LastUpdateTime >= (1.0f / m_UpdateFrequency))
        {
            ExecuteService(DeltaTime);
            m_LastUpdateTime = CurrentTime;
        }
    }

    void AIService::ExecuteService(float DeltaTime)
    {
        // Default implementation - subclasses should override
        Utils::Get().LogDebug(TEXT("AIService executing: ") + m_ServiceName);
    }

    // AISystem implementation
    AISystem::AISystem()
        : m_bIsInitialized(false)
        , m_UpdateFrequency(1.0f)
        , m_LastUpdateTime(0.0f)
    {
    }

    void AISystem::Initialize()
    {
        if (m_bIsInitialized)
        {
            return;
        }
        
        m_bIsInitialized = true;
        Utils::Get().LogInfo(TEXT("AISystem initialized"));
    }

    void AISystem::Shutdown()
    {
        if (!m_bIsInitialized)
        {
            return;
        }
        
        // Shutdown all controllers
        for (auto& Controller : m_AIControllers)
        {
            Controller->SetActive(false);
        }
        m_AIControllers.Empty();
        
        // Shutdown all services
        for (auto& Service : m_AIServices)
        {
            Service->SetActive(false);
        }
        m_AIServices.Empty();
        
        m_bIsInitialized = false;
        Utils::Get().LogInfo(TEXT("AISystem shutdown"));
    }

    void AISystem::SetUpdateFrequency(float Frequency)
    {
        m_UpdateFrequency = Frequency;
    }

    float AISystem::GetUpdateFrequency() const
    {
        return m_UpdateFrequency;
    }

    void AISystem::Update(float DeltaTime)
    {
        if (!m_bIsInitialized)
        {
            return;
        }
        
        float CurrentTime = Utils::Get().GetTimeSinceStart();
        if (CurrentTime - m_LastUpdateTime >= (1.0f / m_UpdateFrequency))
        {
            ProcessAI(DeltaTime);
            m_LastUpdateTime = CurrentTime;
        }
    }

    void AISystem::ProcessAI(float DeltaTime)
    {
        // Update all AI controllers
        for (auto& Controller : m_AIControllers)
        {
            if (Controller->IsActive())
            {
                Controller->Update(DeltaTime);
            }
        }
        
        // Update all AI services
        for (auto& Service : m_AIServices)
        {
            if (Service->IsActive())
            {
                Service->Update(DeltaTime);
            }
        }
    }

    void AISystem::RegisterAIController(AIController* Controller)
    {
        if (Controller && !m_AIControllers.Contains(Controller))
        {
            m_AIControllers.Add(Controller);
            Utils::Get().LogInfo(TEXT("AIController registered"));
        }
    }

    void AISystem::UnregisterAIController(AIController* Controller)
    {
        if (Controller)
        {
            m_AIControllers.Remove(Controller);
            Utils::Get().LogInfo(TEXT("AIController unregistered"));
        }
    }

    void AISystem::RegisterAIService(AIService* Service)
    {
        if (Service && !m_AIServices.Contains(Service))
        {
            m_AIServices.Add(Service);
            Utils::Get().LogInfo(TEXT("AIService registered: ") + Service->GetServiceName());
        }
    }

    void AISystem::UnregisterAIService(AIService* Service)
    {
        if (Service)
        {
            m_AIServices.Remove(Service);
            Utils::Get().LogInfo(TEXT("AIService unregistered: ") + Service->GetServiceName());
        }
    }

    TArray<AIController*> AISystem::GetAIControllers() const
    {
        return m_AIControllers;
    }

    TArray<AIService*> AISystem::GetAIServices() const
    {
        return m_AIServices;
    }

    int32 AISystem::GetAIControllerCount() const
    {
        return static_cast<int32>(m_AIControllers.Num());
    }

    int32 AISystem::GetAIServiceCount() const
    {
        return static_cast<int32>(m_AIServices.Num());
    }

    bool AISystem::IsInitialized() const
    {
        return m_bIsInitialized;
    }
}
