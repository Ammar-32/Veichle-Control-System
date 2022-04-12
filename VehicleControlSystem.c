/*
 ============================================================================
 Name        : C_Project.c
 Author      : Ammar Moataz
 Description : Veichle Control System Project
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#define WITH_ENGINE_TEMP_CONTROLLER 1
char input; // Global variable for the user inputs in console

/* enumerated the states OFF and ON */
enum state{
	OFF=0, ON=1
};

/* Added all variables for a vehicle in a struct */
struct Vehicle{
	enum state engineState;
	enum state AC;
	unsigned short vehicleSpeed;
	float roomTemperature;
	enum state engineTemperatureControllerState;
	float engineTemperature;
	char trafficLight;
};

/* Function Prototypes*/
void displayMainMenu(void); // function to display main menu in console
void displaySensorSetMenu(void); // function to display main menu in console
void engineOFF(struct Vehicle * v); // function to be called when engine is OFF
void engineON(struct Vehicle * v); // function to be called when engine is ON
void setTrafficLight(struct Vehicle * v); // function to set traffic light color
void displayVehicle(struct Vehicle * v); // function to display vehicle state in console
void processingTrafficLight(struct Vehicle * v); // function to adapt to traffic light color changes
void setRoomTemperature(struct Vehicle * v); // function to set room temperature
void processingRoomTemperature(struct Vehicle * v); // function to adapt to room temperature changes
void setEngineTemperature(struct Vehicle * v); // function to set engine temperature
void processingEngineTemperature(struct Vehicle * v); // function to adapt to engine temperature changes
void processingVehicleSpeed(struct Vehicle * v);// function to adapt to vehicle speed changes
void processingChanges(struct Vehicle * v);// function to adapt to all changes

/* Main Function */
int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	struct Vehicle v = {OFF,OFF,100,20.0,OFF,125.0,'G'}; // initial vehicle states
	while(1){
		if(v.engineState==OFF){
			engineOFF(&v);
			if(input=='c'){
				return 0;
			}
		}else{
			engineON(&v);
		}
	}
	return 0;
}

/* Function Definitions */
void displayMainMenu(void){
	printf("a. Turn on the vehicle engine\n");
	printf("b. Turn off the vehicle engine\n");
	printf("c. Quit the system\n");
	printf("\n");
}

void displaySensorSetMenu(void){
	printf("a. Turn off the engine\n");
	printf("b. Set the traffic light color\n");
	printf("c. Set the room temperature (Temperature Sensor)\n");
#if WITH_ENGINE_TEMP_CONTROLLER
	printf("d. Set the engine temperature (Engine Temperature Sensor)\n");
#endif
	printf("\n");
}

void engineOFF(struct Vehicle* v){
	displayMainMenu();
	scanf(" %c",&input);
	switch(input){
	case 'a':
		v->engineState=ON; // turn on engine
		break;
	case 'b':
		printf("Vehicle turned Off\n\n");
		break;
	case 'c':
		printf("Quit the system\n");
		return;
	default:
		fprintf(stderr, "Enter a valid character !\n\n");
		break;
	}
}

void engineON(struct Vehicle* v){
	displaySensorSetMenu();
	scanf(" %c",&input);
	switch(input){
	case 'a':
		v->engineState=OFF;
		break;
	case 'b':
		setTrafficLight(v);
		processingChanges(v);
		displayVehicle(v);
		break;
	case 'c':
		setRoomTemperature(v);
		processingChanges(v);
		displayVehicle(v);
		break;
#if WITH_ENGINE_TEMP_CONTROLLER
	case 'd':
		setEngineTemperature(v);
		processingChanges(v);
		displayVehicle(v);
		break;
#endif
	default:
		fprintf(stderr, "Enter a valid character !\n\n");
		break;
	}
}

void setTrafficLight(struct Vehicle * v){
	printf("Enter traffic light color : \n");
	scanf(" %c",&input);
	if(input == 'G' || input == 'g' || input == 'O' || input == 'o' || input == 'R' || input == 'r'){
		v->trafficLight = input;
	}else{
		fprintf(stderr, "\nWrong Color, Enter a valid color\n");
		setTrafficLight(v);
	}
}

void displayVehicle(struct Vehicle * v){
	printf("Engine is %s\n", (v->engineState == 1)?"ON":"OFF");
	printf("AC is %s\n", (v->AC == 1)?"ON":"OFF");
	printf("Vehicle Speed: %d km/hr\n", v->vehicleSpeed);
	printf("Room Temperature: %.2f C\n", v->roomTemperature);
#if WITH_ENGINE_TEMP_CONTROLLER
	printf("Engine Temperature Controller is %s\n", (v->engineTemperatureControllerState == 1)?"ON":"OFF");
	printf("Engine Temperature: %.2f C\n\n", v->engineTemperature);
#endif
	printf("\n");
}

void processingTrafficLight(struct Vehicle * v){
	if(v->trafficLight=='G' || v->trafficLight=='g'){
		v->vehicleSpeed = 100;
	}else if(v->trafficLight == 'O' || v->trafficLight=='o'){
		v->vehicleSpeed = 30;
	}else{
		v->vehicleSpeed = 0;
	}
}

void setRoomTemperature(struct Vehicle * v){
	printf("Enter room temperature : \n");
	float roomTemp;
	scanf(" %f",&roomTemp);
	v->roomTemperature = roomTemp;
}

void processingRoomTemperature(struct Vehicle * v){
	if(v->roomTemperature < 10 || v->roomTemperature>30){
		v->AC=ON;
		v->roomTemperature=20;
	}else{
		v->AC=OFF;
	}
}

void setEngineTemperature(struct Vehicle * v){
	printf("Enter engine temperature : \n");
	float engineTemp;
	scanf(" %f",&engineTemp);
	v->engineTemperature = engineTemp;
}

void processingEngineTemperature(struct Vehicle * v){
	if(v->engineTemperature < 100 || v->engineTemperature>150){
		v->engineTemperatureControllerState=ON;
		v->engineTemperature=125;
	}else{
		v->engineTemperatureControllerState=OFF;
	}
}

void processingVehicleSpeed(struct Vehicle * v){
	if(v->vehicleSpeed==30){
		v->AC=ON;
		v->roomTemperature*=1.25;
		v->roomTemperature++;
#if WITH_ENGINE_TEMP_CONTROLLER
		v->engineTemperatureControllerState=ON;
		v->engineTemperature*=1.25;
		v->engineTemperature++;
#endif
	}
}

void processingChanges(struct Vehicle * v){
	processingTrafficLight(v);
	processingRoomTemperature(v);
	processingEngineTemperature(v);
	processingVehicleSpeed(v);
}
