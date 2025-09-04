/*
 * UE4 Navigation Implementation - Self-contained Navigation system implementation
 * Vibe coder approved ✨
 */

#include "Navigation.h"
#include "Utils.h"
#include <algorithm>

namespace UE4SDK
{
    // NavMesh implementation
    NavMesh::NavMesh()
        : m_MeshName(TEXT("UnnamedNavMesh"))
        , m_bIsValid(false)
        , m_CellSize(100.0f)
        , m_CellHeight(100.0f)
        , m_AgentRadius(50.0f)
        , m_AgentHeight(200.0f)
        , m_MaxSlope(45.0f)
        , m_MaxClimb(100.0f)
    {
    }

    void NavMesh::SetMeshName(const FString& Name)
    {
        m_MeshName = Name;
    }

    const FString& NavMesh::GetMeshName() const
    {
        return m_MeshName;
    }

    void NavMesh::SetCellSize(float Size)
    {
        m_CellSize = Size;
    }

    float NavMesh::GetCellSize() const
    {
        return m_CellSize;
    }

    void NavMesh::SetCellHeight(float Height)
    {
        m_CellHeight = Height;
    }

    float NavMesh::GetCellHeight() const
    {
        return m_CellHeight;
    }

    void NavMesh::SetAgentRadius(float Radius)
    {
        m_AgentRadius = Radius;
    }

    float NavMesh::GetAgentRadius() const
    {
        return m_AgentRadius;
    }

    void NavMesh::SetAgentHeight(float Height)
    {
        m_AgentHeight = Height;
    }

    float NavMesh::GetAgentHeight() const
    {
        return m_AgentHeight;
    }

    void NavMesh::SetMaxSlope(float Slope)
    {
        m_MaxSlope = Slope;
    }

    float NavMesh::GetMaxSlope() const
    {
        return m_MaxSlope;
    }

    void NavMesh::SetMaxClimb(float Climb)
    {
        m_MaxClimb = Climb;
    }

    float NavMesh::GetMaxClimb() const
    {
        return m_MaxClimb;
    }

    void NavMesh::SetIsValid(bool bValid)
    {
        m_bIsValid = bValid;
    }

    bool NavMesh::IsValid() const
    {
        return m_bIsValid;
    }

    void NavMesh::AddNavMeshPolygon(const NavMeshPolygon& Polygon)
    {
        m_Polygons.Add(Polygon);
    }

    void NavMesh::RemoveNavMeshPolygon(int32 Index)
    {
        if (Index >= 0 && Index < m_Polygons.Num())
        {
            m_Polygons.RemoveAt(Index);
        }
    }

    TArray<NavMeshPolygon> NavMesh::GetPolygons() const
    {
        return m_Polygons;
    }

    int32 NavMesh::GetPolygonCount() const
    {
        return m_Polygons.Num();
    }

    void NavMesh::ClearPolygons()
    {
        m_Polygons.Empty();
    }

    bool NavMesh::IsPointInNavMesh(const FVector& Point) const
    {
        for (const auto& Polygon : m_Polygons)
        {
            if (Polygon.IsPointInside(Point))
            {
                return true;
            }
        }
        return false;
    }

    FVector NavMesh::FindNearestPointOnNavMesh(const FVector& Point) const
    {
        FVector NearestPoint = Point;
        float MinDistance = FLT_MAX;
        
        for (const auto& Polygon : m_Polygons)
        {
            FVector ClosestPoint = Polygon.GetClosestPoint(Point);
            float Distance = Utils::Get().Distance(Point, ClosestPoint);
            
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                NearestPoint = ClosestPoint;
            }
        }
        
        return NearestPoint;
    }

    void NavMesh::BuildNavMesh()
    {
        // Simulate navmesh building
        m_bIsValid = true;
        Utils::Get().LogInfo(TEXT("NavMesh built: ") + m_MeshName + TEXT(" with ") + FString::FromInt(m_Polygons.Num()) + TEXT(" polygons"));
    }

    void NavMesh::RebuildNavMesh()
    {
        ClearPolygons();
        BuildNavMesh();
    }

    // NavMeshPolygon implementation
    NavMeshPolygon::NavMeshPolygon()
        : m_PolygonID(0)
        , m_bIsValid(false)
    {
    }

    NavMeshPolygon::NavMeshPolygon(int32 ID, const TArray<FVector>& Vertices)
        : m_PolygonID(ID)
        , m_Vertices(Vertices)
        , m_bIsValid(Vertices.Num() >= 3)
    {
    }

    void NavMeshPolygon::SetPolygonID(int32 ID)
    {
        m_PolygonID = ID;
    }

    int32 NavMeshPolygon::GetPolygonID() const
    {
        return m_PolygonID;
    }

    void NavMeshPolygon::SetVertices(const TArray<FVector>& Vertices)
    {
        m_Vertices = Vertices;
        m_bIsValid = Vertices.Num() >= 3;
    }

    TArray<FVector> NavMeshPolygon::GetVertices() const
    {
        return m_Vertices;
    }

    void NavMeshPolygon::AddVertex(const FVector& Vertex)
    {
        m_Vertices.Add(Vertex);
        m_bIsValid = m_Vertices.Num() >= 3;
    }

    void NavMeshPolygon::RemoveVertex(int32 Index)
    {
        if (Index >= 0 && Index < m_Vertices.Num())
        {
            m_Vertices.RemoveAt(Index);
            m_bIsValid = m_Vertices.Num() >= 3;
        }
    }

    int32 NavMeshPolygon::GetVertexCount() const
    {
        return m_Vertices.Num();
    }

    void NavMeshPolygon::SetIsValid(bool bValid)
    {
        m_bIsValid = bValid;
    }

    bool NavMeshPolygon::IsValid() const
    {
        return m_bIsValid;
    }

    FVector NavMeshPolygon::GetCenter() const
    {
        if (m_Vertices.Num() == 0)
        {
            return FVector::ZeroVector;
        }
        
        FVector Center = FVector::ZeroVector;
        for (const auto& Vertex : m_Vertices)
        {
            Center += Vertex;
        }
        
        return Center / static_cast<float>(m_Vertices.Num());
    }

    FVector NavMeshPolygon::GetNormal() const
    {
        if (m_Vertices.Num() < 3)
        {
            return FVector::UpVector;
        }
        
        FVector V1 = m_Vertices[1] - m_Vertices[0];
        FVector V2 = m_Vertices[2] - m_Vertices[0];
        
        return (V1 ^ V2).GetSafeNormal();
    }

    bool NavMeshPolygon::IsPointInside(const FVector& Point) const
    {
        if (!m_bIsValid || m_Vertices.Num() < 3)
        {
            return false;
        }
        
        // Simple point-in-polygon test using ray casting
        int32 Intersections = 0;
        FVector RayEnd = Point + FVector(10000.0f, 0.0f, 0.0f);
        
        for (int32 i = 0; i < m_Vertices.Num(); ++i)
        {
            int32 NextIndex = (i + 1) % m_Vertices.Num();
            FVector V1 = m_Vertices[i];
            FVector V2 = m_Vertices[NextIndex];
            
            if (Utils::Get().LineIntersectsLine(Point, RayEnd, V1, V2))
            {
                Intersections++;
            }
        }
        
        return (Intersections % 2) == 1;
    }

    FVector NavMeshPolygon::GetClosestPoint(const FVector& Point) const
    {
        if (!m_bIsValid || m_Vertices.Num() < 3)
        {
            return Point;
        }
        
        FVector ClosestPoint = Point;
        float MinDistance = FLT_MAX;
        
        // Check distance to each edge
        for (int32 i = 0; i < m_Vertices.Num(); ++i)
        {
            int32 NextIndex = (i + 1) % m_Vertices.Num();
            FVector V1 = m_Vertices[i];
            FVector V2 = m_Vertices[NextIndex];
            
            FVector EdgeClosestPoint = Utils::Get().GetClosestPointOnLine(Point, V1, V2);
            float Distance = Utils::Get().Distance(Point, EdgeClosestPoint);
            
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                ClosestPoint = EdgeClosestPoint;
            }
        }
        
        return ClosestPoint;
    }

    // NavPath implementation
    NavPath::NavPath()
        : m_PathName(TEXT("UnnamedPath"))
        , m_bIsValid(false)
        , m_TotalDistance(0.0f)
    {
    }

    void NavPath::SetPathName(const FString& Name)
    {
        m_PathName = Name;
    }

    const FString& NavPath::GetPathName() const
    {
        return m_PathName;
    }

    void NavPath::SetPathPoints(const TArray<FVector>& Points)
    {
        m_PathPoints = Points;
        m_bIsValid = Points.Num() >= 2;
        CalculateTotalDistance();
    }

    TArray<FVector> NavPath::GetPathPoints() const
    {
        return m_PathPoints;
    }

    void NavPath::AddPathPoint(const FVector& Point)
    {
        m_PathPoints.Add(Point);
        m_bIsValid = m_PathPoints.Num() >= 2;
        CalculateTotalDistance();
    }

    void NavPath::RemovePathPoint(int32 Index)
    {
        if (Index >= 0 && Index < m_PathPoints.Num())
        {
            m_PathPoints.RemoveAt(Index);
            m_bIsValid = m_PathPoints.Num() >= 2;
            CalculateTotalDistance();
        }
    }

    void NavPath::ClearPathPoints()
    {
        m_PathPoints.Empty();
        m_bIsValid = false;
        m_TotalDistance = 0.0f;
    }

    int32 NavPath::GetPathPointCount() const
    {
        return m_PathPoints.Num();
    }

    void NavPath::SetIsValid(bool bValid)
    {
        m_bIsValid = bValid;
    }

    bool NavPath::IsValid() const
    {
        return m_bIsValid;
    }

    float NavPath::GetTotalDistance() const
    {
        return m_TotalDistance;
    }

    void NavPath::CalculateTotalDistance()
    {
        m_TotalDistance = 0.0f;
        
        for (int32 i = 0; i < m_PathPoints.Num() - 1; ++i)
        {
            m_TotalDistance += Utils::Get().Distance(m_PathPoints[i], m_PathPoints[i + 1]);
        }
    }

    FVector NavPath::GetPathPoint(int32 Index) const
    {
        if (Index >= 0 && Index < m_PathPoints.Num())
        {
            return m_PathPoints[Index];
        }
        return FVector::ZeroVector;
    }

    FVector NavPath::GetStartPoint() const
    {
        return m_PathPoints.Num() > 0 ? m_PathPoints[0] : FVector::ZeroVector;
    }

    FVector NavPath::GetEndPoint() const
    {
        return m_PathPoints.Num() > 0 ? m_PathPoints[m_PathPoints.Num() - 1] : FVector::ZeroVector;
    }

    FVector NavPath::GetPointAtDistance(float Distance) const
    {
        if (!m_bIsValid || m_PathPoints.Num() < 2)
        {
            return FVector::ZeroVector;
        }
        
        float CurrentDistance = 0.0f;
        
        for (int32 i = 0; i < m_PathPoints.Num() - 1; ++i)
        {
            float SegmentDistance = Utils::Get().Distance(m_PathPoints[i], m_PathPoints[i + 1]);
            
            if (CurrentDistance + SegmentDistance >= Distance)
            {
                float Alpha = (Distance - CurrentDistance) / SegmentDistance;
                return Utils::Get().LerpVector(m_PathPoints[i], m_PathPoints[i + 1], Alpha);
            }
            
            CurrentDistance += SegmentDistance;
        }
        
        return GetEndPoint();
    }

    float NavPath::GetDistanceToPoint(int32 PointIndex) const
    {
        if (PointIndex < 0 || PointIndex >= m_PathPoints.Num())
        {
            return 0.0f;
        }
        
        float Distance = 0.0f;
        
        for (int32 i = 0; i < PointIndex; ++i)
        {
            Distance += Utils::Get().Distance(m_PathPoints[i], m_PathPoints[i + 1]);
        }
        
        return Distance;
    }

    // NavQueryFilter implementation
    NavQueryFilter::NavQueryFilter()
        : m_FilterName(TEXT("UnnamedFilter"))
        , m_AgentRadius(50.0f)
        , m_AgentHeight(200.0f)
        , m_MaxSlope(45.0f)
        , m_MaxClimb(100.0f)
        , m_MaxJumpDown(200.0f)
        , m_MaxJumpUp(100.0f)
        , m_bAllowSwimming(false)
        , m_bAllowFlying(false)
        , m_bAllowClimbing(false)
    {
    }

    void NavQueryFilter::SetFilterName(const FString& Name)
    {
        m_FilterName = Name;
    }

    const FString& NavQueryFilter::GetFilterName() const
    {
        return m_FilterName;
    }

    void NavQueryFilter::SetAgentRadius(float Radius)
    {
        m_AgentRadius = Radius;
    }

    float NavQueryFilter::GetAgentRadius() const
    {
        return m_AgentRadius;
    }

    void NavQueryFilter::SetAgentHeight(float Height)
    {
        m_AgentHeight = Height;
    }

    float NavQueryFilter::GetAgentHeight() const
    {
        return m_AgentHeight;
    }

    void NavQueryFilter::SetMaxSlope(float Slope)
    {
        m_MaxSlope = Slope;
    }

    float NavQueryFilter::GetMaxSlope() const
    {
        return m_MaxSlope;
    }

    void NavQueryFilter::SetMaxClimb(float Climb)
    {
        m_MaxClimb = Climb;
    }

    float NavQueryFilter::GetMaxClimb() const
    {
        return m_MaxClimb;
    }

    void NavQueryFilter::SetMaxJumpDown(float JumpDown)
    {
        m_MaxJumpDown = JumpDown;
    }

    float NavQueryFilter::GetMaxJumpDown() const
    {
        return m_MaxJumpDown;
    }

    void NavQueryFilter::SetMaxJumpUp(float JumpUp)
    {
        m_MaxJumpUp = JumpUp;
    }

    float NavQueryFilter::GetMaxJumpUp() const
    {
        return m_MaxJumpUp;
    }

    void NavQueryFilter::SetAllowSwimming(bool bAllow)
    {
        m_bAllowSwimming = bAllow;
    }

    bool NavQueryFilter::GetAllowSwimming() const
    {
        return m_bAllowSwimming;
    }

    void NavQueryFilter::SetAllowFlying(bool bAllow)
    {
        m_bAllowFlying = bAllow;
    }

    bool NavQueryFilter::GetAllowFlying() const
    {
        return m_bAllowFlying;
    }

    void NavQueryFilter::SetAllowClimbing(bool bAllow)
    {
        m_bAllowClimbing = bAllow;
    }

    bool NavQueryFilter::GetAllowClimbing() const
    {
        return m_bAllowClimbing;
    }

    bool NavQueryFilter::IsValidForAgent(const FVector& AgentSize) const
    {
        return AgentSize.X <= m_AgentRadius * 2.0f && AgentSize.Z <= m_AgentHeight;
    }

    // NavigationSystem implementation
    NavigationSystem::NavigationSystem()
        : m_SystemName(TEXT("UnnamedNavigationSystem"))
        , m_bIsInitialized(false)
        , m_UpdateFrequency(1.0f)
        , m_LastUpdateTime(0.0f)
    {
    }

    void NavigationSystem::SetSystemName(const FString& Name)
    {
        m_SystemName = Name;
    }

    const FString& NavigationSystem::GetSystemName() const
    {
        return m_SystemName;
    }

    void NavigationSystem::Initialize()
    {
        if (m_bIsInitialized)
        {
            return;
        }
        
        m_bIsInitialized = true;
        Utils::Get().LogInfo(TEXT("NavigationSystem initialized: ") + m_SystemName);
    }

    void NavigationSystem::Shutdown()
    {
        if (!m_bIsInitialized)
        {
            return;
        }
        
        // Clear all navmeshes
        m_NavMeshes.Empty();
        
        // Clear all paths
        m_NavPaths.Empty();
        
        // Clear all query filters
        m_QueryFilters.Empty();
        
        m_bIsInitialized = false;
        Utils::Get().LogInfo(TEXT("NavigationSystem shutdown: ") + m_SystemName);
    }

    void NavigationSystem::SetUpdateFrequency(float Frequency)
    {
        m_UpdateFrequency = Frequency;
    }

    float NavigationSystem::GetUpdateFrequency() const
    {
        return m_UpdateFrequency;
    }

    void NavigationSystem::Update(float DeltaTime)
    {
        if (!m_bIsInitialized)
        {
            return;
        }
        
        float CurrentTime = Utils::Get().GetTimeSinceStart();
        if (CurrentTime - m_LastUpdateTime >= (1.0f / m_UpdateFrequency))
        {
            ProcessNavigation(DeltaTime);
            m_LastUpdateTime = CurrentTime;
        }
    }

    void NavigationSystem::ProcessNavigation(float DeltaTime)
    {
        // Update all navmeshes
        for (auto& NavMesh : m_NavMeshes)
        {
            if (NavMesh->IsValid())
            {
                // Process navmesh updates
            }
        }
        
        // Update all paths
        for (auto& Path : m_NavPaths)
        {
            if (Path->IsValid())
            {
                // Process path updates
            }
        }
    }

    void NavigationSystem::RegisterNavMesh(NavMesh* Mesh)
    {
        if (Mesh && !m_NavMeshes.Contains(Mesh))
        {
            m_NavMeshes.Add(Mesh);
            Utils::Get().LogInfo(TEXT("NavMesh registered: ") + Mesh->GetMeshName());
        }
    }

    void NavigationSystem::UnregisterNavMesh(NavMesh* Mesh)
    {
        if (Mesh)
        {
            m_NavMeshes.Remove(Mesh);
            Utils::Get().LogInfo(TEXT("NavMesh unregistered: ") + Mesh->GetMeshName());
        }
    }

    void NavigationSystem::RegisterNavPath(NavPath* Path)
    {
        if (Path && !m_NavPaths.Contains(Path))
        {
            m_NavPaths.Add(Path);
            Utils::Get().LogInfo(TEXT("NavPath registered: ") + Path->GetPathName());
        }
    }

    void NavigationSystem::UnregisterNavPath(NavPath* Path)
    {
        if (Path)
        {
            m_NavPaths.Remove(Path);
            Utils::Get().LogInfo(TEXT("NavPath unregistered: ") + Path->GetPathName());
        }
    }

    void NavigationSystem::RegisterQueryFilter(NavQueryFilter* Filter)
    {
        if (Filter && !m_QueryFilters.Contains(Filter))
        {
            m_QueryFilters.Add(Filter);
            Utils::Get().LogInfo(TEXT("NavQueryFilter registered: ") + Filter->GetFilterName());
        }
    }

    void NavigationSystem::UnregisterQueryFilter(NavQueryFilter* Filter)
    {
        if (Filter)
        {
            m_QueryFilters.Remove(Filter);
            Utils::Get().LogInfo(TEXT("NavQueryFilter unregistered: ") + Filter->GetFilterName());
        }
    }

    TArray<NavMesh*> NavigationSystem::GetNavMeshes() const
    {
        return m_NavMeshes;
    }

    TArray<NavPath*> NavigationSystem::GetNavPaths() const
    {
        return m_NavPaths;
    }

    TArray<NavQueryFilter*> NavigationSystem::GetQueryFilters() const
    {
        return m_QueryFilters;
    }

    NavMesh* NavigationSystem::FindNavMesh(const FString& MeshName) const
    {
        for (NavMesh* Mesh : m_NavMeshes)
        {
            if (Mesh->GetMeshName() == MeshName)
            {
                return Mesh;
            }
        }
        return nullptr;
    }

    NavPath* NavigationSystem::FindNavPath(const FString& PathName) const
    {
        for (NavPath* Path : m_NavPaths)
        {
            if (Path->GetPathName() == PathName)
            {
                return Path;
            }
        }
        return nullptr;
    }

    NavQueryFilter* NavigationSystem::FindQueryFilter(const FString& FilterName) const
    {
        for (NavQueryFilter* Filter : m_QueryFilters)
        {
            if (Filter->GetFilterName() == FilterName)
            {
                return Filter;
            }
        }
        return nullptr;
    }

    NavPath* NavigationSystem::FindPath(const FVector& Start, const FVector& End, NavQueryFilter* Filter)
    {
        if (!m_bIsInitialized)
        {
            return nullptr;
        }
        
        // Simple pathfinding implementation
        NavPath* Path = new NavPath();
        Path->SetPathName(TEXT("GeneratedPath"));
        
        // Add start and end points
        Path->AddPathPoint(Start);
        Path->AddPathPoint(End);
        
        // Validate path against navmeshes
        bool bValidPath = true;
        for (NavMesh* Mesh : m_NavMeshes)
        {
            if (!Mesh->IsPointInNavMesh(Start) || !Mesh->IsPointInNavMesh(End))
            {
                bValidPath = false;
                break;
            }
        }
        
        Path->SetIsValid(bValidPath);
        
        if (bValidPath)
        {
            RegisterNavPath(Path);
            Utils::Get().LogInfo(TEXT("Path found from ") + Start.ToString() + TEXT(" to ") + End.ToString());
        }
        else
        {
            Utils::Get().LogWarning(TEXT("Path not found from ") + Start.ToString() + TEXT(" to ") + End.ToString());
        }
        
        return Path;
    }

    bool NavigationSystem::IsPointInNavMesh(const FVector& Point) const
    {
        for (NavMesh* Mesh : m_NavMeshes)
        {
            if (Mesh->IsPointInNavMesh(Point))
            {
                return true;
            }
        }
        return false;
    }

    FVector NavigationSystem::FindNearestPointOnNavMesh(const FVector& Point) const
    {
        FVector NearestPoint = Point;
        float MinDistance = FLT_MAX;
        
        for (NavMesh* Mesh : m_NavMeshes)
        {
            FVector ClosestPoint = Mesh->FindNearestPointOnNavMesh(Point);
            float Distance = Utils::Get().Distance(Point, ClosestPoint);
            
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                NearestPoint = ClosestPoint;
            }
        }
        
        return NearestPoint;
    }

    int32 NavigationSystem::GetNavMeshCount() const
    {
        return static_cast<int32>(m_NavMeshes.Num());
    }

    int32 NavigationSystem::GetNavPathCount() const
    {
        return static_cast<int32>(m_NavPaths.Num());
    }

    int32 NavigationSystem::GetQueryFilterCount() const
    {
        return static_cast<int32>(m_QueryFilters.Num());
    }

    bool NavigationSystem::IsInitialized() const
    {
        return m_bIsInitialized;
    }

    // NavigationSystemManager implementation
    NavigationSystemManager* NavigationSystemManager::s_Instance = nullptr;

    NavigationSystemManager& NavigationSystemManager::Get()
    {
        if (!s_Instance)
        {
            s_Instance = new NavigationSystemManager();
        }
        return *s_Instance;
    }

    NavigationSystemManager::NavigationSystemManager()
        : m_bIsInitialized(false)
    {
    }

    void NavigationSystemManager::Initialize()
    {
        if (m_bIsInitialized)
        {
            return;
        }
        
        m_bIsInitialized = true;
        Utils::Get().LogInfo(TEXT("NavigationSystemManager initialized"));
    }

    void NavigationSystemManager::Shutdown()
    {
        if (!m_bIsInitialized)
        {
            return;
        }
        
        // Shutdown all navigation systems
        for (auto& System : m_NavigationSystems)
        {
            System->Shutdown();
        }
        m_NavigationSystems.Empty();
        
        m_bIsInitialized = false;
        Utils::Get().LogInfo(TEXT("NavigationSystemManager shutdown"));
    }

    void NavigationSystemManager::Update(float DeltaTime)
    {
        if (!m_bIsInitialized)
        {
            return;
        }
        
        for (auto& System : m_NavigationSystems)
        {
            System->Update(DeltaTime);
        }
    }

    NavigationSystem* NavigationSystemManager::CreateNavigationSystem(const FString& SystemName)
    {
        auto NewSystem = std::make_unique<NavigationSystem>();
        NewSystem->SetSystemName(SystemName);
        NewSystem->Initialize();
        
        NavigationSystem* SystemPtr = NewSystem.get();
        m_NavigationSystems.Add(std::move(NewSystem));
        
        Utils::Get().LogInfo(TEXT("NavigationSystem created: ") + SystemName);
        return SystemPtr;
    }

    void NavigationSystemManager::DestroyNavigationSystem(NavigationSystem* System)
    {
        for (auto it = m_NavigationSystems.begin(); it != m_NavigationSystems.end(); ++it)
        {
            if (it->get() == System)
            {
                System->Shutdown();
                m_NavigationSystems.erase(it);
                break;
            }
        }
    }

    NavigationSystem* NavigationSystemManager::FindNavigationSystem(const FString& SystemName) const
    {
        for (const auto& System : m_NavigationSystems)
        {
            if (System->GetSystemName() == SystemName)
            {
                return System.get();
            }
        }
        return nullptr;
    }

    TArray<NavigationSystem*> NavigationSystemManager::GetAllNavigationSystems() const
    {
        TArray<NavigationSystem*> Systems;
        for (const auto& System : m_NavigationSystems)
        {
            Systems.Add(System.get());
        }
        return Systems;
    }

    int32 NavigationSystemManager::GetNavigationSystemCount() const
    {
        return static_cast<int32>(m_NavigationSystems.Num());
    }

    bool NavigationSystemManager::IsInitialized() const
    {
        return m_bIsInitialized;
    }
}
