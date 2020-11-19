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

// Default long press delay is 1s (1000ms), but can be changed at initialization
#define LP_DELAY 500

#include <Arduino.h>
#include <ButtonDebouncer.h>

#define BUTTON_PIN          4

void callback(uint8_t pin, uint8_t event, uint8_t count, uint16_t length)
{
  if (event == EVENT_LONG_PRESSED)
  {
    Serial.print("Event : "); Serial.print(event);
    Serial.print(" Length: "); Serial.print(length);
    Serial.println();
  }
}

ButtonDebouncer button = ButtonDebouncer(BUTTON_PIN, callback, BUTTON_PUSHBUTTON | BUTTON_DEFAULT_HIGH | BUTTON_SET_PULLUP, DEBOUNCE_DELAY, REPEAT_DELAY, LP_DELAY);

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println();
}

void loop() {
  button.loop();
}
