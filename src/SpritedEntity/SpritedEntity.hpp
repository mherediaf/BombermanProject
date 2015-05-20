#ifndef SPRITED_ENTITY_HPP
#define SPRITED_ENTITY_HPP

#include <SDL/SDL.h>
#include "../GameEntity/GameEntity.hpp"
#include <string>

class SpritedEntity: public GameEntity{
	private:
		bool imageIsSet();
	
	protected:
		bool image_set;
		SDL_Surface* sprite;
		std::string* spritePath;

	public:
		SpritedEntity();
		~SpritedEntity();

		SDL_Surface* getSprite();
		std::string* getSpritePath() const;
		void loadBMP(char* path);
		void loadBMP(const char* path);
		void loadBMP(std::string& path);
};

#endif
