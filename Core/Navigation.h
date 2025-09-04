/*
 * UE4 Navigation System - Self-contained Navigation implementation
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
    class NavMesh;
    class NavPath;
    class NavQueryFilter;
    class NavigationSystem;

    // Navigation types
    enum class ENavigationQueryResult : uint8
    {
        Invalid = 0,
        Error = 1,
        Fail = 2,
        Success = 3
    };

    enum class ENavPathType : uint8
    {
        Invalid = 0,
        Regular = 1,
        Partial = 2,
        PartialFromPath = 3
    };

    enum class ENavAreaFlag : uint8
    {
        Default = 0,
        Jump = 1,
        Crouch = 2,
        Walk = 3,
        Swim = 4,
        Fly = 5,
        Custom1 = 6,
        Custom2 = 7,
        Custom3 = 8
    };

    // NavMesh - Navigation mesh data
    class NavMesh : public UObject
    {
    public:
        NavMesh();
        virtual ~NavMesh() = default;

        // Mesh data
        void AddVertex(const FVector& Vertex);
        void AddTriangle(int32 V0, int32 V1, int32 V2, ENavAreaFlag AreaFlag = ENavAreaFlag::Default);
        
        TArray<FVector> GetVertices() const { return m_Vertices; }
        TArray<int32> GetTriangles() const { return m_Triangles; }
        TArray<ENavAreaFlag> GetAreaFlags() const { return m_AreaFlags; }

        // Mesh queries
        bool IsPointInMesh(const FVector& Point) const;
        FVector FindClosestPointOnMesh(const FVector& Point) const;
        ENavAreaFlag GetAreaFlagAtPoint(const FVector& Point) const;

        // Mesh bounds
        void SetBounds(const FVector& Min, const FVector& Max);
        FVector GetMinBounds() const { return m_MinBounds; }
        FVector GetMaxBounds() const { return m_MaxBounds; }

        // Mesh info
        void SetMeshName(const FString& Name) { m_MeshName = Name; }
        const FString& GetMeshName() const { return m_MeshName; }

        int32 GetVertexCount() const { return static_cast<int32>(m_Vertices.Num()); }
        int32 GetTriangleCount() const { return static_cast<int32>(m_Triangles.Num()) / 3; }

    private:
        FString m_MeshName;
        TArray<FVector> m_Vertices;
        TArray<int32> m_Triangles;
        TArray<ENavAreaFlag> m_AreaFlags;
        FVector m_MinBounds;
        FVector m_MaxBounds;
    };

    // NavPath - Navigation path data
    class NavPath : public UObject
    {
    public:
        NavPath();
        virtual ~NavPath() = default;

        // Path data
        void AddPathPoint(const FVector& Point);
        void SetPathPoints(const TArray<FVector>& Points);
        TArray<FVector> GetPathPoints() const { return m_PathPoints; }

        // Path queries
        FVector GetPathPoint(int32 Index) const;
        int32 GetPathPointCount() const { return static_cast<int32>(m_PathPoints.Num()); }
        float GetPathLength() const;
        float GetPathLengthFromIndex(int32 StartIndex) const;

        // Path state
        void SetPathType(ENavPathType Type) { m_PathType = Type; }
        ENavPathType GetPathType() const { return m_PathType; }

        void SetQueryResult(ENavigationQueryResult Result) { m_QueryResult = Result; }
        ENavigationQueryResult GetQueryResult() const { return m_QueryResult; }

        // Path validation
        bool IsValid() const { return m_QueryResult == ENavigationQueryResult::Success && m_PathPoints.Num() > 0; }
        bool IsPartial() const { return m_PathType == ENavPathType::Partial || m_PathType == ENavPathType::PartialFromPath; }

        // Path info
        void SetPathName(const FString& Name) { m_PathName = Name; }
        const FString& GetPathName() const { return m_PathName; }

        // Path iteration
        int32 GetCurrentPathIndex() const { return m_CurrentPathIndex; }
        void SetCurrentPathIndex(int32 Index) { m_CurrentPathIndex = Index; }
        FVector GetCurrentPathPoint() const;
        FVector GetNextPathPoint() const;
        bool HasReachedEnd() const;

    private:
        FString m_PathName;
        TArray<FVector> m_PathPoints;
        ENavPathType m_PathType;
        ENavigationQueryResult m_QueryResult;
        int32 m_CurrentPathIndex;
    };

    // NavQueryFilter - Navigation query parameters
    class NavQueryFilter : public UObject
    {
    public:
        NavQueryFilter();
        virtual ~NavQueryFilter() = default;

        // Area costs
        void SetAreaCost(ENavAreaFlag Area, float Cost);
        float GetAreaCost(ENavAreaFlag Area) const;
        void SetAllAreaCosts(float Cost);

        // Area flags
        void SetAreaFlags(ENavAreaFlag Area, bool bEnabled);
        bool GetAreaFlags(ENavAreaFlag Area) const;
        void SetAllAreaFlags(bool bEnabled);

        // Query parameters
        void SetMaxSearchNodes(int32 MaxNodes) { m_MaxSearchNodes = MaxNodes; }
        int32 GetMaxSearchNodes() const { return m_MaxSearchNodes; }

        void SetHeuristicScale(float Scale) { m_HeuristicScale = Scale; }
        float GetHeuristicScale() const { return m_HeuristicScale; }

        void SetIsReachabilityTest(bool bTest) { m_bIsReachabilityTest = bTest; }
        bool IsReachabilityTest() const { return m_bIsReachabilityTest; }

        // Filter info
        void SetFilterName(const FString& Name) { m_FilterName = Name; }
        const FString& GetFilterName() const { return m_FilterName; }

    private:
        FString m_FilterName;
        TMap<ENavAreaFlag, float> m_AreaCosts;
        TMap<ENavAreaFlag, bool> m_AreaFlags;
        int32 m_MaxSearchNodes;
        float m_HeuristicScale;
        bool m_bIsReachabilityTest;
    };

    // NavigationSystem - Global navigation management
    class NavigationSystem : public UObject
    {
    public:
        NavigationSystem();
        virtual ~NavigationSystem() = default;

        // NavMesh management
        NavMesh* CreateNavMesh(const FString& MeshName);
        void DestroyNavMesh(NavMesh* Mesh);
        NavMesh* FindNavMesh(const FString& MeshName) const;
        TArray<NavMesh*> GetAllNavMeshes() const;

        // Pathfinding
        NavPath* FindPath(const FVector& Start, const FVector& End, NavQueryFilter* Filter = nullptr);
        NavPath* FindPathToActor(const FVector& Start, UObject* Target, NavQueryFilter* Filter = nullptr);
        NavPath* FindPathToLocation(const FVector& Start, const FVector& End, NavQueryFilter* Filter = nullptr);

        // Path validation
        bool IsPathValid(const NavPath* Path) const;
        bool IsLocationReachable(const FVector& Start, const FVector& End, NavQueryFilter* Filter = nullptr);

        // Random navigation
        FVector GetRandomPointInRadius(const FVector& Origin, float Radius, NavQueryFilter* Filter = nullptr);
        FVector GetRandomReachablePointInRadius(const FVector& Origin, float Radius, NavQueryFilter* Filter = nullptr);

        // Navigation queries
        FVector ProjectPointToNavigation(const FVector& Point, const FVector& Extent = FVector(100, 100, 100)) const;
        bool IsPointInNavigationBounds(const FVector& Point) const;
        FVector GetClosestPointOnNavigation(const FVector& Point) const;

        // NavMesh queries
        NavMesh* GetNavMeshAtLocation(const FVector& Location) const;
        TArray<NavMesh*> GetNavMeshesInBounds(const FVector& Min, const FVector& Max) const;

        // Query filter management
        NavQueryFilter* CreateQueryFilter(const FString& FilterName);
        void DestroyQueryFilter(NavQueryFilter* Filter);
        NavQueryFilter* FindQueryFilter(const FString& FilterName) const;
        TArray<NavQueryFilter*> GetAllQueryFilters() const;

        // Navigation bounds
        void SetNavigationBounds(const FVector& Min, const FVector& Max);
        FVector GetNavigationMinBounds() const { return m_NavigationMinBounds; }
        FVector GetNavigationMaxBounds() const { return m_NavigationMaxBounds; }

        // System info
        void SetSystemName(const FString& Name) { m_SystemName = Name; }
        const FString& GetSystemName() const { return m_SystemName; }

        // System state
        void EnableNavigation() { m_bNavigationEnabled = true; }
        void DisableNavigation() { m_bNavigationEnabled = false; }
        bool IsNavigationEnabled() const { return m_bNavigationEnabled; }

        // Updates
        void UpdateNavigation(float DeltaTime);

    private:
        FString m_SystemName;
        TArray<std::unique_ptr<NavMesh>> m_NavMeshes;
        TArray<std::unique_ptr<NavQueryFilter>> m_QueryFilters;
        FVector m_NavigationMinBounds;
        FVector m_NavigationMaxBounds;
        bool m_bNavigationEnabled;

        // Pathfinding algorithms
        NavPath* FindPathAStar(const FVector& Start, const FVector& End, NavQueryFilter* Filter);
        NavPath* FindPathDijkstra(const FVector& Start, const FVector& End, NavQueryFilter* Filter);
        
        // Helper functions
        float CalculateHeuristic(const FVector& Start, const FVector& End) const;
        TArray<FVector> GetNeighbors(const FVector& Point, NavMesh* Mesh) const;
        bool IsValidPathPoint(const FVector& Point, NavMesh* Mesh) const;
    };

    // Navigation System - Global navigation management
    class NavigationSystemManager
    {
    public:
        static NavigationSystemManager& Get();

        // System management
        NavigationSystem* CreateNavigationSystem(const FString& SystemName);
        void DestroyNavigationSystem(NavigationSystem* System);
        NavigationSystem* FindNavigationSystem(const FString& SystemName) const;
        NavigationSystem* GetDefaultNavigationSystem() const;
        TArray<NavigationSystem*> GetAllNavigationSystems() const;

        // Global pathfinding
        NavPath* FindPath(const FVector& Start, const FVector& End, NavQueryFilter* Filter = nullptr);
        NavPath* FindPathToActor(const FVector& Start, UObject* Target, NavQueryFilter* Filter = nullptr);

        // Global queries
        bool IsLocationReachable(const FVector& Start, const FVector& End, NavQueryFilter* Filter = nullptr);
        FVector GetRandomPointInRadius(const FVector& Origin, float Radius, NavQueryFilter* Filter = nullptr);
        FVector ProjectPointToNavigation(const FVector& Point, const FVector& Extent = FVector(100, 100, 100)) const;

        // System updates
        void UpdateNavigation(float DeltaTime);

        // Cleanup
        void Cleanup();

    private:
        NavigationSystemManager() = default;
        ~NavigationSystemManager() = default;

        NavigationSystemManager(const NavigationSystemManager&) = delete;
        NavigationSystemManager& operator=(const NavigationSystemManager&) = delete;

        TArray<std::unique_ptr<NavigationSystem>> m_NavigationSystems;
        NavigationSystem* m_DefaultSystem;

        static NavigationSystemManager* s_Instance;
    };

    // Global access macros
    #define NAV_SYSTEM_MANAGER UE4SDK::NavigationSystemManager::Get()
    #define CREATE_NAV_MESH(Name) NAV_SYSTEM_MANAGER.GetDefaultNavigationSystem()->CreateNavMesh(Name)
    #define CREATE_NAV_PATH() GOBJECTS.CreateObject(TEXT("NavPath"))
    #define CREATE_QUERY_FILTER(Name) NAV_SYSTEM_MANAGER.GetDefaultNavigationSystem()->CreateQueryFilter(Name)
    #define FIND_PATH(Start, End) NAV_SYSTEM_MANAGER.FindPath(Start, End)
}
