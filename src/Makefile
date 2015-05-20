GPP=g++
PTHREAD=-pthread
OBJECTS=main.o \
		Menu/Menu.o \
		Field/Field.o \
		Player/Player.o \
		Window/Window.o \
		MovingEntity/MovingEntity.o \
		SpritedEntity/SpritedEntity.o \
		Position/Position.o \
		SolidObject/SolidObject.o \
		Wall/Wall.o \
		BreakableWall/BreakableWall.o \
		UnbreakableWall/UnbreakableWall.o \
		TimedObject/TimedObject.o \
		Bomb/Bomb.o \
		GameObject/GameObject.o \
		Flame/Flame.o \
		GameSaver/GameSaver.o \
		GameLoader/GameLoader.o \
		BaseSocket/BaseSocket.o \
		Connection/Connection.o \
		TcpClient/TcpClient.o \
		TcpServer/TcpServer.o \
		ConsoleCommand/ConsoleCommand.o 

CFLAGS=-std=c++11 -O2 -Wall -ggdb -lX11
SDL_FLAGS=-lSDL_ttf `pkg-config sdl --libs --cflags`

BIN=Bomberman
BIN_DIR=../bin

all: $(OBJECTS)
	$(GPP) -o $(BIN_DIR)/$(BIN) $(OBJECTS) $(CFLAGS) $(PTHREAD) $(SDL_FLAGS)

main.o: main.cpp
	$(GPP) -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)

Field/Field.o: Field/Field.cpp
	$(GPP) -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)

Player/Player.o: Player/Player.cpp
	$(GPP) -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)

Window/Window.o: Window/Window.cpp
	$(GPP) -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)

MovingEntity/MovingEntity.o: MovingEntity/MovingEntity.cpp
	$(GPP) -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)

SpritedEntity/SpritedEntity.o: SpritedEntity/SpritedEntity.cpp
	$(GPP) -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)

Position/Position.o: Position/Position.cpp
	$(GPP) -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)

SolidObject/SolidObject.o: SolidObject/SolidObject.cpp
	$(GPP) -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)

Wall/Wall.o: Wall/Wall.cpp
	$(GPP) -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)

BreakableWall/BreakableWall.o: BreakableWall/BreakableWall.cpp
	$(GPP) -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)

UnbreakableWall/UnbreakableWall.o: UnbreakableWall/UnbreakableWall.cpp
	$(GPP) -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)

TimedObject/TimedObject.o: TimedObject/TimedObject.cpp
	$(GPP) -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)
	
Bomb/Bomb.o: Bomb/Bomb.cpp
	$(GPP) -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)

GameObject/GameObject.o: GameObject/GameObject.cpp
	$(GPP) -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)

Flame/Flame.o: Flame/Flame.cpp
	$(GPP) -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)

GameSaver/GameSaver.o: GameSaver/GameSaver.cpp
	$(GPP) -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)

GameLoader/GameLoader.o: GameLoader/GameLoader.cpp
	$(GPP) -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)

SaveButton/SaveButton.o: SaveButton/SaveButton.cpp
	$(GPP)  -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)
	
BaseSocket/BaseSocket.o: BaseSocket/BaseSocket.cpp
	$(GPP)  -c -o $@ $< $(CFLAGS)

Connection/Connection.o: Connection/Connection.cpp
	$(GPP)  -c -o $@ $< $(CFLAGS)

TcpClient/TcpClient.o: TcpClient/TcpClient.cpp
	$(GPP)  -c -o $@ $< $(CFLAGS)

TcpServer/TcpServer.o: TcpServer/TcpServer.cpp
	$(GPP)  -c -o $@ $< $(CFLAGS)

ConsoleCommand/ConsoleCommand.o: ConsoleCommand/ConsoleCommand.cpp
	$(GPP)  -c -o $@ $< $(CFLAGS)

Menu/Menu.o: Menu/Menu.cpp
	$(GPP)  -c -o $@ $< $(CFLAGS) $(SDL_FLAGS)

clean:
	rm -f $(OBJECTS)
	rm -f $(BIN_DIR)/$(BIN)
