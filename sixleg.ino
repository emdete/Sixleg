static const int leg_front_left_up = 11;
static const int leg_back_left_up = 9;
static const int leg_middle_left_up = 10;
static const int leg_front_right_up = 6;
static const int leg_back_right_up = 4;
static const int leg_middle_right_up = 5;

static const int leg_front_left_move = 12;
static const int leg_back_left_move = 14;
static const int leg_middle_left_move = 13;
static const int leg_front_right_move = 17;
static const int leg_back_right_move = 16;
static const int leg_middle_right_move = 15;

static const int head_tilt = 8;
static const int head_pan_right = 2;
static const int head_pan_left = 3;
static const int tail_up = 7;

static const int wait_delay = 600; // wait time after a atomic move until final position is reached

static void init_legs() {
	Serial.println("sixleg init_legs");
	digitalWrite(leg_front_left_move, LOW);
	digitalWrite(leg_front_left_up, LOW);
	digitalWrite(leg_back_left_move, LOW);
	digitalWrite(leg_back_left_up, LOW);
	digitalWrite(leg_middle_left_up, LOW);
	digitalWrite(leg_front_right_up, LOW);
	digitalWrite(leg_back_right_up, LOW);
	digitalWrite(leg_middle_right_up, LOW);
	digitalWrite(leg_front_right_move, LOW);
	digitalWrite(leg_middle_right_move, LOW);
	digitalWrite(leg_front_left_move, LOW);
	digitalWrite(leg_middle_left_move, LOW);
	digitalWrite(head_up, LOW);
	digitalWrite(head_left, LOW);
	digitalWrite(tail_up, LOW);
}

// (re)setup unit
void setup() {
	Serial.begin(38400);
	Serial.println("sixleg init");
	randomSeed(analogRead(0));
	pinMode(leg_back_left_move, OUTPUT);
	pinMode(leg_back_left_up, OUTPUT);
	pinMode(leg_back_right_move, OUTPUT);
	pinMode(leg_back_right_up, OUTPUT);
	pinMode(leg_front_left_move, OUTPUT);
	pinMode(leg_front_left_up, OUTPUT);
	pinMode(leg_front_right_move, OUTPUT);
	pinMode(leg_front_right_up, OUTPUT);
	pinMode(leg_middle_left_move, OUTPUT);
	pinMode(leg_middle_left_up, OUTPUT);
	pinMode(leg_middle_right_move, OUTPUT);
	pinMode(leg_middle_right_up, OUTPUT);
	pinMode(head_up, OUTPUT);
	pinMode(head_left, OUTPUT);
	pinMode(tail_up, OUTPUT);
	init_legs();
}

// check all legs counterclockwise
#define CHECK(up, back) \
	digitalWrite(up, HIGH); \
	delay(wait_delay); \
	digitalWrite(back, LOW); \
	delay(wait_delay); \
	digitalWrite(back, HIGH); \
	delay(wait_delay); \
	digitalWrite(up, LOW); \
	delay(wait_delay);
static void checkleg(int count) {
	for (int i=0;i<count;i++) {
		CHECK(leg_front_left_up, leg_front_left_move);
		CHECK(leg_middle_left_up, leg_middle_left_move);
		CHECK(leg_back_left_up, leg_back_left_move);
		CHECK(leg_back_right_up, leg_back_right_move);
		CHECK(leg_middle_right_up, leg_middle_right_move);
		CHECK(leg_front_right_up, leg_front_right_move);
	}
}

// try to pivot
static void pivot() {
/*	digitalWrite(leg_middle_right_up, HIGH);
	digitalWrite(leg_middle_right_move, HIGH);
	delay(wait_delay);
	digitalWrite(leg_middle_right_up, LOW);*/
	digitalWrite(leg_front_left_up, HIGH);
	digitalWrite(leg_back_left_up, HIGH);
	digitalWrite(leg_front_right_up, HIGH);
	digitalWrite(leg_back_right_up, HIGH);
	for (int i=0;i<5;i++) {
		delay(900);
		digitalWrite(leg_front_left_up, LOW);
		delay(100);
		digitalWrite(leg_front_left_up, HIGH);
		delay(900);
		digitalWrite(leg_back_left_up, LOW);
		delay(100);
		digitalWrite(leg_back_left_up, HIGH);
		delay(900);
		digitalWrite(leg_front_right_up, LOW);
		delay(100);
		digitalWrite(leg_front_right_up, HIGH);
		delay(900);
		digitalWrite(leg_back_right_up, LOW);
		delay(100);
		digitalWrite(leg_back_right_up, HIGH);
	}
	digitalWrite(leg_front_left_up, LOW);
	digitalWrite(leg_back_left_up, LOW);
	digitalWrite(leg_front_right_up, LOW);
	digitalWrite(leg_back_right_up, LOW);
	delay(300);
	digitalWrite(leg_middle_right_up, HIGH);
	digitalWrite(leg_middle_right_move, LOW);
	delay(wait_delay);
	digitalWrite(leg_middle_right_up, LOW);
}

// sit down
static void sitdown() {
	// legs forward
	digitalWrite(leg_front_right_move, HIGH);
	digitalWrite(leg_middle_right_move, HIGH);
	digitalWrite(leg_front_left_move, HIGH);
	digitalWrite(leg_middle_left_move, HIGH);
	// middle legs up
	digitalWrite(leg_middle_left_up, HIGH);
	digitalWrite(leg_middle_right_up, HIGH);
	delay(wait_delay);
	// rear legs up
	digitalWrite(leg_back_right_up, HIGH);
	digitalWrite(leg_back_left_up, HIGH);
	// sitting
	delay(123 * random(9, 11));
	// stand up
	digitalWrite(leg_front_left_up, LOW);
	digitalWrite(leg_back_left_up, LOW);
	digitalWrite(leg_middle_left_up, LOW);
	digitalWrite(leg_front_right_up, LOW);
	digitalWrite(leg_back_right_up, LOW);
	digitalWrite(leg_middle_right_up, LOW);
	delay(900);
	digitalWrite(leg_front_right_move, LOW);
	digitalWrite(leg_middle_right_move, LOW);
	digitalWrite(leg_front_left_move, LOW);
	digitalWrite(leg_middle_left_move, LOW);
	delay(900);
}

// shake
static void shake() {
	for (int i=0;i<random(1,3);i++) {
		digitalWrite(leg_middle_right_up, HIGH);
		digitalWrite(leg_middle_left_up, HIGH);
		digitalWrite(leg_middle_left_move, HIGH);
		digitalWrite(leg_middle_right_move, HIGH);
		delay(400);
		digitalWrite(leg_middle_left_move, LOW);
		digitalWrite(leg_middle_right_move, LOW);
		delay(400);
	}
	digitalWrite(leg_middle_right_up, LOW);
	digitalWrite(leg_middle_left_up, LOW);
}

// walk count steps
#define LEFT_UP(S) \
	Serial.println(S==HIGH?"walk LEFT_UP":"walk LEFT_LOW"); \
	digitalWrite(leg_front_left_up, S); \
	digitalWrite(leg_back_left_up, S); \
	digitalWrite(leg_middle_right_up, S); \
	delay(wait_delay);
#define LEFT_FORWARD(S) \
	Serial.println(S==HIGH?"walk LEFT_FORWARD":"walk LEFT_BACKWARD"); \
	digitalWrite(leg_front_left_move, S); \
	digitalWrite(leg_middle_right_move, S); \
	digitalWrite(leg_back_right_move, S); \
	delay(wait_delay);
#define RIGHT_UP(S) \
	Serial.println(S==HIGH?"walk RIGHT_UP":"walk RIGHT_LOW"); \
	digitalWrite(leg_middle_left_up, S); \
	digitalWrite(leg_front_right_up, S); \
	digitalWrite(leg_back_right_up, S); \
	delay(wait_delay);
#define RIGHT_FORWARD(S) \
	Serial.println(S==HIGH?"walk RIGHT_FORWARD":"walk RIGHT_BACKWARD"); \
	digitalWrite(leg_front_right_move, S); \
	digitalWrite(leg_middle_left_move, S); \
	digitalWrite(leg_back_left_move, S); \
	delay(wait_delay);
static void walk(const int count) {
	Serial.println("walk");
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
	delay(wait_delay);
}

// lift left middle feet to greet
static void greet() {
	digitalWrite(leg_middle_right_up, HIGH);
	digitalWrite(leg_middle_right_move, HIGH);
	delay(1200);
	digitalWrite(leg_middle_right_move, LOW);
	delay(300);
	digitalWrite(leg_middle_right_up, LOW);
	delay(123 * random(8, 12));
}

// dance!
static void rythm() {
	static int legs[] = {
		leg_front_left_up,
		leg_middle_left_up,
		leg_back_left_up,
		leg_front_right_up,
		leg_middle_right_up,
		leg_back_right_up,
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
	for (int i=0;i<random(3,7);i++) {
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
}

static void laydown() {
	digitalWrite(leg_front_left_up, HIGH);
	digitalWrite(leg_back_left_up, HIGH);
	digitalWrite(leg_middle_left_up, HIGH);
	digitalWrite(leg_front_right_up, HIGH);
	digitalWrite(leg_back_right_up, HIGH);
	digitalWrite(leg_middle_right_up, HIGH);
	delay(123 * random(7, 12));
	digitalWrite(leg_front_left_up, LOW);
	digitalWrite(leg_back_left_up, LOW);
	digitalWrite(leg_middle_left_up, LOW);
	digitalWrite(leg_front_right_up, LOW);
	digitalWrite(leg_back_right_up, LOW);
	digitalWrite(leg_middle_right_up, LOW);
}

// tilt body down
static void tilt() {
	for (int i=0;i<3;i++) {
		digitalWrite(leg_front_right_move, HIGH);
		digitalWrite(leg_middle_right_move, HIGH);
		digitalWrite(leg_front_left_move, HIGH);
		digitalWrite(leg_middle_left_move, HIGH);
		delay(123 * random(7, 12));
		digitalWrite(leg_front_right_move, LOW);
		digitalWrite(leg_middle_right_move, LOW);
		digitalWrite(leg_front_left_move, LOW);
		digitalWrite(leg_middle_left_move, LOW);
		delay(123 * random(7, 12));
	}
}

// take one random action
void loop() {
	//switch (random(12)) {
		//case 0: delay(123 * random(3, 7)); break;
		//case 1: sitdown(); break;
		//case 2: greet(); break;
		//case 3: checkleg(random(1,3)); break;
		//case 4: rythm(); break;
		//case 5: shake(); break;
		//case 6: laydown(); break;
		//case 7: tilt(); break;
		//default:
			walk(random(10, 30));
		//break;
	//}
	delay(10000);
}

