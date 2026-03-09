@echo off
echo Compiling the RPG Game (Woodcutting & Building)...
g++ -std=c++17 main.cpp Player.cpp Enemy.cpp -IC:/SFML/include -LC:/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -o game_v14.exe
if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b %errorlevel%
)
echo Done! Launching game_v14.exe...
start game_v14.exe
pause
