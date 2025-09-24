#include<iostream>
#include<easyx.h>
using namespace std;

#define PI 3.1416
#define FPS 30
#define VAUTO 3
#define VCTRL 5
#define DR 1
#define RMAX 100
#define RMIN 5

#define RCENTER 13 
#define ROUT 30
#define R3IN 117
#define R3OUT 137
#define R2IN 199
#define R2OUT 222

string int_to_string(int x) {
	if (x == 0) return "0";
	string res = "";
	int length = 1, tmp = x;
	while (tmp /= 10) length++;
	int divisor = pow(10, length - 1);
	for (int _ = 0; _ < length; _++) {
		res += (x / divisor + '0');
		x %= divisor;
		x *= 10;
	}
	return res;
}

void sleep(int time) {
	clock_t start = clock();
	while (clock() - start < 2000) FlushBatchDraw();
}

class SolidCircle {
	int x;
	int y;
	int r;
	int dx;
	int dy;
	int dr;

public:

	SolidCircle() {}

	SolidCircle(int x, int y, int r) :
		x(x), y(y), r(r), dx(rand() % (2 * VAUTO + 1) - VAUTO), dy(sqrt(VAUTO* VAUTO - dx * dx)), dr(-DR) {}
	
	int getx() { return x; }
	
	int gety() { return y; }
	
	void draw() {
		setfillcolor(LIGHTGREEN);
		solidcircle(x, y, r);
	}

	void move() {

		/* change direction per second */

		if (clock() % 1000 < FPS) {
			dx = rand() % (2 * VAUTO + 1) - VAUTO;
			dy = sqrt(VAUTO * VAUTO - dx * dx);
			if (rand() % 2) dy = -dy;
		}

		/* move automatically */

		x += dx;
		y += dy;

		/* change radius */

		r += dr;
		if (r == RMIN) dr = DR;
		else if (r == RMAX) dr = -DR;

		/* keyboard control */

		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState('W')) y -= VCTRL;
		if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState('S')) y += VCTRL;
		if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState('A')) x -= VCTRL;
		if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState('D')) x += VCTRL;
	}
};

class FillRectangle {
	COLORREF linecolor;
	COLORREF fillcolor;
	int left;
	int top;
	int right;
	int bottom;

public:

	FillRectangle() {}

	FillRectangle(COLORREF linecolor, COLORREF fillcolor, int left, int top, int right, int bottom) :
		linecolor(linecolor), fillcolor(fillcolor), left(left), top(top), right(right), bottom(bottom) {}
	
	void draw() {
		setlinecolor(linecolor);
		setfillcolor(fillcolor);
		fillrectangle(left, top, right, bottom);
	}
};

class SimpleTextBoard{
	class FillRectangle board;
	COLORREF textcolor;
	int textheight;
	int x;
	int y;
	const char* text;

public:

	SimpleTextBoard() {}

	SimpleTextBoard(COLORREF linecolor, COLORREF fillcolor, int left, int top, int right, int bottom, COLORREF textcolor, int textheight, int x, int y, const char* text) :
		board(FillRectangle(linecolor, fillcolor, left, top, right, bottom)), textcolor(textcolor), textheight(textheight), x(x), y(y), text(text) {}
	
	void draw() {
		board.draw();
		setbkmode(TRANSPARENT);
		settextcolor(textcolor);
		settextstyle(textheight, 0, _T("Consolas"));
		outtextxy(x, y, text);
	}
};

class MainBody {
	SolidCircle circle;
	int score;
	IMAGE* img;
	IMAGE* darts;
	int ndart;
	int nround;

	void compute_dscore(int& dscore) {
		if (circle.getx() == 399) dscore = circle.gety() < 399 ? 20 : 3; // tantheta is undefined
		else {
			double tantheta = (399 - circle.gety()) * 1.0 / (circle.getx() - 399);
			if (circle.getx() > 399) { // right
				if (tantheta < tan(-9 * PI / 20)) dscore = 3;
				else if (tantheta < tan(-7 * PI / 20)) dscore = 17;
				else if (tantheta < tan(-5 * PI / 20)) dscore = 2;
				else if (tantheta < tan(-3 * PI / 20)) dscore = 15;
				else if (tantheta < tan(-1 * PI / 20)) dscore = 10;
				else if (tantheta < tan(1 * PI / 20)) dscore = 6;
				else if (tantheta < tan(3 * PI / 20)) dscore = 13;
				else if (tantheta < tan(5 * PI / 20)) dscore = 4;
				else if (tantheta < tan(7 * PI / 20)) dscore = 18;
				else if (tantheta < tan(9 * PI / 20)) dscore = 1;
				else dscore = 20;
			}
			else { // left
				if (tantheta < tan(-9 * PI / 20)) dscore = 20;
				else if (tantheta < tan(-7 * PI / 20)) dscore = 5;
				else if (tantheta < tan(-5 * PI / 20)) dscore = 12;
				else if (tantheta < tan(-3 * PI / 20)) dscore = 9;
				else if (tantheta < tan(-1 * PI / 20)) dscore = 14;
				else if (tantheta < tan(1 * PI / 20)) dscore = 11;
				else if (tantheta < tan(3 * PI / 20)) dscore = 8;
				else if (tantheta < tan(5 * PI / 20)) dscore = 16;
				else if (tantheta < tan(7 * PI / 20)) dscore = 7;
				else if (tantheta < tan(9 * PI / 20)) dscore = 19;
				else dscore = 3;
			}
		}
	}

	void initialize_score() { score = 0; }

	void initialize_ndart() { ndart = 25 - 5 * nround; }

	void initialize_circle() { circle = SolidCircle(rand() % 400 + 200, rand() % 400 + 200, RMAX); }

	void consume_ont_dart() { ndart--; }

	void compute_score() {
		int d_square = (circle.getx() - 399) * (circle.getx() - 399) + (circle.gety() - 399) * (circle.gety() - 399);
		int dscore = 0;
		int multiple = 1;

		/* compute dscore */

		if (d_square < RCENTER * RCENTER) dscore = 50; // BULL'S-EYE!
		else if (d_square < ROUT * ROUT) dscore = 25; // OUTER RING
		else if (d_square < R2OUT * R2OUT) compute_dscore(dscore); // HIT!

		/* compute multiple */

		if (d_square > R3IN * R3IN && d_square < R3OUT * R3OUT) multiple = 3;
		if (d_square > R2IN * R2IN && d_square < R2OUT * R2OUT) multiple = 2;

		/* compute score and output reason */

		setbkmode(TRANSPARENT);
		settextcolor(RED);
		settextstyle(30, 0, _T("Consolas"));
		if (dscore == 50) outtextxy(325, 50, "BULL'S-EYE!");
		else if (dscore == 25) outtextxy(325, 50, "OUTER RING");
		else if (dscore != 0) {
			string s = "";
			s += int_to_string(dscore);
			s += " * ";
			s += int_to_string(multiple);
			s += " = ";
			s += int_to_string(dscore * multiple);
			outtextxy(325, 50, s.c_str());
		}
		else outtextxy(370, 50, "MISS");
		if (score + (dscore * multiple) <= 301) score += (dscore * multiple);
		else outtextxy(370, 80, "LOSS");
		setlinecolor(LIGHTRED);
		setlinestyle(0, 4);
		line(circle.getx() - 15, circle.gety() - 15, circle.getx() + 15, circle.gety() + 15);
		line(circle.getx() - 15, circle.gety() + 15, circle.getx() + 15, circle.gety() - 15);
		setlinestyle(0, 1);
		FlushBatchDraw();
		sleep(2000);
	}

	void drawBackground() {

		/* background image */ 

		putimage(0, 0, img);

		/* score board */ 

		string s = "SCORE: ";
		s += int_to_string(score);
		SimpleTextBoard score_board(BROWN, GREEN, 300, 700, 500, 750, BROWN, 40, 305, 705, s.c_str());
		score_board.draw();

		/* darts */ 

		for (int i = 0; i < ndart; i++) {
			putimage(5 + 20 * i, 755, darts + 0, SRCAND);
			putimage(5 + 20 * i, 755, darts + 1, SRCPAINT);
		}
	}

public:
	MainBody() {}

	MainBody(SolidCircle circle, int score, IMAGE* img, IMAGE* darts, int ndart, int nround) :
		circle(circle), score(score), img(img), darts(darts), ndart(ndart), nround(nround) {}

	int getScore() { return score; }

	int getNdart() { return ndart; }

	int getNround() { return nround; }

	void setNround(int nround) { this->nround = nround; }

	SolidCircle getCircle() { return circle; }

	void initialize() {
		initialize_score();
		initialize_ndart();
		initialize_circle();
	}

	void start_round() {
		putimage(0, 0, img);

		/* board */ 

		FillRectangle board(BROWN, GREEN, 250, 300, 550, 500);
		board.draw();

		/*text */

		string s = "ROUND ";
		s += int_to_string(nround);
		setbkmode(TRANSPARENT);
		settextcolor(BROWN);
		settextstyle(50, 0, _T("Consolas"));
		outtextxy(315, 340, s.c_str());
		s = "";
		s += int_to_string(ndart);
		s += " DARTS";
		setbkmode(TRANSPARENT);
		settextcolor(BROWN);
		settextstyle(40, 0, _T("Consolas"));
		outtextxy(320, 400, s.c_str());
		sleep(2000);
	}

	void go_one_round() {
		while (score != 301 && ndart != 0) {
			drawBackground();
			clock_t start = clock();
			while ((clock() - start) < 1000 / FPS) {
				if (GetAsyncKeyState('J')) {
					compute_score();
					consume_ont_dart();
					initialize_circle();
					break;
				}
			}
			circle.move();
			circle.draw();
			flushmessage();
			FlushBatchDraw();
		}
	}

	void end_round() {
		putimage(0, 0, img);
		// board
		FillRectangle board(BROWN, GREEN, 250, 300, 550, 500);
		board.draw();
		// text
		string s = "ROUND ";
		s += int_to_string(nround);
		s += " PASS";
		setbkmode(TRANSPARENT);
		settextcolor(BROWN);
		settextstyle(50, 0, _T("Consolas"));
		outtextxy(260, 340, s.c_str());
		s = "";
		s += int_to_string(ndart);
		s += " DARTS REMAIN";
		settextstyle(40, 0, _T("Consolas"));
		outtextxy(260, 400, s.c_str());
		if (ndart == 20 - 6 - 5 * (nround - 1)) {
			s = "PERFECT!";
			settextstyle(40, 0, _T("Consolas"));
			outtextxy(325, 450, s.c_str());
		}
		sleep(2000);
	}

	void fail() {
		putimage(0, 0, img);
		SimpleTextBoard fail_board(BROWN, GREEN, 250, 300, 550, 500, BROWN, 50, 300, 370, "GAME OVER");
		fail_board.draw();
		sleep(2000);
	}

	void win() {
		putimage(0, 0, img);
		SimpleTextBoard win_board(BROWN, GREEN, 250, 300, 550, 500, BROWN, 50, 320, 370, "YOU WIN");
		win_board.draw();
		sleep(2000);
	}
};

int main() {
	srand((unsigned int)time(NULL));

	/* objects initialization */

	IMAGE img, darts[2];
	loadimage(&img, "back.png", 800, 800);
	loadimage(darts + 0, "dart.jpg", 40, 40);
	loadimage(darts + 1, "dartbk.jpg", 40, 40);
	MainBody body(SolidCircle(rand() % 400 + 200, rand() % 400 + 200, RMAX), 0, &img, darts, 20, 1);

	/* preparation */

	initgraph(800, 800);
	BeginBatchDraw();

	//<test content = "image">
	// 
	// while (true) FlushBatchDraw();
	//</test>

	/* main body */

	for ( ; body.getNround() <= 3; body.setNround(body.getNround() + 1)) { // 3 rounds
		body.initialize();
		body.start_round();
		body.go_one_round();
		if (body.getScore() == 301) body.end_round();
		else { body.fail(); break; }
	}
	if (body.getScore() == 301) body.win();

	EndBatchDraw();
	closegraph();
	return 0;
}
