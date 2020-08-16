#include <SFML\Graphics.hpp>
#include<SFML\Audio.hpp>
#include "Map.h"
#include <sstream>
#include <iostream>
#include <vector>
using namespace sf;
int dir,//���� ��������
	speed,//����������� �����
	dx = 1,//�������� ������ � �����, � ����� ���������
	dxView = 1,
	positionBird = 100;//����������� ������ �����
bool fall = false,//�������
	 start = false,//������ ����
     hitSound = true;
std::vector <int> vec = { 10000 };
void interaction(Sprite &s, Sound &hit) {//��������������
	Vector2f position = s.getPosition();//������� ������
	for (int i = position.y / 28; i < (position.y + 28) / 28; i++) 
		for (int j = position.x / 41; j < (position.x + 27) / 41; j++) {
			if (Map[i][j] == 'r' || Map[i][j] == 't') { //���� ��������� � �������
				fall = true; //������ �������
				dx = 0; dxView = 0; //�������� ���������� ����� � ������ �� ���
				if (hitSound) { hit.play(); hitSound = false; }
			} 
			
		}
	
	if (position.y <= 0)  s.setPosition(position.x, 0);//���� ��������� �����, ������ �� ����������
	if (position.y >= 248) {//���� ��������� �� �����
		s.setPosition(position.x, 248); //������� �����
		fall = true; //�������
		dx = 0; dxView = 0;//�������� ������������ � ������ � �����
		if (hitSound) { hit.play(); hitSound = false; }//�������� ������
	}
	if (position.x + 202 >= 1600) dxView = 0;//���� ����� ����� �� ������������� ������
}
void update(Sprite &s, Sound &hit) {
	s.move(dx, speed);//�������� �����
	s.setRotation(dir);//������� �����
	interaction(s, hit);//����� ������� ��������������
}
int main() {
	
	RenderWindow window(sf::VideoMode(302, 302), "Flappy Bird");
	window.setFramerateLimit(60);//60 fps
	View view;
	view.reset(sf::FloatRect(0, 0, 302, 302));//��������� ������� ������
	

	float currentFrame = 0; //����� ��������
	
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
		
		/////////////////////����������////////////////////////////
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
		window.draw(back);//��������� ���� 
		
		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				if (Map[i][j] == 'r') { colls.setTextureRect(IntRect(0, 0, 38, 25)); colls.setPosition(j * 40, i * 25);  }
				if (Map[i][j] == 't') { colls.setTextureRect(IntRect(0, 28, 41, 25)); colls.setPosition(j * 40, i * 25);  }
				window.draw(colls);
			}
		}
		window.draw(bird);//�������� �����

		
		if (!start) {
			text.setString("PRESS ENTER");
			text.setPosition(130, 40);
			window.draw(text);
		}
		
		window.display();
	}
	return 0;
}