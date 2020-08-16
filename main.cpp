#include <SFML\Graphics.hpp>
#include<SFML\Audio.hpp>
#include "Map.h"
#include <sstream>
#include <iostream>
#include <vector>
using namespace sf;
int dir,//угол поворота
	speed,//направление вверх
	dx = 1,//движение камеры и птицы, с одной скоростью
	dxView = 1,
	positionBird = 100;//изначальная позция птицы
bool fall = false,//падение
	 start = false,//начало игры
     hitSound = true;
std::vector <int> vec = { 10000 };
void interaction(Sprite &s, Sound &hit) {//взаимодействие
	Vector2f position = s.getPosition();//позиция птички
	for (int i = position.y / 28; i < (position.y + 28) / 28; i++) 
		for (int j = position.x / 41; j < (position.x + 27) / 41; j++) {
			if (Map[i][j] == 'r' || Map[i][j] == 't') { //если ударяется с трубами
				fall = true; //начать падение
				dx = 0; dxView = 0; //отменить пермещение птицы и камеры по икс
				if (hitSound) { hit.play(); hitSound = false; }
			} 
			
		}
	
	if (position.y <= 0)  s.setPosition(position.x, 0);//если врезается вверх, ничего не происходит
	if (position.y >= 248) {//если ударяется об землю
		s.setPosition(position.x, 248); //остаётся внизу
		fall = true; //падение
		dx = 0; dxView = 0;//отменить передвижение и камеры и птицы
		if (hitSound) { hit.play(); hitSound = false; }//Добавить музыку
	}
	if (position.x + 202 >= 1600) dxView = 0;//если конец карты то останавливаем камеру
}
void update(Sprite &s, Sound &hit) {
	s.move(dx, speed);//движение птицы
	s.setRotation(dir);//поворот птицы
	interaction(s, hit);//вызов функций взаимодействия
}
int main() {
	
	RenderWindow window(sf::VideoMode(302, 302), "Flappy Bird");
	window.setFramerateLimit(60);//60 fps
	View view;
	view.reset(sf::FloatRect(0, 0, 302, 302));//установка размера камеры
	

	float currentFrame = 0; //кадры спрайтов
	
	////////////////Sound///////////////
	SoundBuffer bufferHit, bufferFly;
	bufferHit.loadFromFile("music/hit.flac");
	bufferFly.loadFromFile("music/fly.flac");

	Sound soundHit(bufferHit), soundFly(bufferFly);

	///////////////////////////////////

	Image birdIm;
	birdIm.loadFromFile("images/bird.png");
	birdIm.createMaskFromColor(Color(255, 255, 255));

	Image collsIm;
	collsIm.loadFromFile("images/colls.png");

	Texture t1, t2, t3;
	t1.loadFromFile("images/background.jpg");
	t2.loadFromImage(birdIm);
	t3.loadFromImage(collsIm);
	Sprite back(t1), bird(t2), colls(t3);
	///////////////Bird///////////////////
	bird.setTextureRect(IntRect(0,0,27.5,28));
	bird.setPosition(positionBird, 140);
	bird.setOrigin(20,20);
	///////////////////////////////////////

	/////////////Text//////////////////////
	Font font;
	font.loadFromFile("flappy.ttf");
	Text text("",font, 20);

	//////////////////////////////////////
	Clock clock;


	while (window.isOpen()) {
		
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time /= 600;

		

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
		}
		
		/////////////////////Управление////////////////////////////
		if (Keyboard::isKeyPressed(Keyboard::Enter)) start = true;
		if (start) {
			if (Keyboard::isKeyPressed(Keyboard::Space) && (!fall)) {
					dir = -20; speed = -3;
					currentFrame += 0.005*time;
					if (currentFrame > 3) currentFrame = 0;
					bird.setTextureRect(IntRect(27.5 * int(currentFrame), 0, 27.5, 28));
					soundFly.play();
			}
				else { dir = 20; speed = 3 + 0.0015; }

				view.move(dxView, 0);
				update(bird, soundHit);
				window.setView(view);
		}
		////////////////////////////////////////////////////////////////
		window.clear();
		window.draw(back);//рисование фона 
		
		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				if (Map[i][j] == 'r') { colls.setTextureRect(IntRect(0, 0, 38, 25)); colls.setPosition(j * 40, i * 25);  }
				if (Map[i][j] == 't') { colls.setTextureRect(IntRect(0, 28, 41, 25)); colls.setPosition(j * 40, i * 25);  }
				window.draw(colls);
			}
		}
		window.draw(bird);//рисоване птицы

		
		if (!start) {
			text.setString("PRESS ENTER");
			text.setPosition(130, 40);
			window.draw(text);
		}
		
		window.display();
	}
	return 0;
}