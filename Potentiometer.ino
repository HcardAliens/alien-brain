// Potentiometer

void setupPotentiometer() {
	pinMode(potentiometer, INPUT);
}

int potValue() {
	int potentiometerValue = analogRead(potentiometer);
	Serial.print("Potentiometer value:");
	Serial.println(potentiometerValue);
	return potentiometerValue;
}
