/************************************************************************************************************
 * WindSpeed.cpp - Arduino library for making an extractor into an anenometer         			    *                                                                                  *
 * Copyright 2014 Joel Cormack (joel.greta@gmail.com)                                       		    *
 ************************************************************************************************************

 ************************************************************************************************************
 * This library is free software; you can redistribute it and/or                                            *
 * modify it under the terms of the GNU Lesser General Public                                               *
 * License as published by the Free Software Foundation; either                                             *
 * version 2.1 of the License, or (at your option) any later version.                                       *
 *                                                                                                          *
 * This library is distributed in the hope that it will be useful,                                          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                                           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                                        *
 * Lesser General Public License for more details.                                                          *
 *                                                                                                          *
 * You should have received a copy of the GNU Lesser General Public                                         *
 * License along with this library; if not, write to the Free Software                                      *
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA                               *
 ***********************************************************************************************************/
#define _USE_MATH_DEFINES
#include <math.h>
#include "Arduino.h"
#include "WindSpeed.h"

/*this library gives back the miles per hour speed of a wind extractor. the magnet is fixed to the circumference of the extractor and a
 *hall effect sensor is wired into an arduino. the class takes the pin number of the hall effect sensor, the circumference of the extractor
 *and the time interval between measurements or frequency of measurements.
 *
 *pin = microprocessor pin intput
 *radius = radius of center of extractor to sensor in centimeters
 *interval = time interval in milliseconds */

WindSpeed::WindSpeed(int pin, int radius, int interval) {
  
    _pin=pin;
    _radius=radius;
    _interval=interval;
    
    analogReference(DEFAULT);
}

//the following function uses the formulae from http://www.dfrobot.com/wiki/index.php/SHARP_GP2Y0A41SK0F_IR_ranger_sensor_%284-30cm%29_SKU:SEN0143
//converts the signal from the analog pin to cm
double WindSpeed::mph() {
	_rotationsPerInterval = countrotations();			//call countrotations
	_rotationsPerSecond = (_rotationsPerInterval/_interval)*1000;	//convert roations back to milliseconds and multiply by 1000 to get rotations per second
	_metersPerSecond = _rotationsPerSecond * circuference;		//times the distance in meters with roations per hour to get meters per hour
	_mph = mps2mph(_metersPerSecond);
	return _mph;
	
}

float WindSpeed::mps2mph(float _mps){
    return _mps * 2.2369362920544;
}

int WindSpeed::circumference(){
    _radiusMeters = _radius/100;
    return (2 * _radiusMeters) * M_PI;
}

/*this function calculates the rotations of the extractor every interval
*and returns rotations per interval
*/
		
int WindSpeed::countRotations(){
    _rotations=0;
    //while time passed is less than interval, count rotations
    while((unsigned long)(millis() - _previousMillis) <= _interval){
	if(!digitalRead (_pin)){		//if magnet is near sensor
	    _rotations++;			//add one to rotations
	    while(!digitalRead (_pin)){		//while the sensor is open
	    }//end while			//do nothing special..
	}//end if
    }//end while
    return _rotations;
}
