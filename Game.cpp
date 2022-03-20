#include "Game.h"
Game::Game() {
	this->initVars();
	this->initWindow();
	this->initTubes();
	this->initPoints();
	this->initSprites();
	this->initBird();
	this->initSounds();

	this->flyUp.restart(); //timer starts by default
}
Game::~Game() {
	delete this->window;
}
const bool Game::running()const {
	return this->window->isOpen();
}
void Game::initVars() {
	this->gameOver = false;
	this->window = nullptr;
	this->velocity = 0.75f;
	this->gravity = 0.4f;
	this->emptySpace = 200.f;
	this->pointsNumber = 0.f;
	this->mouseHeld = false;
	this->pauseState = true;
	this->birdFloat.restart(); //at the start pause state is on
}
void Game::initWindow() {
	this->videoMode.height = 896;
	this->videoMode.width = 768;
	this->icon.loadFromFile("Images/icon.png");
	this->window = new sf::RenderWindow(this->videoMode, "flep bord", sf::Style::Titlebar | sf::Style::Close);
	this->window->setIcon(149, 158, this->icon.getPixelsPtr());
	this->window->setFramerateLimit(60);
}
void Game::initBird() {
	this->bird.setTexture(this->textureBird);
	this->birdSource = sf::IntRect(0, 0, 92, 64); //texture consists of 3 images of bird with different wings location
	this->birdAnimation.restart(); //animation of changing frames starts when bird is initialized
	this->bird.setTextureRect(this->birdSource);
	this->bird.setOrigin(this->bird.getGlobalBounds().width / 2.f, this->bird.getGlobalBounds().height / 2.f); //origin is center i think
	this->bird.setPosition(sf::Vector2f(this->window->getSize().x / 2.f - 40.f, this->window->getSize().y / 2.f));//bird is slightly left from the center
}
void Game::initTubes() {
	this->tubeSpawnTimer.restart(); 
	
	this->halfTube1.setTexture(this->textureTube);
	this->halfTube2.setTexture(this->textureTube);
	this->halfTube1.setPosition(sf::Vector2f(this->window->getSize().x, -40.f)); //upper half of tube is positioned at -40.f at y for player not to fly over
}
void Game::spawnTubes() {
	this->tubeSpawnTimer.restart();
	this->halfTube1.setTextureRect(sf::IntRect(0, 0, 138, rand() % (this->window->getSize().y - this->textureGround.getSize().y - 250 - int(this->emptySpace)) + 140.f)); //halftube is getting random half of texture
	this->halfTube1.setOrigin(0, this->halfTube1.getGlobalBounds().height); //origin is set like this to turn upper half of tube over
	this->halfTube1.setScale(1, -1); //turning it over
	this->halfTube2.setTextureRect(sf::IntRect(0, 0, 138, this->window->getSize().y - this->halfTube1.getTextureRect().height + this->emptySpace)); //lower half is taking remaining part of space without empty space
	this->halfTube2.setPosition(sf::Vector2f(this->window->getSize().x, this->halfTube1.getTextureRect().height + this->emptySpace));
	 
	this->tubes.push_back(this->halfTube1); //both halver into array
	this->tubes.push_back(this->halfTube2);
}
void Game::initPoints() {
	if (!this->FB.loadFromFile("Fonts/FB.ttf")) {
		std::cout << "ERROR::GAME::INITFONTS\n";
	}

	this->points.setFont(this->FB);
	this->points.setCharacterSize(70);
	this->points.setFillColor(sf::Color::White);
	this->points.setOutlineColor(sf::Color::Black);
	this->points.setOutlineThickness(3);

	std::ifstream fin("highscore.txt"); //highscore is always beign read from the file
	if (fin) {
		fin >> this->highscoreNumber;

		fin.close();
	}
	else // if file doesnt exit then highscore is 0
		this->highscoreNumber = 0;

	this->highscore.setFont(this->FB);
	this->highscore.setCharacterSize(45);
	this->highscore.setFillColor(sf::Color(255, 255, 255, 0)); //black and transparent
	this->highscore.setOutlineColor(sf::Color(0, 0, 0, 0)); //white and transparent
	this->highscore.setOutlineThickness(3);
}
void Game::initSprites() {
	if (!this->textureBackground.loadFromFile("Images/background.png") || !this->textureGround.loadFromFile("Images/ground.png")
		|| !this->textureBird.loadFromFile("Images/bird.png") || !this->textureTube.loadFromFile("Images/pipe.png")
		|| !this->textureFinScore.loadFromFile("Images/score.png") || !this->textureRestart.loadFromFile("Images/restart.png")) {
		std::cout << "ERROR::GAME::INITSPRITES\n";
	}
	this->textureGround.setRepeated(true); //ground consists of repeated ground textures
	this->ground.setTexture(&this->textureGround);
	this->ground.setPosition(0.f, this->window->getSize().y - this->textureGround.getSize().y);
	this->ground.setSize(sf::Vector2f(this->window->getSize().x, 128.f));
	this->ground.setTextureRect({ 0, 0, int(this->window->getSize().x), 128 }); //sprite is the same width as window

	this->background.setTexture(this->textureBackground);
	this->background.setPosition(0.f, -128.f); //background is above than ground

	this->finalScore.setTexture(this->textureFinScore);
	this->finalScore.setOrigin(this->finalScore.getLocalBounds().left + this->finalScore.getLocalBounds().width / 2.f,
		this->finalScore.getLocalBounds().top + this->finalScore.getLocalBounds().height / 2.f); //origin is its center
	this->finalScore.setPosition(this->window->getSize().x / 2.f, this->window->getSize().y / 2.f - 90.f); //slightly higher than the center
	this->finalScore.setColor(sf::Color(255, 255, 255, 0)); //transparent by default

	this->restart.setTexture(this->textureRestart);
	this->restart.setOrigin(this->restart.getLocalBounds().left + this->restart.getLocalBounds().width / 2.f,
		this->restart.getLocalBounds().top + this->restart.getLocalBounds().height / 2.f); //origin is its center
	this->restart.setPosition(this->window->getSize().x / 2.f, this->finalScore.getPosition().y + this->textureFinScore.getSize().y / 2.f + 50.f); //slightly below final score
	this->restart.setColor(sf::Color(255, 255, 255, 0)); //transparent by default
}
void Game::initSounds() {
	if (!this->flyUpSoundBuf.loadFromFile("Sounds/flyUp.wav") || !this->scoringPointBuf.loadFromFile("Sounds/scoringPoint.wav") ||
		!this->fallingBuf.loadFromFile("Sounds/fallingAfterLose.wav") || !this->collidingBuf.loadFromFile("Sounds/colliding.wav")
		|| !this->whistleBuf.loadFromFile("Sounds/whistleAfterLose.wav"))
		std::cout << "ERROR::GAME::INITSOUND\n";

	this->flyUpSound.setBuffer(this->flyUpSoundBuf);
	this->flyUpSound.setVolume(50);

	this->scoringPoint.setBuffer(this->scoringPointBuf);
	this->scoringPoint.setVolume(30);

	this->fallingAfterLose.setBuffer(this->fallingBuf);
	this->fallingAfterLose.setVolume(50);

	this->colliding.setBuffer(this->collidingBuf);
	this->colliding.setVolume(30);

	this->whistleAfterLose.setBuffer(this->whistleBuf);
	this->whistleAfterLose.setVolume(40);
}
void Game::pollEvents() {
	while (this->window->pollEvent(this->even)) {
		switch (this->even.type) {
		case sf::Event::Closed:
			this->window->close(); break;
		case sf::Event::KeyPressed: case sf::Event::MouseButtonPressed:
			if (this->even.key.code == sf::Keyboard::Space || this->even.mouseButton.button == sf::Mouse::Button::Left //bird flies if space or lmb or rmb is pressed
					|| this->even.mouseButton.button == sf::Mouse::Button::Right) {
				this->flyUp.restart(); //animation starts
				this->velocity = 0.75f; //velocity resets
				this->pauseState = false; //pause state ends once button is pressed
				if (!this->gameOver)
					this->flyUpSound.play();
			}
		}
	}
}
void Game::updateTubes() {
	for (auto& t : this->tubes) { //different cycle for game to delete two sides of tube and only then move other
		t.move(-4.f, 0.f);	//in the same cycle one half of tube would shift slightly to the right every time previous tube gets erased
		if (t.getPosition().x == this->bird.getPosition().x) {
			this->pointsNumber += 0.5;	//points are added by half because basically bird crosses two halves and gets half point for each
			this->scoringPoint.play();
		}
	}	
	if (this->tubeSpawnTimer.getElapsedTime().asSeconds() >= 2.f) { //tubes spawned every 2 second
		this->spawnTubes();
	}
	for (int i = 0; i < this->tubes.size(); i++) {
		if (this->tubes[i].getPosition().x + this->tubes[i].getTextureRect().width <= 0) { //erasing tubes from vector when they are gone
			this->tubes.erase(this->tubes.begin() + i);
		}
		if (this->bird.getGlobalBounds().intersects(this->tubes[i].getGlobalBounds())) { //if bird hits tube
			this->colliding.play();

			this->gameOver = true;

			this->points.setCharacterSize(45); //for points to appear at final screen
			this->points.setOrigin(this->points.getLocalBounds().left + this->points.getLocalBounds().width / 2.f, this->points.getLocalBounds().height);
			this->points.setPosition(this->window->getSize().x / 2.f, this->finalScore.getPosition().y - 15.f); //positioning to fit nicely
			this->points.setFillColor(sf::Color(255, 255, 255, 0)); //becomes transparent
			this->points.setOutlineColor(sf::Color(0, 0, 0, 0)); //outline transparent too

			if (this->pointsNumber > this->highscoreNumber) { //if points in this game is greater than all-time highscore
				std::ofstream fout("highscore.txt");
				if (fout) {
					fout << this->pointsNumber; //it is written into file

					fout.close();
				}
				this->highscoreNumber = this->pointsNumber; //highscore becomes current amount of points
			}
			ss << this->highscoreNumber;
			this->highscore.setString(ss.str()); //giving it new value
			ss.str(std::string());

			this->highscore.setOrigin(this->highscore.getLocalBounds().left + this->highscore.getLocalBounds().width / 2.f, this->highscore.getLocalBounds().height);
			this->highscore.setPosition(this->window->getSize().x / 2.f, this->finalScore.getPosition().y + 70.f); //positioning to fit nicely

			this->deathScreenAnimation.restart(); //starting death animation
		}
	}	
}
void Game::drawTubes(sf::RenderTarget& target) {
	for (auto& tube : this->tubes) { //drawing each half of tube
		target.draw(tube);
	}
}
void Game::updateBird() {
	if (!pauseState) { //if it is not pausestate
		if (this->bird.getPosition().y < this->window->getSize().y - this->textureGround.getSize().y - 40.f && this->flyUp.getElapsedTime().asSeconds() >= 0.3f) {//if bird didn't already hit ground
			this->velocity += this->gravity; //velocity is always increased
			this->bird.move(0.f, this->velocity); //bird moves with new velocity
			if ((this->bird.getRotation() >= 335.f || this->bird.getRotation() < 86.f) && this->flyUp.getElapsedTime().asSeconds() >= 0.6f)
				this->bird.rotate(7.f); //rotating to look down after a certain time
		}
		else if (this->bird.getPosition().y > -40.f && this->flyUp.getElapsedTime().asSeconds() < 0.265f) { //if bird didn't hit ceiling which is -40 on y
			
			this->bird.move(0.f, -6.5f); //bird goes up
			this->bird.setRotation(345.f); //rotation is set at this degree
		}
		else if (this->bird.getPosition().y >= this->window->getSize().y - this->textureGround.getSize().y - 40.f) { //if bird hits ground 
			this->colliding.play();
			
			this->gameOver = true; //everything is the same as when bird hits tube

			this->points.setCharacterSize(45);
			this->points.setOrigin(this->points.getLocalBounds().left + this->points.getLocalBounds().width / 2.f, this->points.getLocalBounds().height);
			this->points.setPosition(this->window->getSize().x / 2.f, this->finalScore.getPosition().y - 15.f);
			this->points.setFillColor(sf::Color(255, 255, 255, 0));
			this->points.setOutlineColor(sf::Color(0, 0, 0, 0));

			if (this->pointsNumber > this->highscoreNumber) {
				std::ofstream fout("highscore.txt");
				if (fout) {
					fout << this->pointsNumber;

					fout.close();
				}
				this->highscoreNumber = this->pointsNumber;
			}
			ss << this->highscoreNumber;
			this->highscore.setString(ss.str());
			ss.str(std::string());

			this->highscore.setOrigin(this->highscore.getLocalBounds().left + this->highscore.getLocalBounds().width / 2.f, this->highscore.getLocalBounds().height);
			this->highscore.setPosition(this->window->getSize().x / 2.f, this->finalScore.getPosition().y + 70.f);

			this->deathScreenAnimation.restart();
		}
		if (this->bird.getRotation() >= 85.f && this->bird.getRotation() <= 93.f) { //if bird moves straight down
			this->birdSource.left = 92.f;
			this->bird.setTextureRect(this->birdSource); //then only single frame is displayed
		}
		else { //normal bird state flying changing frames constantly
			if (this->birdAnimation.getElapsedTime().asSeconds() >= 0.15f) {
				this->birdSource.left += 92.f;
				this->bird.setTextureRect(this->birdSource);
				this->birdAnimation.restart();
			}
			if (this->birdSource.left == 184.f) { //renewing cycle
				this->birdSource.left = -92.f;
			}
		}
	}
	else { //if pause state is on
		if (this->birdAnimation.getElapsedTime().asSeconds() >= 0.15f) { //wings animation is still going on
			this->birdSource.left += 92.f;
			this->bird.setTextureRect(this->birdSource);
			this->birdAnimation.restart();
		}
		if (this->birdSource.left == 184.f) {
			this->birdSource.left = -92.f;
		}
		
		if (this->birdFloat.getElapsedTime().asSeconds() <= 0.3f) //bird is moving up and down based on birdFloat timer
			this->bird.move(0.f, 1.f);
		else if (this->birdFloat.getElapsedTime().asSeconds() <= 0.6f)
			this->bird.move(0.f, -1.f);
		else
			this->birdFloat.restart();
	}
} 
void Game::drawBird(sf::RenderTarget& target) {
	target.draw(this->bird);
}
void Game::updatePoints() {
	ss << this->pointsNumber;
	this->points.setString(ss.str());
	this->points.setOrigin(this->points.getLocalBounds().left + this->points.getLocalBounds().width / 2.f, this->points.getLocalBounds().height);
	this->points.setPosition(this->window->getSize().x / 2.f, this->window->getSize().y / 5.f); //points are always getting a new origin and position because its width depends on its value which is always changing too
	ss.str(std::string());
}
void Game::drawPoints(sf::RenderTarget& target) {
	target.draw(this->points);	
	target.draw(this->highscore);
}
void Game::drawBackground(sf::RenderTarget& target) {
	target.draw(this->background);
}
void Game::updateGround() {
	this->ground.setTextureRect({ this->ground.getTextureRect().left + 4, 0, int(this->window->getSize().x), 128 }); //ground texture moves the same amoutn as tubes
}
void Game::drawGround(sf::RenderTarget& target) {
	target.draw(this->ground);
}
void Game::updateDeathScreen() {
	if (this->bird.getPosition().y < this->window->getSize().y - this->textureGround.getSize().y - 40.f) //if bird did not hit the ground
	{
		this->birdSource.left = 92; //bird is the same falling frame
		this->bird.setTextureRect(this->birdSource);
		this->bird.move(0.f, 20.f); //moving down
		if (this->bird.getRotation() >= 330.f || this->bird.getRotation() < 86.f) { //bird need to look down 
			this->bird.rotate(9.f);
			this->fallingAfterLose.play();
			this->whistleAfterLose.play();
		}
	}
	if (this->deathScreenAnimation.getElapsedTime().asSeconds() < 0.415f) {
		this->finalScore.move(0.f, 2.f); //nice slide down animation
		if (this->finalScore.getColor().a <= 255.f)
			this->finalScore.setColor(sf::Color(255, 255, 255, this->finalScore.getColor().a + 10.f)); //becoming less and less transparent
		
		this->restart.move(0.f, 2.f); 
		if (this->restart.getColor().a <= 255.f)
			this->restart.setColor(sf::Color(255, 255, 255, this->restart.getColor().a + 10.f));

		this->points.move(0.f, 2.f);
		if (this->points.getFillColor().a <= 255.f) {
			this->points.setFillColor(sf::Color(255, 255, 255, this->points.getFillColor().a + 10.f));
			this->points.setOutlineColor(sf::Color(0, 0, 0, this->points.getOutlineColor().a + 10.f));
		}

		this->highscore.move(0.f, 2.f);
		if (this->highscore.getFillColor().a <= 255.f) {
			this->highscore.setFillColor(sf::Color(255, 255, 255, this->highscore.getFillColor().a + 10.f));
			this->highscore.setOutlineColor(sf::Color(0, 0, 0, this->highscore.getOutlineColor().a + 10.f));
		}

		std::cout << float(this->finalScore.getColor().a) << std::endl;
	}

}
void Game::drawDeathScreen(sf::RenderTarget& target) {
	target.draw(this->finalScore);
	target.draw(this->restart);
}
void Game::updateMousePosition()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window); //int
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow); //float
}
void Game::update() {
	this->pollEvents();
	this->updateMousePosition();
	if (!this->gameOver) { //everything updates if game is not over
		if (!this->pauseState) { //if it is just start of the game then only ground and bird moves
			this->updatePoints();
			this->updateTubes();
		}
		this->updateBird();
		this->updateGround();
	}
	else {
		this->updateDeathScreen(); //if game is over death screen appears
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { //game restarts if player clicks restart or presses space
			if (!this->mouseHeld || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				if (this->restart.getGlobalBounds().contains(this->mousePosView) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) { //is mouse is on restart
					this->gameOver = false;
					this->velocity = 0.75f; //default values
					this->gravity = 0.4f;
					this->pointsNumber = 0.f;
					this->birdAnimation.restart();
					this->bird.setPosition(sf::Vector2f(this->window->getSize().x / 2.f - 40.f, this->window->getSize().y / 2.f));
					this->bird.setRotation(0.f);
					this->tubes.clear();
					this->tubeSpawnTimer.restart();
					this->points.setCharacterSize(70);
					this->highscore.setFillColor(sf::Color(255, 255, 255, 0));
					this->highscore.setOutlineColor(sf::Color(0, 0, 0, 0));
					this->finalScore.setPosition(this->window->getSize().x / 2.f, this->window->getSize().y / 2.f - 90.f);
					this->restart.setPosition(this->window->getSize().x / 2.f, this->finalScore.getPosition().y + this->textureFinScore.getSize().y / 2.f + 50.f);
					this->finalScore.setColor(sf::Color(255, 255, 255, 0));
					this->restart.setColor(sf::Color(255, 255, 255, 0));
					this->points.setFillColor(sf::Color::White);
					this->points.setOutlineColor(sf::Color::Black);
					this->points.setPosition(this->window->getSize().x / 2.f, this->window->getSize().y / 5.f);
					this->pointsNumber = 0;
					this->pauseState = true; //pause state is on after restart
					this->birdFloat.restart(); //floating in pause state
				}

				this->mouseHeld = true;
			}
		}
		else
			this->mouseHeld = false; //checking if mouse is not just held
	}
}
void Game::draw() {
	this->window->clear();

	this->drawBackground(*this->window);
	this->drawTubes(*this->window);
	this->drawBird(*this->window);
	this->drawGround(*this->window);		
	if(this->gameOver) {
		this->drawDeathScreen(*this->window);
	}
	if (!this->pauseState) {
		this->drawPoints(*this->window); //points only appear after player starts the game(interrupts pause state)
	}
	this->window->display();
}
