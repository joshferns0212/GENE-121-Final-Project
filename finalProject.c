/*Joshua Fernandes, Sasha Ohayon, Syed Muhammed Shayaan
"The Pong"
Motor guide: ballFeeder = motorA,
		rotateMotor = motorB,
		launchMotor1 = motorC,
		launchMotor2 = motorD;
*/

enum controllerState
{
	CNONE = 0,
	CLEFT = 1,
	CRIGHT = 2,
	CBOTH = 3,
};

int motorSpeed;

controllerState getControllerState()
{
	int touchCLEFT = SensorValue(S1),
	touchCRIGHT = SensorValue(S2);

	if(touchCLEFT == 0 && touchCRIGHT == 0)
		return CNONE;
	else if(touchCLEFT == 1 && touchCRIGHT == 0)
		return CLEFT;
	else if(touchCLEFT == 0 && touchCRIGHT == 1)
		return CRIGHT;
 	else
		return CBOTH;
}
void setMotor(int motorValue, int power, int direction)
{
	motor[motorValue] = power*direction;
}
void stepDown (int motor, int currentPower)
{
	for (int power = currentPower; power >= 0; power--)
	{
		setMotor (motor, power, 1);
		wait1Msec (25);
	}
}
bool selectOrientation()
{
	while(getControllerState() != CRIGHT)
	{
		if (getButtonPress(buttonEnter))
			return true;
	}

	if (getGyroDegrees(S3) <= 0)
		setMotor(motorB, 20, -1);
	else if (getGyroDegrees(S3) > 0)
  		setMotor(motorB, 20, 1);

	wait1Msec(500);

	while (getControllerState() != CRIGHT)
	{
  		if (getGyroDegrees(S3) < -35 || getGyroDegrees(S3) > 35)
  			setMotor(motorB, 20, getGyroDegrees(S3)/abs(getGyroDegrees(S3)));

  		if (getButtonPress(buttonEnter))
				return true;
	}
	setMotor(motorB, 0, 1);
	while (getControllerState() == CRIGHT);

	return false;
}
bool selectSpeed()
{
	while (getControllerState() != CRIGHT)
	{
		if (getButtonPress(buttonEnter))
			return true;
		if (getControllerState() == CLEFT)
		{
			while (getControllerState() == CLEFT);

			if (motorSpeed == 100)
			{
				stepDown (motorC, motorSpeed);
				stepDown (motorD, motorSpeed);
				motorSpeed = 0;
			}
			else
			{
				motorSpeed += 20;
				setMotor(motorC, motorSpeed, 1);
				setMotor(motorD, motorSpeed, 1);
			}
		}
	}
	return false;
}
void releaseBall()
{
	setMotor(motorA, 15, 1);
	nMotorEncoder[motorA] = 0 ;
	while(nMotorEncoder[motorA] < 120);
	setMotor(motorA, 0, 1);
}
void endProgram()
{
		if (getGyroDegrees(S3) != 0)
		{
			setMotor(motorB, 15, getGyroDegrees(S3)/abs(getGyroDegrees(S3);

			while (getGyroDegrees(S3) != 0);

			setMotor (motorB, 0, 1);
		}

		stepDown (motorC, motorSpeed);
		stepDown (motorD, motorSpeed);
}
task main()
{
	bool stopProgram = false;

	SensorType[S3] = sensorEV3_Gyro;
	SensorMode[S3] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
	resetGyro(S3);
	SensorType[S1] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[S2] = sensorEV3_Touch;
	wait1Msec(50);

	motorSpeed = 0;

	while (!stopProgram)
	{
		stopProgram = selectOrientation()
		if (stopProgram)
			continue;
		stopProgram = selectSpeed()
		if (stopProgram)
			continue;

		while (getControllerState() != CBOTH)
		{
			if (getButtonPress(buttonEnter))
			{
				stopProgram = true;
				break;
			}
		}
		if (stopProgram)
			continue;
		while (getControllerState() == CBOTH);

		releaseBall ();
	}

	endProgram();
}
