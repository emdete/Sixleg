static const int leg_front_left = 11;
static const int leg_back_left = 9;
static const int leg_middle_left = 10;
static const int leg_front_right = 6;
static const int leg_back_right = 4;
static const int leg_middle_right = 5;

static const int move_outer_right = 2;
static const int move_inner_right = 3;
static const int move_outer_left = 7;
static const int move_inner_left = 8;

static void init_legs() {
	digitalWrite(leg_front_left, LOW);
	digitalWrite(leg_back_left, LOW);
	digitalWrite(leg_middle_left, LOW);
	digitalWrite(leg_front_right, LOW);
	digitalWrite(leg_back_right, LOW);
	digitalWrite(leg_middle_right, LOW);
	digitalWrite(move_outer_right, LOW);
	digitalWrite(move_inner_right, LOW);
	digitalWrite(move_outer_left, LOW);
	digitalWrite(move_inner_left, LOW);
}

// (re)setup unit
void setup() {
	Serial.begin(38400);
	Serial.println("sixleg init");
	randomSeed(analogRead(0));
	pinMode(leg_front_left, OUTPUT);
	pinMode(leg_back_left, OUTPUT);
	pinMode(leg_middle_left, OUTPUT);
	pinMode(leg_front_right, OUTPUT);
	pinMode(leg_back_right, OUTPUT);
	pinMode(leg_middle_right, OUTPUT);
	pinMode(move_outer_right, OUTPUT);
	pinMode(move_inner_right, OUTPUT);
	pinMode(move_outer_left, OUTPUT);
	pinMode(move_inner_left, OUTPUT);
	init_legs();
}

// check all legs counterclockwise
static void checkleg() {
	digitalWrite(leg_front_left, HIGH);
	delay(500);
	digitalWrite(leg_front_left, LOW);
	digitalWrite(leg_middle_left, HIGH);
	delay(500);
	digitalWrite(leg_middle_left, LOW);
	digitalWrite(leg_back_left, HIGH);
	delay(500);
	digitalWrite(leg_back_left, LOW);
	digitalWrite(leg_back_right, HIGH);
	delay(500);
	digitalWrite(leg_back_right, LOW);
	digitalWrite(leg_middle_right, HIGH);
	delay(500);
	digitalWrite(leg_middle_right, LOW);
	digitalWrite(leg_front_right, HIGH);
	delay(500);
	digitalWrite(leg_front_right, LOW);
}

// try to pivot
static void pivot() {
/*	digitalWrite(leg_middle_right, HIGH);
	digitalWrite(move_inner_right, HIGH);
	delay(500);
	digitalWrite(leg_middle_right, LOW);*/
	digitalWrite(leg_front_left, HIGH);
	digitalWrite(leg_back_left, HIGH);
	digitalWrite(leg_front_right, HIGH);
	digitalWrite(leg_back_right, HIGH);
	for (int i=0;i<5;i++) {
		delay(900);
		digitalWrite(leg_front_left, LOW);
		delay(100);
		digitalWrite(leg_front_left, HIGH);
		delay(900);
		digitalWrite(leg_back_left, LOW);
		delay(100);
		digitalWrite(leg_back_left, HIGH);
		delay(900);
		digitalWrite(leg_front_right, LOW);
		delay(100);
		digitalWrite(leg_front_right, HIGH);
		delay(900);
		digitalWrite(leg_back_right, LOW);
		delay(100);
		digitalWrite(leg_back_right, HIGH);
	}
	digitalWrite(leg_front_left, LOW);
	digitalWrite(leg_back_left, LOW);
	digitalWrite(leg_front_right, LOW);
	digitalWrite(leg_back_right, LOW);
	delay(300);
	digitalWrite(leg_middle_right, HIGH);
	digitalWrite(move_inner_right, LOW);
	delay(500);
	digitalWrite(leg_middle_right, LOW);
}

// sit down
static void sitdown() {
	// legs forward
	digitalWrite(move_outer_right, HIGH);
	digitalWrite(move_inner_right, HIGH);
	digitalWrite(move_outer_left, HIGH);
	digitalWrite(move_inner_left, HIGH);
	// middle legs up
	digitalWrite(leg_middle_left, HIGH);
	digitalWrite(leg_middle_right, HIGH);
	delay(500);
	// rear legs up
	digitalWrite(leg_back_right, HIGH);
	digitalWrite(leg_back_left, HIGH);
	// sitting
	delay(123 * random(9, 11));
	// stand up
	digitalWrite(leg_front_left, LOW);
	digitalWrite(leg_back_left, LOW);
	digitalWrite(leg_middle_left, LOW);
	digitalWrite(leg_front_right, LOW);
	digitalWrite(leg_back_right, LOW);
	digitalWrite(leg_middle_right, LOW);
	delay(900);
	digitalWrite(move_outer_right, LOW);
	digitalWrite(move_inner_right, LOW);
	digitalWrite(move_outer_left, LOW);
	digitalWrite(move_inner_left, LOW);
	delay(900);
}

// shake
static void shake() {
	digitalWrite(leg_middle_right, HIGH);
	digitalWrite(leg_middle_left, HIGH);
	digitalWrite(move_inner_left, HIGH);
	digitalWrite(move_inner_right, HIGH);
	delay(400);
	digitalWrite(move_inner_left, LOW);
	digitalWrite(move_inner_right, LOW);
	delay(100);
	digitalWrite(leg_middle_right, LOW);
	digitalWrite(leg_middle_left, LOW);
	delay(800);
}

#define WAIT_DELAY 300
#define LEFT_UP(S) digitalWrite(leg_front_left, S); digitalWrite(leg_back_left, S); digitalWrite(leg_middle_right, S); delay(WAIT_DELAY);
#define LEFT_FORWARD(S) digitalWrite(move_outer_left, S); digitalWrite(move_inner_right, S); delay(WAIT_DELAY);
#define RIGHT_UP(S) digitalWrite(leg_middle_left, S); digitalWrite(leg_front_right, S); digitalWrite(leg_back_right, S); delay(WAIT_DELAY);
#define RIGHT_FORWARD(S) digitalWrite(move_outer_right, S); digitalWrite(move_inner_left, S); delay(WAIT_DELAY);

// walk count steps
static void walk(const int count) {
	LEFT_UP(HIGH);
	for (int i=0;i<count;i++) {
		// take one full step (all phases)
		LEFT_FORWARD(HIGH);
		LEFT_UP(LOW);
		RIGHT_UP(HIGH);
		LEFT_FORWARD(LOW);
		RIGHT_FORWARD(HIGH);
		RIGHT_UP(LOW);
		LEFT_UP(HIGH);
		RIGHT_FORWARD(LOW);
	}
	LEFT_UP(LOW);
}

// lift left middle feet to greet
static void greet() {
	digitalWrite(leg_middle_right, HIGH);
	digitalWrite(move_inner_right, HIGH);
	delay(1200);
	digitalWrite(move_inner_right, LOW);
	delay(300);
	digitalWrite(leg_middle_right, LOW);
	delay(123 * random(8, 12));
}

// dance!
static void rythm() {
	static int legs[] = {
		leg_front_left,
		leg_middle_left,
		leg_back_left,
		leg_front_right,
		leg_middle_right,
		leg_back_right,
	};
	static byte r[] = {
		17,
		0,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		0,
		0,
		0,
		1,
		0,
		0,
		0,
	};
	byte state = 0;
	for (int p=0;p<sizeof(r)/sizeof(*r);p++) {
		long tick = millis();
		byte change = state ^ r[p];
		for (int b=0;b<6;b++) {
			if (change & 1 << b) {
				digitalWrite(legs[b], r[p] & 1 << b? HIGH: LOW);
			}
		}
		state = r[p];
		tick += 60000L / 120; // BPM
		while (millis() < tick) {
			delay(3);
		}
	}
}

static void laydown() {
	digitalWrite(leg_front_left, HIGH);
	digitalWrite(leg_back_left, HIGH);
	digitalWrite(leg_middle_left, HIGH);
	digitalWrite(leg_front_right, HIGH);
	digitalWrite(leg_back_right, HIGH);
	digitalWrite(leg_middle_right, HIGH);
	delay(123 * random(7, 12));
	digitalWrite(leg_front_left, LOW);
	digitalWrite(leg_back_left, LOW);
	digitalWrite(leg_middle_left, LOW);
	digitalWrite(leg_front_right, LOW);
	digitalWrite(leg_back_right, LOW);
	digitalWrite(leg_middle_right, LOW);
}

// tilt body down
static void tilt() {
	for (int i=0;i<3;i++) {
		digitalWrite(move_outer_right, HIGH);
		digitalWrite(move_inner_right, HIGH);
		digitalWrite(move_outer_left, HIGH);
		digitalWrite(move_inner_left, HIGH);
		delay(123 * random(7, 12));
		digitalWrite(move_outer_right, LOW);
		digitalWrite(move_inner_right, LOW);
		digitalWrite(move_outer_left, LOW);
		digitalWrite(move_inner_left, LOW);
		delay(123 * random(7, 12));
	}
}

// take one random action
void loop() {
	switch (random(12)) {
		case 0:
			delay(123 * random(3, 7));
			break;
		case 1:
			sitdown();
			break;
		case 2:
			greet();
			break;
		case 3:
			for (int i=0;i<random(1,3);i++)
				checkleg();
			break;
		case 4:
			for (int i=0;i<random(3,7);i++)
				rythm();
			break;
		case 5:
			for (int i=0;i<random(1,3);i++)
				shake();
			break;
		case 6:
			laydown();
			break;
		case 7:
			tilt();
			break;
		default:
			walk(random(10, 30));
		break;
	}
}

