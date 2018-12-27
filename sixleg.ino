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

static const int wait_delay = 500; // wait time after a atomic move until final position is reached

static void init_legs() {
	Serial.println("sixleg init_legs");
	digitalWrite(head_pan_left, LOW);
	digitalWrite(head_pan_right, LOW);
	digitalWrite(head_tilt, LOW);
	digitalWrite(leg_back_left_move, LOW);
	digitalWrite(leg_back_left_up, LOW);
	digitalWrite(leg_back_right_move, LOW);
	digitalWrite(leg_back_right_up, LOW);
	digitalWrite(leg_front_left_move, LOW);
	digitalWrite(leg_front_left_up, LOW);
	digitalWrite(leg_front_right_move, LOW);
	digitalWrite(leg_front_right_up, LOW);
	digitalWrite(leg_middle_left_move, LOW);
	digitalWrite(leg_middle_left_up, LOW);
	digitalWrite(leg_middle_right_move, LOW);
	digitalWrite(leg_middle_right_up, LOW);
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
	pinMode(head_tilt, OUTPUT);
	pinMode(head_pan_left, OUTPUT);
	pinMode(head_pan_right, OUTPUT);
	pinMode(tail_up, OUTPUT);
	init_legs();
}

// check all legs counterclockwise
#define CHECK(up, back) \
	digitalWrite(up, HIGH); \
	delay(wait_delay); \
	digitalWrite(back, HIGH); \
	delay(wait_delay); \
	digitalWrite(back, LOW); \
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
		CHECK(head_pan_right, head_pan_left);
		CHECK(tail_up, head_tilt);
	}
}

// try to pivot
static void pivot() {
	digitalWrite(leg_back_left_up, HIGH);
	digitalWrite(leg_back_right_up, HIGH);
	digitalWrite(leg_front_left_up, HIGH);
	digitalWrite(leg_front_right_up, HIGH);
	for (int i=0;i<5;i++) {
		delay(wait_delay*3);
		digitalWrite(leg_front_left_up, LOW);
		delay(wait_delay/2);
		digitalWrite(leg_front_left_up, HIGH);
		delay(wait_delay*3);
		digitalWrite(leg_back_left_up, LOW);
		delay(wait_delay/2);
		digitalWrite(leg_back_left_up, HIGH);
		delay(wait_delay*3);
		digitalWrite(leg_front_right_up, LOW);
		delay(wait_delay/2);
		digitalWrite(leg_front_right_up, HIGH);
		delay(wait_delay*3);
		digitalWrite(leg_back_right_up, LOW);
		delay(wait_delay/2);
		digitalWrite(leg_back_right_up, HIGH);
	}
	init_legs();
	delay(wait_delay);
}

// sit down
static void sitdown() {
	// legs forward
	digitalWrite(leg_back_left_move, HIGH);
	digitalWrite(leg_back_right_move, HIGH);
	digitalWrite(leg_front_left_move, HIGH);
	digitalWrite(leg_front_right_move, HIGH);
	digitalWrite(leg_middle_left_move, HIGH);
	digitalWrite(leg_middle_right_move, HIGH);
	// middle legs up
	digitalWrite(leg_middle_left_up, HIGH);
	digitalWrite(leg_middle_right_up, HIGH);
	delay(wait_delay);
	// rear legs up
	digitalWrite(leg_back_right_up, HIGH);
	digitalWrite(leg_back_left_up, HIGH);
	delay(wait_delay);
	// sitting
	delay(123 * random(9, 11));
	// stand up
	digitalWrite(leg_middle_left_up, LOW);
	digitalWrite(leg_middle_right_up, LOW);
	delay(wait_delay);
	digitalWrite(leg_back_left_up, LOW);
	digitalWrite(leg_back_right_up, LOW);
	delay(wait_delay*2);
	// legs back
	digitalWrite(leg_back_left_move, LOW);
	digitalWrite(leg_back_right_move, LOW);
	digitalWrite(leg_front_left_move, LOW);
	digitalWrite(leg_front_right_move, LOW);
	digitalWrite(leg_middle_left_move, LOW);
	digitalWrite(leg_middle_right_move, LOW);
	delay(wait_delay*2);
}

// shake
static void shake() {
	digitalWrite(leg_middle_right_up, HIGH);
	digitalWrite(leg_middle_left_up, HIGH);
	delay(wait_delay);
	for (int i=0;i<random(1,3);i++) {
		digitalWrite(leg_middle_left_move, HIGH);
		digitalWrite(leg_middle_right_move, HIGH);
		delay(wait_delay);
		digitalWrite(leg_middle_left_move, LOW);
		digitalWrite(leg_middle_right_move, LOW);
		delay(wait_delay);
	}
	digitalWrite(leg_middle_right_up, LOW);
	digitalWrite(leg_middle_left_up, LOW);
}

// walk count steps
#define LEFT_UP(S) \
	Serial.println(S==HIGH?"walk LEFT_UP":"walk LEFT_LOW"); \
	digitalWrite(leg_front_left_up, S); \
	digitalWrite(leg_middle_right_up, S); \
	digitalWrite(leg_back_left_up, S);
#define LEFT_FORWARD(S) \
	Serial.println(S==HIGH?"walk LEFT_FORWARD":"walk LEFT_BACKWARD"); \
	digitalWrite(leg_front_left_move, S); \
	digitalWrite(leg_middle_right_move, S); \
	digitalWrite(leg_back_left_move, S); \
	digitalWrite(head_pan_right, HIGH); \
	digitalWrite(head_pan_left, HIGH);
#define RIGHT_UP(S) \
	Serial.println(S==HIGH?"walk RIGHT_UP":"walk RIGHT_LOW"); \
	digitalWrite(leg_front_right_up, S); \
	digitalWrite(leg_middle_left_up, S); \
	digitalWrite(leg_back_right_up, S);
#define RIGHT_FORWARD(S) \
	Serial.println(S==HIGH?"walk RIGHT_FORWARD":"walk RIGHT_BACKWARD"); \
	digitalWrite(leg_front_right_move, S); \
	digitalWrite(leg_middle_left_move, S); \
	digitalWrite(leg_back_right_move, S); \
	digitalWrite(head_pan_right, LOW); \
	digitalWrite(head_pan_left, LOW);
static void walk(const int count) {
	Serial.println("walk");
	LEFT_UP(HIGH);
	for (int i=0;i<count;i++) {
		// take one full step (all phases)
		LEFT_FORWARD(LOW);
		delay(wait_delay);
		LEFT_UP(LOW);
		delay(wait_delay);
		RIGHT_UP(HIGH);
		delay(wait_delay/3);
		LEFT_FORWARD(HIGH);
		RIGHT_FORWARD(LOW);
		delay(wait_delay);
		RIGHT_UP(LOW);
		delay(wait_delay);
		LEFT_UP(HIGH);
		delay(wait_delay/3);
		RIGHT_FORWARD(HIGH);
	}
	init_legs();
}

// lift feet to greet
static void greet() {
	digitalWrite(leg_front_right_up, HIGH);
	delay(wait_delay*3);
	digitalWrite(leg_front_right_move, HIGH);
	delay(wait_delay * random(8, 12));
	digitalWrite(leg_front_right_move, LOW);
	delay(wait_delay);
	digitalWrite(leg_front_right_up, LOW);
	delay(wait_delay);
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
		digitalWrite(leg_back_left_move, HIGH);
		digitalWrite(leg_back_right_move, HIGH);
		digitalWrite(leg_front_left_move, HIGH);
		digitalWrite(leg_front_right_move, HIGH);
		digitalWrite(leg_middle_left_move, HIGH);
		digitalWrite(leg_middle_right_move, HIGH);
		delay(wait_delay * random(7, 12));
		digitalWrite(leg_back_left_move, LOW);
		digitalWrite(leg_back_right_move, LOW);
		digitalWrite(leg_front_left_move, LOW);
		digitalWrite(leg_front_right_move, LOW);
		digitalWrite(leg_middle_left_move, LOW);
		digitalWrite(leg_middle_right_move, LOW);
		delay(wait_delay * random(3, 7));
	}
}

// take one random action
void loop() {
	switch (random(12)) {
		case 0: delay(wait_delay * random(3, 7)); break;
		case 1: sitdown(); break;
		case 2: greet(); break;
		case 3: checkleg(random(1,3)); break;
		case 4: pivot(); break;
		case 5: shake(); break;
		case 6: laydown(); break;
		case 7: tilt(); break;
		//case 8: rythm(); break;
		default: walk(20*random(1,3)); break;
	}
	delay(wait_delay);
}

