#include <LiquidCrystal.h>

int button_pin = 9;
int led_pin = 7;

float random_number = 0;
float reaction_timer = 0;
int game_state = -1;

float reaction_time = 0;
float total_reaction_time = 0;

int current_round = 1;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
    Serial.begin(9600);

    pinMode(led_pin, OUTPUT);
    pinMode(button_pin, INPUT_PULLUP);

    lcd.begin(16, 2);
    delay(1000);
}

void loop() {
    reaction_game();
}

void reaction_game() {
    if (game_state == -1) {
        lcd.setCursor(0, 0);
        lcd.print("Click button");

        lcd.setCursor(0, 1);
        lcd.print("to begin game!");

        if (digitalRead(button_pin) == 0) {
            game_state = 0;
        }

    } else if (game_state == 0) {
        lcd.clear();
        lcd.print("Wait for it...");

        // Calculate a random time for the LED to start at
        randomSeed(analogRead(A0));
        random_number = random(1000, 7000);

        // Set the timer
        delay(random_number);

        // Start waiting for player to press button
        lcd.clear();
        lcd.print("Go!!");

        game_state = 1;
        reaction_timer = millis();

        digitalWrite(led_pin, 1);

    } else if (game_state == 1) {
        // Flash LED
        if (millis() == (reaction_timer + 100)) {
            digitalWrite(led_pin, 0);
        }

        // Calculate reaction time
        if (digitalRead(button_pin) == 0) {
            reaction_time = millis() - reaction_timer;
            game_state = 2;
        }

    } else if (game_state == 2) {
        // Print reaction time for that round
        Serial.println("Reaction time for that round: ");
        Serial.println(reaction_time);

        lcd.setCursor(0, 0);
        lcd.print("reaction time: ");

        lcd.setCursor(0, 1);
        lcd.print(reaction_time);

        delay(2000);

        total_reaction_time += reaction_time;

        // Check if 3 rounds have been done
        if (current_round == 3) {
            // Set to game over state
            game_state = 3;

        } else {
            // Start next round
            current_round += 1;
            game_state = 0;

            Serial.print("Next round!");

            lcd.setCursor(0, 0);
            lcd.print("Next round!");
        }

    } else if (game_state == 3) {
        // Game over state
        Serial.println("Average reaction time: ");
        Serial.println(total_reaction_time / 3);

        lcd.setCursor(0, 0);
        lcd.print("Avg react time:");

        lcd.setCursor(0, 1);
        lcd.print(total_reaction_time);
    }
}