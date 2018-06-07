#define #include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sstream>

using namespace sf;
using namespace std;

int main()
{
	
	std::size_t maxTextLength = 5;
	std::string string;
	sf::Font font;
	font.loadFromFile("C:\\Users\\sobieraj95\\Desktop\\test visual\\Verdana.ttf");

	sf::Text text2;
	text2.setString("Prosze podac ilosc miast: ");
	text2.setFont(font);
	text2.setCharacterSize(20);
	text2.setPosition(20, 20);
	text2.setColor(sf::Color::Black);

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(20);
	text.setPosition(120, 50);
	text.setColor(sf::Color::Black);

	//======================================= PIERWSZE OKNO Z PYTANIEM O ILOSC MIAST ===========================================

	sf::RenderWindow win(VideoMode(300, 100), "Komiwojazer by Sobierajski & Socha", Style::Close);
	while (win.isOpen())
	{
		sf::Event event;
		while (win.pollEvent(event))
		{
			if (event.type == Event::Closed)
				win.close();
			else if (event.type == Event::TextEntered)
			{
				sf::Uint32 unicode = event.text.unicode;
				if (unicode == 8 && string.size() > 0)
				{
					string.erase(string.size() - 1, 1);
					text.setString(string);
				}
				else if ((unicode >= 48 && unicode <= 57) && string.size() < maxTextLength)
				{
					string += static_cast<char>(unicode);
					text.setString(string);
				}
			}else if (sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				win.close();
		}

		sf::Texture tekstura_tlo;
		tekstura_tlo.loadFromFile("kartka.png");
		sf::Sprite tlo;
		tlo.setTexture(tekstura_tlo);
		tlo.setPosition(-150, -150);
		win.draw(tlo);

		sf::Texture przycisk;
		przycisk.loadFromFile("C:\\Users\\sobieraj95\\Desktop\\komiwojazer_projekt\\button.png");
		sf::Sprite button;
		button.setTexture(przycisk);
		button.setPosition(210, 50);		
		win.draw(button);
		
		win.draw(text2);
		win.draw(text);
		win.display();
	}

	int N;
	istringstream iss(string);
	iss >> N;

	
	clock_t zacznij = clock();
	int** random = (int**)malloc(N*sizeof(int *));
	for (int i = 0; i < N; i++)
		random[i] = (int*)malloc(2 * sizeof(int));

	int** posortowane = (int**)malloc(N*sizeof(int *));
	for (int i = 0; i < N; i++)
		posortowane[i] = (int*)malloc(2 * sizeof(int));

	srand(time(NULL));
	for (int i = 0; i<N; i++)
	{
		random[i][0] = rand() % 525+35;
		random[i][1] = rand() % 500+35;
	}

	for (int i = 0; i<N; i++)
	{
		posortowane[i][0] = -1;
		posortowane[i][1] = -1;
	}

	int pkt_sasiedni = 0,usun;
	bool pierwszy;
	long long dystans_calkowity = 0;

	posortowane[0][0] = random[0][0];
	posortowane[0][1] = random[0][1];

	random[0][0] = -1;
	random[0][1] = -1;
	
	//==================================== GŁOWNA PETLA PROGRAMU, ALGORYTM =============================
	for (int i = 0; i<N - 1; i++)
	{
		int dystans = 0, najblizsza_odleglosc = 0, dystans_licz = 0;
		pierwszy = true;
		usun = 0;

		for (int j = 1; j<N; j++)
		{	
			if (random[j][0] != -1 && random[j][1] != -1)
			{
				dystans = sqrt(pow(posortowane[i][0] - random[j][0], 2) + pow(posortowane[i][1] - random[j][1], 2));

				if (pierwszy == true)
				{
					najblizsza_odleglosc = dystans;
					pierwszy = false;
				}

				if (dystans > 0 && dystans <= najblizsza_odleglosc)
				{
					najblizsza_odleglosc = dystans;
					
					posortowane[i + 1][0] = random[j][0];
					posortowane[i + 1][1] = random[j][1];

					dystans_licz = najblizsza_odleglosc;
					usun = j;
				}
			}
		}
		random[usun][0] = -1;
		random[usun][1] = -1;
		dystans_calkowity+= dystans_licz;
	}

	double czasdouble = ((clock() - zacznij));

	ostringstream ss;
	ss << czasdouble;
	ostringstream ss2;
	ss2 << dystans_calkowity;
	std::string napis_koncowy = "Czas dzialania algorytmu: " + ss.str() + "ms    Calkowity dystans: " + ss2.str() + "j";

	//=========================================== DRUGIE OKNO RYSUJACE PUNKTY ===============================================

	sf::Text napis;
	napis.setFont(font);
	napis.setCharacterSize(15);
	napis.setPosition(20, 570);
	napis.setString(napis_koncowy);
	napis.setColor(sf::Color::Black);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(600, 600), "Komiwojazer by Sobierajski & Socha", sf::Style::Close, settings);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		sf::CircleShape circle(10); 
		circle.setFillColor(sf::Color(150, 50, 250)); 
		circle.setOutlineColor(sf::Color(50, 150, 150)); 
		circle.setOutlineThickness(2); 
		circle.setOrigin(10, 10); 
		
		sf::Texture tekstura;
		tekstura.loadFromFile("C:\\Users\\sobieraj95\\Desktop\\komiwojazer_projekt\\kartka.png");
		sf::Sprite obrazek;
		obrazek.setTexture(tekstura);
		window.draw(obrazek);
		window.draw(napis);

		for (int i = 0; i < N; i++)
		{
			if (N - i != 1)
			{
				circle.setPosition(posortowane[i][0], posortowane[i][1]);
				sf::Vertex line[2] = 
				{
					sf::Vertex(sf::Vector2f(posortowane[i][0], posortowane[i][1]), sf::Color::Red),
					sf::Vertex(sf::Vector2f(posortowane[i + 1][0], posortowane[i + 1][1]), sf::Color::Red)
				};
				window.draw(line, 2, sf::Lines); 
				window.draw(circle);			
			}
			else{
				circle.setPosition(posortowane[N-1][0], posortowane[N-1][1]);
				window.draw(circle);
			}			
		};
		window.display();
	}
	return 0;
}