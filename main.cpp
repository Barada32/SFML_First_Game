#include <SFML/Graphics.hpp>

using namespace sf;


float offsetX = 0, offsetY = 0;


const int H = 20;
const int W = 45;


String TileMap[H] = {

"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"B                                           B",
"B      BBBB                                 B",
"B                      BBBB       000       B",
"B                                BBB        B",
"B         0000                  BBB      BB B",
"B          BB       BBBBBB        BBB       B",
"BBB                           BBB       BBB B",
"B              00                           B",
"B                                           B",
"B    B         BB           BBB             B",
"B    B BBBBB          BB             BBB    B",
"B    B         BB BB                        B",
"B    B         BB             BBB           B",
"B    B         BB                           B",
"B    B         BBB                          B",
"B    BB                  BB 000             B",
"B                         BB00BBB           B",
"B              BB         BBBBBBB           B",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",

};


class PLAYER {

public:

	float dx, dy;
	FloatRect rect;
	bool onGround;
	Sprite sprite;
	float currentFrame;

	PLAYER(Texture& image)
	{
		sprite.setTexture(image);
		rect = FloatRect(7 * 32, 9 * 32, 40, 50);

		dx = dy = 0.1;
		currentFrame = 0;
	}


	void update(float time)
	{

		rect.left += dx * time;
		Collision(0);

		if (!onGround) dy = dy + 0.0005 * time;
		rect.top += dy * time;
		onGround = false;
		Collision(1);


		currentFrame += 0.005 * time;
		if (currentFrame > 6) currentFrame -= 6;

		if (dx > 0) sprite.setTextureRect(IntRect(40 * int(currentFrame), 244, 40, 50));
		if (dx < 0) sprite.setTextureRect(IntRect(40 * int(currentFrame) + 40, 244, -40, 50));


		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

		dx = 0;
	}



	void Collision(int dir)
	{
		for (int i = rect.top / 32; i < (rect.top + rect.height) / 32; i++)
			for (int j = rect.left / 32; j < (rect.left + rect.width) / 32; j++)
			{
				if (TileMap[i][j] == 'B')
				{
					if ((dx > 0) && (dir == 0)) rect.left = j * 32 - rect.width;
					if ((dx < 0) && (dir == 0)) rect.left = j * 32 + 32;
					if ((dy > 0) && (dir == 1)) { rect.top = i * 32 - rect.height;  dy = 0;   onGround = true; }
					if ((dy < 0) && (dir == 1)) { rect.top = i * 32 + 32;   dy = 0; }
				}

				if (TileMap[i][j] == '0')
				{
					TileMap[i][j] = ' ';
				}

			}

	}
};


int main()
{

	RenderWindow window(VideoMode(600, 400), "Test!"/*, Style::Fullscreen*/);

	Texture tileSet;
	tileSet.loadFromFile("Mario_Tileset.png");





	Texture t;
	t.loadFromFile("image/sonic.png");

	float currentFrame = 0;

	PLAYER p(t);

	Clock clock;

	RectangleShape rectangle(Vector2f(32, 32));

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time / 700;

		if (time > 20) time = 20;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			p.dx = -0.1;

		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			p.dx = 0.1;
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (p.onGround) { p.dy = -0.35; p.onGround = false; }
		}

		p.update(time);

		if (p.rect.left > 300) offsetX = p.rect.left - 300;
		offsetY = p.rect.top - 200;

		window.clear(Color::White);


		for (int i = 0; i < H; i++)
			for (int j = 0; j < W; j++)
			{
				if (TileMap[i][j] == 'B') rectangle.setFillColor(Color::Black);

				if (TileMap[i][j] == '0')  rectangle.setFillColor(Color::Green);

				if (TileMap[i][j] == ' ') continue;

				rectangle.setPosition(j * 32 - offsetX, i * 32 - offsetY);
				window.draw(rectangle);
			}

		window.draw(p.sprite);
		window.display();
	}

	return 0;
}




