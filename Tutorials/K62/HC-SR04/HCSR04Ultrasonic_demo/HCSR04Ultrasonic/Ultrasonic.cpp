/*
 *Ultrasonic.cpp - Library for HC-SR04 Ultrasonic sensor module
 
 */
#include <stdio.h>
#include <stdlib.h>
#include <Ultrasonic.h>

Ultrasonic::Ultrasonic(int tp,int ep)
{
	pinMode(tp,OUTPUT);
	pinMode(ep,INPUT);
	_trigPin = tp;
	_EchoPin = ep;
	_cmDivisor = 27.6233;
	_inDivisor = 70.1633;
}

long Ultrasonic::timing()
{
	digitalWrite(_trigPin,LOW);
	delayMicroseconds(2);
	digitalWrite(_trigPin,HIGH);
	delayMicroseconds(10);
	digitalWrite(_trigPin,LOW);
	return pulseIn(_EchoPin,HIGH);
}

float Ultrasonic::CalcDistance(long microsec,int metric)
{
	if(metric)//centimeter divisor
		return microsec / _cmDivisor / 2.0;
	else //inches divisor
		return microsec /_inDivisor / 2.0;
}

void Ultrasonic::SetDivisor(float value,int metric)
{
	if(metric)//centimeter divisor
		_cmDivisor = value;
	else
		_inDivisor = value;
}