/*

  Debounce buttons and trigger events long press
  Copyright (C) 2020-2020 by Daniel Carrasco <admin at electrosoftcloud dot com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <Arduino.h>
#include <ButtonDebouncer.h>

#define BUTTON_PIN              4

#define CUSTOM_DEBOUNCE_DELAY   50

// Time the library waits for a second (or more) clicks
// Set to 0 to disable double clicks but get a faster response
#define CUSTOM_REPEAT_DELAY     500

ButtonDebouncer * button;

void setup() {
    Serial.begin(9600);
    Serial.println();
    Serial.println();
    button = new ButtonDebouncer(BUTTON_PIN, BUTTON_PUSHBUTTON | BUTTON_DEFAULT_HIGH | BUTTON_SET_PULLUP, CUSTOM_DEBOUNCE_DELAY, CUSTOM_REPEAT_DELAY);
}

void loop() {
    if (unsigned int event = button->loop()) {
        if (event == EVENT_RELEASED) {
            Serial.print("Count : "); Serial.print(button->getEventCount());
            Serial.print(" Length: "); Serial.print(button->getEventLength());
            Serial.println();
        }
    }
}
