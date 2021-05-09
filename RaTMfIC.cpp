/*
	Research and Test Module for Internal Communication (RaTMfIC)
	Modulo de Pesquisa e Teste para Comunicacao interna (MdPTpCI)
*/
//AnalogIN
#define Test A0
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
bool vetordirection=1;
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
	//define function
	void VectorRectilinearInversion(int val); //function to reverse the acceleration vector
//-------------------program-------------------
	int val=analogRead(Test);
	val = map(val, 0, 675, 0, 1);
	//calls the function to reverse the acceleration vector
	VectorRectilinearInversion(val);
	//define ΔV
	analogWrite(Rvel, 255);
	analogWrite(Lvel, 255);
}
/*					    --->
	Acceleration vector: a m/s²
							--->
-----------------------------V code-----------------------------------------//
*/
void VectorRectilinearInversion(int val) {
	void ProgressiveRectilinearAcceleration();
	void RetrogradeRectilinearAcceleration();	
	Serial.println(vetordirection);
	//two-phase system
	if(val == 0 && vetordirection == 1) {
		RetrogradeRectilinearAcceleration();
	}
	else if(val == 1 && vetordirection == 0) {
		ProgressiveRectilinearAcceleration();
	}
	//transition phase
	else if(val == 0 && vetordirection == 0) {
		stopRequest=1;
		SafetyShutdownSystem();
		vetordirection=1;
	}
	else if(val == 1 && vetordirection == 1) {
		stopRequest=1;
		vetordirection=0;
		SafetyShutdownSystem();
	}
	else {
		//error escape
		SafetyShutdownSystem();
	};
}
/*							--->
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
		//stop engines
		digitalWrite(Rmov7, 0);
		digitalWrite(Rmov8, 0);
		digitalWrite(Lmov4, 0);
		digitalWrite(Lmov2, 0);
	};
	//ShutdownSystem
}