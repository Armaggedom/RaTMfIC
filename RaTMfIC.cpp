/*
	Research and Test Module for Internal Communication (RaTMfIC)
	Modulo de Pesquisa e Teste para Comunicacao interna (MdPTpCI)
*/
//AnalogIN
#define VECTORINVERSION A0
#define ACCELEROMETER A1
//right movement
#define Rmov7 7	//R = right motor
#define Lmov2 2 //L = left motor
//left movement
#define Lmov4 4
#define Rmov8 8
// analog vel definer
#define Lvel 3
#define Rvel 5
//global variables
bool stopRequest=0;
//global function
void SafetyShutdownSystem();
//---------------------------predefinition code-----------------------------//
void setup() {
	//Serial initialize
	Serial.begin(9600);
	// digital pin
	pinMode(Lmov2,OUTPUT);
	pinMode(Lmov4, OUTPUT);
	pinMode(Rmov7, OUTPUT);
	pinMode(Rmov8, OUTPUT);
	// analog pin
	pinMode(Lvel, OUTPUT);
	// analog pre-definition
	analogWrite(Lvel, 0);
	analogWrite(Rvel, 0);

}
//---------------------------Loop code--------------------------------------//
void loop() {
	//local variable
	int aceleration=analogRead(ACCELEROMETER);
	//define function
	void VectorRectilinearInversion(int VectorValue, int aceleration); //function to reverse the acceleration vector
	int Acelerometer(int aceleration);
//-------------------program-------------------
	int VectorValue=analogRead(VECTORINVERSION);
	VectorValue=map(VectorValue, 0, 675, 0, 1);
	//calls the function to reverse the acceleration vector
	VectorRectilinearInversion(VectorValue, aceleration);
	//define ΔV
	analogWrite(Rvel, Acelerometer(aceleration));
	analogWrite(Lvel, Acelerometer(aceleration));
}
/*					    --->
	Acceleration vector: a m/s²
//							--->
-----------------------------V code-----------------------------------------//
*/
int Acelerometer(int aceleration) {
	aceleration=map(aceleration, 0, 1023, 0, 180);
//	Serial.println(aceleration);
	return aceleration;
}
bool key=1;
void VectorRectilinearInversion(int VectorValue, int aceleration) {
	
	/*							-->	
	vectorValue is direction the V
	key is a key to block motor
	*/
	void ProgressiveRectilinearAcceleration();
	void RetrogradeRectilinearAcceleration();	
	;
	
	//two-phase system
	if(VectorValue==0 and key==1) {
		RetrogradeRectilinearAcceleration();
	}
	else if(VectorValue==1 and key==0) {
		ProgressiveRectilinearAcceleration();
	}
	//transition phase
	else if(VectorValue==0 and key==0 or VectorValue==1 and key==1) {
		//aceleration not_eq 0
		// Airlock system
		if(VectorValue==0 and key==0) {
			stopRequest=1;
			key=1;
			SafetyShutdownSystem();
		}
		else if(VectorValue==1 and key==1) {
			stopRequest=1;
			key=0;
			SafetyShutdownSystem();
		}
		else {
			Serial.print("[VV-2] VectorValue and vetordirection ERROR\nVector value = ");
			Serial.println(VectorValue);
			Serial.print(" Vector directiom = ");
			Serial.println(key);
		};
	}
/*	else if(NULL) {
		movment equal to 0; rotation
	}
*/	else {
		//error escape
		Serial.print("[VV-1] VectorValue and vetordirection ERROR\nVector value = ");
		Serial.println(VectorValue);
		Serial.print(" Vector directiom = ");
		Serial.println(key);
		SafetyShutdownSystem();
	};
	Serial.println(key);
}
/*    						--->
-----------------------------V code-----------------------------------------//
*/
void ProgressiveRectilinearAcceleration() {
	Serial.print("ProgressiveRectilinearAcceleration mode activated\n");
	//turn on pinout
	digitalWrite(Rmov7, HIGH);
	digitalWrite(Lmov2, HIGH);
	//turn off pinout
	digitalWrite(Rmov8, LOW);
	digitalWrite(Lmov4, LOW);
}
/*						    --->
-----------------------------V*-1 code--------------------------------------//
VectorRectilinearInversion switch
*/
void RetrogradeRectilinearAcceleration() {
	Serial.print("RetrogradeRectilinearAcceleration mode activated\n");
	//turn on pinout
	digitalWrite(Rmov8, HIGH);
	digitalWrite(Lmov4, HIGH);
	//turn off pinout
	digitalWrite(Rmov7, LOW);
	digitalWrite(Lmov2, LOW);
}
//activated when errors occur or the motors and modules are requested to stop
void SafetyShutdownSystem() {
	Serial.print("SafetyShutdownSystem activated\n");
	if(stopRequest=1) {
		//shutdown only engines
		digitalWrite(Rmov7, 0);
		digitalWrite(Rmov8, 0);
		digitalWrite(Lmov4, 0);
		digitalWrite(Lmov2, 0);
	};
	//ShutdownSystem
}