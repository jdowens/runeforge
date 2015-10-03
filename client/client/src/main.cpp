#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string>
#include<vector>
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<Thor/Input.hpp>
#include<Thor/Particles.hpp>
#include<Thor/Math.hpp>
#include<Thor/Animations.hpp>
#include<Thor/Vectors.hpp>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const sf::IntRect swordRect1(0, 0, 64, 512);
const sf::IntRect swordRect2(65, 0, 64, 512);
const sf::IntRect swordRect3(130, 0, 64, 512);
const sf::IntRect swordRect4(195, 0, 64, 512);
const int MAX_CLICKS = 8;

int main()
{
	sf::Font font;
	font.loadFromFile("resources/Fonts/arial.ttf");
	sf::Text clickCountText;
	sf::Text goldText;
	sf::Text bankText;
	sf::Text penaltyText;
	clickCountText.setString("0");
	clickCountText.setFont(font);
	clickCountText.setCharacterSize(24);
	clickCountText.setColor(sf::Color::White);
	goldText.setString("0");
	goldText.setFont(font);
	goldText.setCharacterSize(64);
	goldText.setColor(sf::Color::Yellow);
	bankText.setString("0");
	bankText.setFont(font);
	bankText.setCharacterSize(24);
	bankText.setColor(sf::Color::Yellow);
	bankText.setPosition(0.0f, 32.0f);
	penaltyText.setString("- 50");
	penaltyText.setFont(font);
	penaltyText.setCharacterSize(64);
	penaltyText.setColor(sf::Color::Red);

	std::srand(std::time(NULL));
	sf::Texture swordTexture;
	sf::Sprite swordSprite;
	swordTexture.loadFromFile("resources/TestSword/testsword.png");
	swordSprite.setTexture(swordTexture);
	swordSprite.setTextureRect(swordRect1);
	swordSprite.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - swordRect1.width/2, WINDOW_HEIGHT / 2 - swordRect1.height/2));

	// background
	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("resources/test_background.jpg");
	sf::Sprite background;
	background.setTexture(backgroundTexture);

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "Rune Forge");

	sf::Clock clock;

	// clicks and rects
	int maxStepOneClicks = (std::rand() % MAX_CLICKS) + 1;
	int maxStepTwoClicks = (std::rand() % MAX_CLICKS) + 1;
	int maxStepThreeClicks = (std::rand() % MAX_CLICKS) + 1;
	int stepOneClicks = maxStepOneClicks;
	int stepTwoClicks = maxStepTwoClicks;
	int stepThreeClicks = maxStepThreeClicks;
	int currentRegion = 1;
	sf::IntRect stepOneRect(swordSprite.getGlobalBounds().left + swordSprite.getGlobalBounds().width - 16, swordSprite.getGlobalBounds().top, 16, 512);
	sf::IntRect stepTwoRect(swordSprite.getGlobalBounds().left, swordSprite.getGlobalBounds().top, 16, 512);
	sf::IntRect stepThreeRect(swordSprite.getGlobalBounds().left + (swordSprite.getGlobalBounds().width / 2) - 32, swordSprite.getGlobalBounds().top, 64, 64);
	sf::IntRect stepFourRect(WINDOW_WIDTH - 128, 0, 128, WINDOW_HEIGHT);
	sf::RectangleShape region1;
	region1.setSize(sf::Vector2f(stepOneRect.width, stepOneRect.height));
	region1.setPosition(stepOneRect.left, stepOneRect.top);
	region1.setFillColor(sf::Color::Transparent);
	region1.setOutlineThickness(4.0f);
	region1.setOutlineColor(sf::Color::Red);

	sf::RectangleShape region2;
	region2.setSize(sf::Vector2f(stepTwoRect.width, stepTwoRect.height));
	region2.setPosition(stepTwoRect.left, stepTwoRect.top);
	region2.setFillColor(sf::Color::Transparent);
	region2.setOutlineThickness(4.0f);
	region2.setOutlineColor(sf::Color::Red);

	sf::RectangleShape region3;
	region3.setSize(sf::Vector2f(stepThreeRect.width, stepThreeRect.height));
	region3.setPosition(stepThreeRect.left, stepThreeRect.top);
	region3.setFillColor(sf::Color::Transparent);
	region3.setOutlineThickness(4.0f);
	region3.setOutlineColor(sf::Color::Red);

	sf::RectangleShape region4;
	region4.setSize(sf::Vector2f(stepFourRect.width, stepFourRect.height));
	region4.setPosition(stepFourRect.left, stepFourRect.top);
	region4.setFillColor(sf::Color::Transparent);
	region4.setOutlineThickness(4.0f);
	region4.setOutlineColor(sf::Color::Yellow);

	bool swordSelected = false;
	sf::Vector2f mousePos;
	float goldTextTime = 0.0f;
	float penaltyTextTime = 0.0f;
	int bankAccount = 0;

	int goldEarned = (maxStepOneClicks + maxStepTwoClicks + maxStepThreeClicks) * 10;

	// particles
	thor::UniversalEmitter emitter;
	thor::ParticleSystem particle_system;
	sf::Texture particleTexture;
	particleTexture.loadFromFile("Media/particle.png");
	emitter.setParticleLifetime(sf::seconds(1.0f));
	particle_system.setTexture(particleTexture);

	// Build color gradient
	thor::ColorGradient gradient;
	gradient[0.f] = sf::Color(200, 200, 200);
	gradient[0.5f] = sf::Color::Red;
	gradient[1.f] = sf::Color::Black;

	// Create color and fade in/out animations
	thor::ColorAnimation colorizer(gradient);
	thor::FadeAnimation fader(0.1f, 0.1f);

	particle_system.clearAffectors();
	// Add particle affectors
	particle_system.addAffector(thor::AnimationAffector(colorizer));
	particle_system.addAffector(thor::AnimationAffector(fader));
	particle_system.addAffector(thor::TorqueAffector(100.f));
	particle_system.addAffector(thor::ForceAffector(sf::Vector2f(0.f, 200.0f)));

	emitter.setParticlePosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
	emitter.setParticleVelocity(thor::Distributions::deflect(thor::PolarVector2f(200.0f, -90.0f), 180.0f));
	emitter.setEmissionRate(5.0f);

	// sounds
	std::vector<sf::SoundBuffer> hammerSounds;
	sf::SoundBuffer hammerSound1;
	sf::SoundBuffer hammerSound2;
	sf::SoundBuffer hammerSound3;
	sf::SoundBuffer goldEarnedSound;
	sf::SoundBuffer penaltySound;
	hammerSound1.loadFromFile("resources/Sounds/Effects/Explosion.ogg");
	hammerSound2.loadFromFile("resources/Sounds/Effects/Explosion3.ogg");
	hammerSound3.loadFromFile("resources/Sounds/Effects/Explosion6.ogg");
	hammerSounds.push_back(hammerSound1);
	hammerSounds.push_back(hammerSound2);
	hammerSounds.push_back(hammerSound3);
	goldEarnedSound.loadFromFile("resources/Sounds/Effects/Pickup_Coin18.ogg");
	penaltySound.loadFromFile("resources/Sounds/Effects/Randomize11.ogg");
	sf::Sound soundEffect;

	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();
		goldTextTime -= dt;
		penaltyTextTime -= dt;
		if (goldTextTime < 0.0f)
			goldTextTime = 0.0f;
		if (penaltyTextTime < 0.0f)
			penaltyTextTime = 0.0f;

		particle_system.update(sf::seconds(dt));

		goldText.setColor(sf::Color(goldText.getColor().r, goldText.getColor().g, goldText.getColor().b, static_cast<float>((goldTextTime / 3.0f) * 255.0f)));
		penaltyText.setColor(sf::Color(penaltyText.getColor().r, penaltyText.getColor().g, penaltyText.getColor().b, static_cast<float>((penaltyTextTime / 3.0f) * 255.0f)));

		if (currentRegion == 1)
		{
			clickCountText.setString("Clicks remaining: " + std::to_string(stepOneClicks));
			region1.setOutlineColor(sf::Color(region1.getOutlineColor().r, region1.getOutlineColor().g, region1.getOutlineColor().b, 32 + (stepOneClicks * 28)-1));
		}
		else if (currentRegion == 2)
		{
			clickCountText.setString("Clicks remaining: " + std::to_string(stepTwoClicks));
			region2.setOutlineColor(sf::Color(region2.getOutlineColor().r, region2.getOutlineColor().g, region2.getOutlineColor().b, 32 + (stepTwoClicks * 28) - 1));
		}
		else if (currentRegion == 3)
		{
			clickCountText.setString("Clicks remaining: " + std::to_string(stepThreeClicks));
			region3.setOutlineColor(sf::Color(region3.getOutlineColor().r, region3.getOutlineColor().g, region3.getOutlineColor().b, 32 + (stepThreeClicks * 28) - 1));
		}
		else
		{
			clickCountText.setString("Drag and finish!");
		}


		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				if (swordSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))) && currentRegion == 4)
				{
					swordSelected = true;
					mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
					swordSprite.setOrigin(swordSprite.getInverseTransform().transformPoint(mousePos));
				}

				if (stepOneRect.contains(sf::Mouse::getPosition(window)))
				{
					printf("Region 1 clicked!\n");
					if (stepOneClicks > 0)
					{
						stepOneClicks--;
						if (stepOneClicks == 0)
							currentRegion = 2;
						emitter.setParticlePosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
						particle_system.addEmitter(thor::refEmitter(emitter), sf::seconds(0.35f));
						soundEffect.setBuffer(hammerSounds[std::rand() % 3]);
						soundEffect.play();
					}
				}
				else if (stepOneClicks == 0 && stepTwoRect.contains(sf::Mouse::getPosition(window)))
				{
					printf("Region 2 clicked!\n");
					if (stepTwoClicks > 0)
					{
						stepTwoClicks--;
						if (stepTwoClicks == 0)
							currentRegion = 3;
						emitter.setParticlePosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
						particle_system.addEmitter(thor::refEmitter(emitter), sf::seconds(0.35f));
						soundEffect.setBuffer(hammerSounds[std::rand() % 3]);
						soundEffect.play();
					}
				}
				else if (stepTwoClicks == 0 && stepThreeRect.contains(sf::Mouse::getPosition(window)))
				{
					printf("Region 3 clicked!\n");
					if (stepThreeClicks > 0)
					{
						stepThreeClicks--;
						if (stepThreeClicks == 0)
							currentRegion = 4;
						emitter.setParticlePosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
						particle_system.addEmitter(thor::refEmitter(emitter), sf::seconds(0.35f));
						soundEffect.setBuffer(hammerSounds[std::rand() % 3]);
						soundEffect.play();
					}
				}
				else if (currentRegion != 4)
				{
					penaltyTextTime = 3.0f;
					penaltyText.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
					goldEarned -= 50;
					if (goldEarned < 0)
						goldEarned = 0;
					soundEffect.setBuffer(penaltySound);
					soundEffect.play();
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
			{
				
				if (swordSelected && region4.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))))
				{
					bankAccount += goldEarned;
					bankText.setString("Bank: " + std::to_string(bankAccount));
					goldText.setString("+ " + std::to_string(goldEarned));
					maxStepOneClicks = (std::rand() % MAX_CLICKS) + 1;
					maxStepTwoClicks = (std::rand() % MAX_CLICKS) + 1;
					maxStepThreeClicks = (std::rand() % MAX_CLICKS) + 1;
					stepOneClicks = maxStepOneClicks;
					stepTwoClicks = maxStepTwoClicks;
					stepThreeClicks = maxStepThreeClicks;
					goldEarned = (maxStepOneClicks + maxStepTwoClicks + maxStepThreeClicks) * 10;
					currentRegion = 1;
					swordSprite.setTextureRect(swordRect1);
					swordSprite.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - swordRect1.width / 2, WINDOW_HEIGHT / 2 - swordRect1.height / 2));
					swordSprite.setOrigin(sf::Vector2f(0.0f, 0.0f));
					goldTextTime = 3.0f;
					sf::Vector2f curMousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
					goldText.setPosition(curMousePos.x - 256, curMousePos.y);
					soundEffect.setBuffer(goldEarnedSound);
					soundEffect.play();
				}
				swordSelected = false;
			}
		}

		// if done allow drag
		if (currentRegion == 4)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && swordSelected)
			{
				sf::Vector2f newPos;
				newPos.x = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)).x - mousePos.x + swordSprite.getGlobalBounds().left;
				newPos.y = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)).y - mousePos.y + swordSprite.getGlobalBounds().top;
				swordSprite.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
			}
		}

		if (stepOneClicks == 0)
			swordSprite.setTextureRect(swordRect2);
		if (stepTwoClicks == 0)
			swordSprite.setTextureRect(swordRect3);
		if (stepThreeClicks == 0)
			swordSprite.setTextureRect(swordRect4);

		window.clear(sf::Color::Blue);
		window.draw(background);
		window.draw(swordSprite);
		if (currentRegion == 1)
			window.draw(region1);
		else if (currentRegion == 2)
			window.draw(region2);
		else if (currentRegion == 3)
			window.draw(region3);
		window.draw(region4);
		window.draw(particle_system);
		window.draw(clickCountText);
		window.draw(bankText);
		if (goldTextTime > 0.0f)
			window.draw(goldText);
		if (penaltyTextTime > 0.0f)
			window.draw(penaltyText);
		window.display();
	}

}