#include<iostream>
#include<easyx.h>
#include<vector>


#define LENGTH_OF_FISHINGBAR 200


class FishingBar {

	friend class ProgressBar;

	int length;

	int top;

	int bottom;

	COLORREF color;

	int speed;

	bool isup;

	int time;

	void update_speed() {

		if (isup && top != 24) speed += 1;

		else if (!isup && bottom != 960 - 24) speed -= 1;

	}

	void move() {

		top -= speed;

		bottom -= speed;

		if (top < 24) {

			top = 24;

			bottom = top + length;

			if (isup) speed = 0;

			else speed = 0 - speed * 2 / 3;

		}

		else if (bottom > 960 - 24) {

			bottom = 960 - 24;

			top = bottom - length;

			speed = 0 - speed * 2 / 3;

		}

	}

public:

	FishingBar() : length(LENGTH_OF_FISHINGBAR), top(960 - 24 - length), bottom(960 - 24), color(RGB(127, 231, 0)), speed(0), isup(false), time(0) {}

	void draw() {

		setlinecolor(RGB(63, 88, 14));

		setfillcolor(color);

		fillrectangle(24, top, 72, bottom);

	}

	void up(bool isup) {

		this->isup = isup;

	}

	void update() {

		update_speed();

		move();

	}

};



class Fish {

	friend class ProgressBar;

	int height;

	int x;

	int y;

	int speed;

	bool isup;

	void update_direction_randomly() {

		isup = rand() % 2;

	}

	void update_speed() {

		if (isup && height != 800) speed += 1;

		else if (!isup && height != 0) speed -= 1;

		// if (rand() % 20 == 0) speed = 0;

	}

	void move() {

		height += speed;

		if (height < 0) {

			height = 0;

			speed = 0;

		}

		else if (height > 800) {

			height = 800;

			speed = 0;

		}

		y = 960 - 24 - 60 - height;

	}

public:

	Fish() : height(0), x(18), y(960 - 24 - 60 - height) {}

	void draw() {
		IMAGE fish[2];

		loadimage(&fish[0], _T("fish.png"), 60, 60);

        loadimage(&fish[1], _T("fishbk.png"), 60, 60);

		putimage(x, y, fish + 0, SRCAND);

		putimage(x, y, fish + 1, SRCPAINT);

	}

	void update() {

		update_direction_randomly();

		update_speed();

		move();

	}

};



class ProgressBar {

	COLORREF color;

	void update_height(FishingBar& fishingbar, Fish fish) {

		if (fish.y + 60 < fishingbar.top || fish.y > fishingbar.bottom) {

			height -= 3;

			fishingbar.color = RGB(100, 156, 114);

			fishingbar.time++;

		}

		else {

			height += 2;

			fishingbar.color = RGB(127, 231, 0);

			fishingbar.time = 0;

		}

		if (fishingbar.time > 5) {

			fishingbar.color = RGB(127, 231, 0);

		}

	}

	void update_color() {

		color = RGB((height > 480 ? 255 - (height - 480) * 255 / 480 : 255), (height < 480 ? height * 255 / 480 : 255), 0);

	}

public:

	int height;

	ProgressBar() : height(200), color(RGB(255, 255 / 2, 0)) {}

	void draw() {

		setlinecolor(RGB(136, 89, 57));

		setfillcolor(color);

		fillrectangle(96, 960 - height, 120, 960);

	}

	void update(FishingBar& fishingbar, Fish fish) {

		update_height(fishingbar, fish);

		update_color();

	}

};



void drawBackground() {

	setlinecolor(RGB(136, 89, 57));

	setfillcolor(RGB(238, 173, 79));

	fillrectangle(0, 0, 96, 960);

	setfillcolor(RGB(99, 142, 234));

	fillrectangle(24, 24, 72, 960 - 24);

	setfillcolor(RGB(136, 89, 57));

	fillrectangle(96, 0, 120, 960);

}



int main() {

	// srand((unsigned int)time(NULL));

	initgraph(1200, 960);

	BeginBatchDraw();

	FishingBar fishingbar;

	Fish fish;

	ProgressBar progressbar;

	while (progressbar.height > 0 && progressbar.height < 960) {

		drawBackground();

		clock_t start = clock();

		while ((clock() - start) < 1000 / 40) {

			ExMessage msg;

			peekmessage(&msg);

			if (msg.message == WM_LBUTTONDOWN) fishingbar.up(true);

			else if (msg.message == WM_LBUTTONUP) fishingbar.up(false);

		}

		flushmessage();

		fishingbar.update();

		fishingbar.draw();

		fish.update();

		fish.draw();

		progressbar.update(fishingbar, fish);

		progressbar.draw();

		FlushBatchDraw();

	}

	EndBatchDraw();

	closegraph();

	if (progressbar.height > 480) std::cout << "You win!" << std::endl;

	else std::cout << "You lose!" << std::endl;

	return 0;

}

