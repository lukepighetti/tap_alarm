#include <math.h>

bool test = false;

int r1 = 4;
int r2 = 5;
int r3 = 6;
int r4 = 7;

void setup() {
  Serial.begin(9600);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(r4, OUTPUT);

  setTime(20); /// 8PM in 24hr time

  if (test == true) {
    print("Starting in test mode...");
  } else {
    print("Starting in production mode!");
  }
}

/// The loop to use in test
void testLoop() {
  if (currentTime() >= 20.01 && currentTime() <= 20.02) {
    print("Do a randomDance()!");
    randomDance();
  }

  print("currentTime() -> " + String(currentTime()));

  delay(5000);
}

/// The loop to use in production
void productionLoop() {
  /// From 11PM - 5AM, do a random dance every 45-60 minutes
  if (currentTime() >= 23 || currentTime() <= 5) {
    randomDance();
    delayMinutes(45 + random(15));
  }

  /// Throw in a minute delay just in case we get a race condition
  delayMinutes(1);
}

void loop() {
  if (test == true) {
    testLoop();
  } else {
    productionLoop();
  }
  allOff();
}

/// Pick a dance at random, and do it! JUST DO IT!
void randomDance() {
  switch (random(1, 5)) {
    case 1:
      spinDance();
      break;
    case 2:
      gallopDance();
      break;
    case 3:
      twirlDance();
      break;
    case 4:
      tapKnock();
      break;
    case 5:
    default:
      shaveAndAHaircut();
      break;
  }

  allOff();
}

/// 1,2,3,4,3,2,1 repeat
void spinDance() {
  dance(
    "*-----*-----*-----*-----*-----*-----*---",
    "-*---*-*---*-*---*-*---*-*---*-*---*-*--",
    "--*-*---*-*---*-*---*-*---*-*---*-*---*-",
    "---*-----*-----*-----*-----*-----*-----*"
  );
}

/// I'm a cowgirl, and I'm riding on my horse.
/// Bang-da-doo-bang, and I'm shootin' all around.
void gallopDance() {
  dance(
    "**--*----------------------------------------*--**",
    "-----*--**--*------------------------*--**--*-----",
    "-------------*--**--*--------*--**--*-------------",
    "---------------------*--**--*---------------------"
  );
}

/// 1,2,3,4 repeat
void twirlDance() {
  dance(
    "*---*---*---*---*---*---*---*---*---*---",
    "-*---*---*---*---*---*---*---*---*---*--",
    "--*---*---*---*---*---*---*---*---*---*-",
    "---*---*---*---*---*---*---*---*---*---*"
  );
}

/// Tap... tap... tap... KNOCK KNOCK KNOCK.
void tapKnock() {
  dance(
    "*------------------------------*-*-*",
    "-------*-----------------------*-*-*",
    "--------------*----------------*-*-*",
    "---------------------*---------*-*-*"
  );
}

/// Shave and a haircut, two bits!
void shaveAndAHaircut() {
  dance(
    //1.2.3.4.1.2.3.4.
    "*-----------*---",
    "----------*-----",
    "----*-----*-----",
    "--**--*-----*---"
  );
}

/// Perform a dance based on a string for each solenoid
/// where * triggers a fire and the time is based on the length
/// of the first string.
///
/// eg "*---*---*---" is a single tap, three pauses, repeating three times
void dance(String pin1, String pin2, String pin3, String pin4) {
  int dwell = 50;
  int tick = 100;

  for (int i = 0; i < pin1.length(); i++) {
    bool fire1 = pin1[i] == char('*');
    bool fire2 = pin2[i] == char('*');
    bool fire3 = pin3[i] == char('*');
    bool fire4 = pin4[i] == char('*');

    if (fire1) {
      digitalWrite(r1, HIGH);
    }

    if (fire2) {
      digitalWrite(r2, HIGH);
    }

    if (fire3) {
      digitalWrite(r3, HIGH);
    }

    if (fire4) {
      digitalWrite(r4, HIGH);
    }

    delay(dwell);
    allOff();
    delay(tick - dwell);
  }
}

/// Turn all the solenoids off, a convenience method
/// to make it easy to ensure things aren't being left on.
/// Use liberally, apply where needed.
void allOff() {
  digitalWrite(r1, LOW);
  digitalWrite(r2, LOW);
  digitalWrite(r3, LOW);
  digitalWrite(r4, LOW);
}

/// A convenience method for printing to serial logs
void print(String str) {
  Serial.print(str + "\n");
}

void delayMinutes(int minutes) {
  delay(minutes * 60 * 1000);
}

/// A collection of convenience methods to provide us the ability to
/// schedule tasks by the hour
double timeOffset = 0;

double setTime(double hours) {
  timeOffset = hours;
}

double currentHours() {
  return ((double) millis()) / 60 / 60 / 1000;
}

double currentTime() {
  return fmod(currentHours(), 24) + timeOffset;
}
