/*

  Debounce buttons and trigger events
  Copyright (C) 2020-2020 by Daniel Carrasco <admin at electrosoftcloud dot com>

  Based on Debounce Event library: https://github.com/xoseperez/debounceevent

  The ButtonDebouncer library is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The ButtonDebouncer library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with the ButtonDebouncer library.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <Arduino.h>
#include "ButtonDebouncer.h"


ButtonDebouncer::ButtonDebouncer(uint8_t pin, DEBOUNCE_EVENT_CALLBACK_SIGNATURE, uint8_t mode, unsigned long delay, unsigned long repeat, unsigned long long_press_delay, unsigned long burst_delay, unsigned long burst_interval) {
    this->callback = callback;
    _init(pin, mode, delay, repeat, long_press_delay, burst_delay, burst_interval);
}

ButtonDebouncer::ButtonDebouncer(uint8_t pin, uint8_t mode, unsigned long delay, unsigned long repeat, unsigned long long_press_delay, unsigned long burst_delay, unsigned long burst_interval) {
    this->callback = NULL;
    _init(pin, mode, delay, repeat, long_press_delay, burst_delay, burst_interval);
}


void ButtonDebouncer::_init(uint8_t pin, uint8_t mode, unsigned long delay, unsigned long repeat, unsigned long long_press_delay, unsigned long burst_delay, unsigned long burst_interval) {

    // store configuration
    _pin = pin;
    _mode = mode & 0x01;
    _defaultStatus = ((mode & BUTTON_DEFAULT_HIGH) > 0);
    _delay = delay;
    _repeat = repeat;
    _long_press_delay = long_press_delay;
    _burst_delay = burst_delay;
    _burst_interval = burst_interval;

    // set up button
    #if ESP8266
    if (_pin == 16) {
        if (_defaultStatus) {
            pinMode(_pin, INPUT);
        } else {
            pinMode(_pin, INPUT_PULLDOWN_16);
        }
    } else {
    #endif // ESP8266
        if ((mode & BUTTON_SET_PULLUP) > 0) {
            pinMode(_pin, INPUT_PULLUP);
        } else {
            pinMode(_pin, INPUT);
        }
    #if ESP8266
    }
    #endif // ESP8266

    _status = (_mode == BUTTON_SWITCH) ? digitalRead(_pin) : _defaultStatus;
    _last_status = _status;

}

unsigned char ButtonDebouncer::loop() {

    unsigned char event = EVENT_NONE;

    if (digitalRead(_pin) != _status) {
        if (digitalRead(_pin) != _last_status)
        {

            _last_event = millis();
            _last_status = digitalRead(_pin);

        }
        else if (millis() - _last_event > _delay)
        {
            _status = !_status;

            if (_mode == BUTTON_SWITCH)
            {

                event = EVENT_CHANGED;

            } else {

                // released
                if (_status == _defaultStatus)
                {

                    _event_length = millis() - _event_start;
                    _ready = true;

                // pressed
                }
                else
                {

                    event = EVENT_PRESSED;
                    _event_start = millis();
                    _event_length = 0;
                    if (_reset_count)
                    {
                        _event_count = 1;
                        _reset_count = false;
                    }
                    else
                    {
                        ++_event_count;
                    }
                    _ready = false;

                }
            }
        }
    }
    else if (digitalRead(_pin) != _last_status && (millis() - _last_event > _delay))
    {
        _last_status = digitalRead(_pin);
    }

    if (_ready && (millis() - _event_start > _repeat)) {
        _ready = false;
        _reset_count = true;
        event = EVENT_RELEASED;
        _long_press = false;
        _burst_count = 0;        
    }

    // Not compatible with button switch
    if (_mode != BUTTON_SWITCH && !_ready && _status != _defaultStatus)
    {
        if ((millis() - _event_start > _long_press_delay) && !_long_press){
            event = EVENT_LONG_PRESSED;
            _event_length = millis() - _event_start;
            _long_press = true;
        }
        else if ((millis() - _event_start > _burst_delay) && (((millis() - _event_start) / _burst_interval) > _burst_count))
        {
            event = EVENT_BURST;
            _burst_count = ((millis() - _event_start) / _burst_interval);
            _event_length = millis() - _event_start;
        }
    }

    if (event != EVENT_NONE) {
        if (this->callback) this->callback(_pin, event, _event_count, _event_length);
    }

    return event;

}