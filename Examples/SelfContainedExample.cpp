/*
 * UE4 SDK Self-Contained Example
 * This example works without any Runtime folder dependencies!
 * Clean, vibe coder style example ✨
 */

#include "../UE4SDK.h"

// Example function showing self-contained SDK usage
void SelfContainedSDKExample()
{
    // Initialize the SDK
    if (!UE4_SDK.Initialize())
    {
        UE_LOG(LogUE4SDK, Error, TEXT("Failed to initialize UE4 SDK"));
        return;
    }
    
    // Get the engine and utils
    auto& Engine = UE4_ENGINE;
    auto& Utils = UE4_UTILS;
    
    // Create some vectors and do math operations
    FVector Start = FVector(0, 0, 0);
    FVector End = FVector(100, 100, 100);
    
    // Calculate distance
    float Distance = Utils.Distance(Start, End);
    UE_LOG(LogUE4SDK, Log, TEXT("Distance: %s"), *Utils.ToString(Distance));
    
    // Create a rotator
    FRotator Rotation = FRotator(0, 45, 0);
    UE_LOG(LogUE4SDK, Log, TEXT("Rotation: %s"), *Rotation.ToString());
    
    // Create a transform
    FTransform Transform = FTransform(Rotation, Start, FVector::OneVector);
    UE_LOG(LogUE4SDK, Log, TEXT("Transform: %s"), *Transform.ToString());
    
    // Work with arrays
    TArray<FVector> VectorArray;
    VectorArray.Add(FVector(1, 2, 3));
    VectorArray.Add(FVector(4, 5, 6));
    VectorArray.Add(FVector(7, 8, 9));
    
    UE_LOG(LogUE4SDK, Log, TEXT("Array size: %d"), VectorArray.Num());
    
    // Work with maps
    TMap<FString, int32> ScoreMap;
    ScoreMap.Add(TEXT("Player1"), 100);
    ScoreMap.Add(TEXT("Player2"), 200);
    ScoreMap.Add(TEXT("Player3"), 150);
    
    UE_LOG(LogUE4SDK, Log, TEXT("Player1 score: %d"), ScoreMap[TEXT("Player1")]);
    
    // Work with sets
    TSet<FString> TagSet;
    TagSet.Add(TEXT("Player"));
    TagSet.Add(TEXT("Alive"));
    TagSet.Add(TEXT("Moving"));
    
    UE_LOG(LogUE4SDK, Log, TEXT("Has Player tag: %s"), TagSet.Contains(TEXT("Player")) ? TEXT("Yes") : TEXT("No"));
    
    // String operations
    FString PlayerName = TEXT("VibeCoder");
    FString WelcomeMessage = Utils.FormatString(TEXT("Welcome %s! You have %d points."), *PlayerName, 1000);
    UE_LOG(LogUE4SDK, Log, TEXT("%s"), *WelcomeMessage);
    
    // Math utilities
    float LerpedValue = Utils.Lerp(0.0f, 100.0f, 0.5f);
    UE_LOG(LogUE4SDK, Log, TEXT("Lerped value: %s"), *Utils.ToString(LerpedValue));
    
    // Random utilities
    float RandomFloat = Utils.RandomFloat(0.0f, 100.0f);
    FVector RandomVector = Utils.RandomVector(FVector(-100, -100, -100), FVector(100, 100, 100));
    UE_LOG(LogUE4SDK, Log, TEXT("Random float: %s"), *Utils.ToString(RandomFloat));
    UE_LOG(LogUE4SDK, Log, TEXT("Random vector: %s"), *RandomVector.ToString());
    
    // Color operations
    FLinearColor Red = FLinearColor::Red;
    FLinearColor Blue = FLinearColor::Blue;
    FLinearColor Mixed = Utils.LerpColor(Red, Blue, 0.5f);
    UE_LOG(LogUE4SDK, Log, TEXT("Mixed color: %s"), *Mixed.ToString());
    
    // Time operations
    FString CurrentTime = Utils.GetCurrentTimeString();
    UE_LOG(LogUE4SDK, Log, TEXT("Current time: %s"), *CurrentTime);
    
    // Performance timing
    Utils.StartTimer(TEXT("MyTimer"));
    
    // Do some work
    for (int32 i = 0; i < 1000; ++i)
    {
        Utils.RandomFloat(0.0f, 1.0f);
    }
    
    float ElapsedTime = Utils.StopTimer(TEXT("MyTimer"));
    UE_LOG(LogUE4SDK, Log, TEXT("Timer elapsed: %s seconds"), *Utils.ToString(ElapsedTime));
    
    // File operations
    FString TestString = TEXT("Hello, UE4 SDK!");
    FString FilePath = TEXT("test.txt");
    
    if (Utils.SaveStringToFile(TestString, FilePath))
    {
        UE_LOG(LogUE4SDK, Log, TEXT("File saved successfully"));
        
        FString LoadedString = Utils.LoadStringFromFile(FilePath);
        UE_LOG(LogUE4SDK, Log, TEXT("Loaded string: %s"), *LoadedString);
    }
    
    // Event system
    Utils.BindEvent(TEXT("MyEvent"), []()
    {
        UE_LOG(LogUE4SDK, Log, TEXT("MyEvent triggered!"));
    });
    
    Utils.TriggerEvent(TEXT("MyEvent"));
    
    // Platform info
    FString PlatformName = Utils.GetPlatformName();
    FString EngineVersion = Utils.GetEngineVersion();
    UE_LOG(LogUE4SDK, Log, TEXT("Platform: %s"), *PlatformName);
    UE_LOG(LogUE4SDK, Log, TEXT("Engine version: %s"), *EngineVersion);
    
    // Debug drawing (simulated)
    Utils.DrawDebugLine(Start, End, FLinearColor::Red, 5.0f);
    Utils.DrawDebugSphere(FVector(50, 50, 50), 25.0f, FLinearColor::Blue, 5.0f);
    
    Utils.LogInfo(TEXT("Self-contained SDK example completed successfully!"));
}

// Example showing advanced features
void AdvancedSelfContainedExample()
{
    auto& Utils = UE4_UTILS;
    
    // Array operations
    TArray<int32> Numbers;
    for (int32 i = 1; i <= 10; ++i)
    {
        Numbers.Add(i);
    }
    
    // Filter even numbers
    auto EvenNumbers = Utils.Filter(Numbers, [](int32 Number) { return Number % 2 == 0; });
    UE_LOG(LogUE4SDK, Log, TEXT("Even numbers count: %d"), EvenNumbers.Num());
    
    // Map numbers to strings
    auto NumberStrings = Utils.Map(Numbers, [](int32 Number) { return Utils.ToString(Number); });
    UE_LOG(LogUE4SDK, Log, TEXT("Number strings count: %d"), NumberStrings.Num());
    
    // Shuffle array
    auto ShuffledNumbers = Utils.ShuffleArray(Numbers);
    UE_LOG(LogUE4SDK, Log, TEXT("First shuffled number: %d"), ShuffledNumbers[0]);
    
    // Get random element
    int32 RandomNumber = Utils.GetRandomElement(Numbers);
    UE_LOG(LogUE4SDK, Log, TEXT("Random number: %d"), RandomNumber);
    
    // String manipulation
    FString LongString = TEXT("Hello,World,UE4,SDK,Is,Amazing");
    auto StringParts = Utils.SplitString(LongString, TEXT(","));
    UE_LOG(LogUE4SDK, Log, TEXT("Split into %d parts"), StringParts.Num());
    
    FString JoinedString = Utils.JoinString(StringParts, TEXT(" | "));
    UE_LOG(LogUE4SDK, Log, TEXT("Joined: %s"), *JoinedString);
    
    // Color utilities
    FLinearColor HSVColor = Utils.HSVToRGB(120.0f, 1.0f, 1.0f); // Green
    UE_LOG(LogUE4SDK, Log, TEXT("HSV to RGB: %s"), *HSVColor.ToString());
    
    FString HexColor = Utils.ColorToHex(FLinearColor::Red);
    UE_LOG(LogUE4SDK, Log, TEXT("Color to hex: %s"), *HexColor);
    
    // Angle utilities
    float Degrees = 180.0f;
    float Radians = Utils.DegreesToRadians(Degrees);
    float BackToDegrees = Utils.RadiansToDegrees(Radians);
    UE_LOG(LogUE4SDK, Log, TEXT("Degrees: %s, Radians: %s, Back: %s"), 
           *Utils.ToString(Degrees), *Utils.ToString(Radians), *Utils.ToString(BackToDegrees));
    
    // Interpolation utilities
    float EaseInValue = Utils.EaseIn(0.5f);
    float EaseOutValue = Utils.EaseOut(0.5f);
    float EaseInOutValue = Utils.EaseInOut(0.5f);
    
    UE_LOG(LogUE4SDK, Log, TEXT("EaseIn: %s, EaseOut: %s, EaseInOut: %s"),
           *Utils.ToString(EaseInValue), *Utils.ToString(EaseOutValue), *Utils.ToString(EaseInOutValue));
    
    // Random utilities
    FString RandomString = Utils.GetRandomString(10);
    FString RandomName = Utils.GetRandomName();
    FString RandomEmail = Utils.GetRandomEmail();
    
    UE_LOG(LogUE4SDK, Log, TEXT("Random string: %s"), *RandomString);
    UE_LOG(LogUE4SDK, Log, TEXT("Random name: %s"), *RandomName);
    UE_LOG(LogUE4SDK, Log, TEXT("Random email: %s"), *RandomEmail);
    
    // Memory info
    int64 MemoryUsage = Utils.GetMemoryUsage();
    int64 AvailableMemory = Utils.GetAvailableMemory();
    
    UE_LOG(LogUE4SDK, Log, TEXT("Memory usage: %lld bytes"), MemoryUsage);
    UE_LOG(LogUE4SDK, Log, TEXT("Available memory: %lld bytes"), AvailableMemory);
    
    Utils.LogInfo(TEXT("Advanced self-contained example completed!"));
}

// Example showing game-like functionality
void GameLikeExample()
{
    auto& Utils = UE4_UTILS;
    
    // Player data
    struct Player
    {
        FString Name;
        int32 Score;
        FVector Position;
        bool bAlive;
        
        Player(const FString& InName, int32 InScore, const FVector& InPosition, bool InAlive = true)
            : Name(InName), Score(InScore), Position(InPosition), bAlive(InAlive) {}
    };
    
    // Create players
    TArray<Player> Players;
    Players.Add(Player(TEXT("VibeCoder"), 1000, FVector(0, 0, 0)));
    Players.Add(Player(TEXT("GameDev"), 1500, FVector(100, 0, 0)));
    Players.Add(Player(TEXT("UE4Master"), 2000, FVector(200, 0, 0)));
    
    // Find player with highest score
    Player* HighestScorePlayer = nullptr;
    int32 HighestScore = 0;
    
    for (auto& Player : Players)
    {
        if (Player.Score > HighestScore)
        {
            HighestScore = Player.Score;
            HighestScorePlayer = &Player;
        }
    }
    
    if (HighestScorePlayer)
    {
        UE_LOG(LogUE4SDK, Log, TEXT("Highest score player: %s with %d points"), 
               *HighestScorePlayer->Name, HighestScorePlayer->Score);
    }
    
    // Calculate distances between players
    for (int32 i = 0; i < Players.Num(); ++i)
    {
        for (int32 j = i + 1; j < Players.Num(); ++j)
        {
            float Distance = Utils.Distance(Players[i].Position, Players[j].Position);
            UE_LOG(LogUE4SDK, Log, TEXT("Distance between %s and %s: %s"),
                   *Players[i].Name, *Players[j].Name, *Utils.ToString(Distance));
        }
    }
    
    // Simulate player movement
    for (auto& Player : Players)
    {
        FVector RandomMovement = Utils.RandomVector(FVector(-10, -10, 0), FVector(10, 10, 0));
        Player.Position += RandomMovement;
        
        UE_LOG(LogUE4SDK, Log, TEXT("%s moved to: %s"), 
               *Player.Name, *Player.Position.ToString());
    }
    
    // Simulate scoring
    for (auto& Player : Players)
    {
        int32 ScoreIncrease = Utils.RandomInt(10, 100);
        Player.Score += ScoreIncrease;
        
        UE_LOG(LogUE4SDK, Log, TEXT("%s gained %d points, new score: %d"),
               *Player.Name, ScoreIncrease, Player.Score);
    }
    
    // Create a leaderboard
    TArray<Player*> SortedPlayers;
    for (auto& Player : Players)
    {
        SortedPlayers.Add(&Player);
    }
    
    // Sort by score (descending)
    SortedPlayers.Sort([](const Player* A, const Player* B) { return A->Score > B->Score; });
    
    UE_LOG(LogUE4SDK, Log, TEXT("=== LEADERBOARD ==="));
    for (int32 i = 0; i < SortedPlayers.Num(); ++i)
    {
        UE_LOG(LogUE4SDK, Log, TEXT("%d. %s - %d points"), 
               i + 1, *SortedPlayers[i]->Name, SortedPlayers[i]->Score);
    }
    
    Utils.LogInfo(TEXT("Game-like example completed!"));
}
