#include "F1.h"

// ------- DO NOT MODIFY ------- //

static void tire_pressure_status(void *data)
{
	tire_sensor *t = (tire_sensor *)data;
	if (t->pressure >= 21 && t->pressure <= 26) {
		printf("Tire has normal pressure.\n");
	} else if (t->pressure > 26 && t->pressure <= 28) {
		printf("Tire has high pressure.\n");
	} else if (t->pressure >= 19 && t->pressure < 21) {
		printf("Tire has low pressure.\n");
	} else {
		printf("Tire has abnormal pressure.\n");
	}
}

static void tire_temperature_status(void *data)
{
	tire_sensor *t = (tire_sensor *)data;
	if (t->temperature >= 0 && t->temperature <= 120) {
		printf("Tire has normal temperature.\n");
	} else {
		printf("Tire has abnormal temperature.\n");
	}
}

static void tire_wear_level_status(void *data)
{
	tire_sensor *t = (tire_sensor *)data;
	if (t->wear_level >= 0 && t->wear_level <= 20) {
		printf("Tire is new.\n");
	} else if (t->wear_level >= 20 && t->wear_level <= 40) {
		printf("Tire is slightly used.\n");
	} else if (t->wear_level >= 40 && t->wear_level <= 60) {
		printf("Tire is used. Consider a pit stop!\n");
	} else {
		printf("Tire is extremely used. Box this lap!\n");
	}
}

static void tire_performance_score(void *data)
{
	tire_sensor *t = (tire_sensor *)data;
	int score = 0;

	// Check pressure
	if (t->pressure >= 21 && t->pressure <= 26) {
		score += 4;
	} else if (t->pressure >= 19 && t->pressure < 21) {
		score += 2;
	} else if (t->pressure > 26 && t->pressure <= 28) {
		score += 3;
	} else {
		score -= 1;
	}

	// Check temperature
	if (t->temperature >= 80 && t->temperature <= 100) {
		score += 4;
	} else if (t->temperature >= 100 && t->temperature < 120) {
		score += 3;
	} else if (t->temperature > 60 && t->temperature <= 80) {
		score += 2;
	} else {
		score -= 1;
	}

	// Check wear level
	if (t->wear_level >= 0 && t->wear_level < 20) {
		score += 4;
	} else if (t->wear_level >= 20 && t->wear_level < 40) {
		score += 2;
	} else if (t->wear_level > 40 && t->wear_level < 60) {
		score -= 1;
	} else {
		score -= 2;
	}

	// Cap the score at 10
	if (score > 10) {
		score = 10;
	} else if (score < 1) {
		score = 1;
	}

	t->performace_score = score;
	printf("The tire performance score is: %d\n", score);
}

static void pmu_compute_power(void *data)
{
	power_management_unit *pmu = (power_management_unit *)data;
	float power = pmu->voltage * pmu->current;

	printf("Power output: %.2f kW.\n", power);
}

static void pmu_regenerate_energy(void *data)
{
	power_management_unit *pmu = (power_management_unit *)data;

	pmu->energy_storage += pmu->energy_regen;
	pmu->energy_regen = 0;

	// Cap the energy storage to 100%
	if (pmu->energy_storage > 100)
		pmu->energy_storage = 100;

	printf("Energy left in storage: %d\n", pmu->energy_storage);
}

static void pmu_get_energy_usage(void *data)
{
	power_management_unit *pmu = (power_management_unit *)data;
	float energy = pmu->power_consumption * pmu->current;

	printf("Energy usage: %.2f kW\n", energy);
}

static void pmu_is_battery_healthy(void *data)
{
	power_management_unit *pmu = (power_management_unit *)data;
	int ok = 0;

	if (pmu->voltage < 10 || pmu->voltage > 20) {
		printf("Battery voltage out of range: %.2fV\n", pmu->voltage);
		ok = 1;
	}

	if (pmu->current < -100 || pmu->current > 100) {
		printf("Battery current out of range: %.2fA\n", pmu->current);
		ok = 1;
	}

	if (pmu->power_consumption < 0 || pmu->power_consumption > 1000) {
		printf("Power consumption out of range: %.2f kW\n",
			   pmu->power_consumption);
		ok = 1;
	}

	if (pmu->energy_regen < 0 || pmu->energy_regen > 100) {
		printf("Energy regeneration out of range: %d%%\n", pmu->energy_regen);
		ok = 1;
	}

	if (pmu->energy_storage < 0 || pmu->energy_storage > 100) {
		printf("Energy storage out of range: %d%%\n", pmu->energy_storage);
		ok = 1;
	}

	if (ok == 1)
		return;

	printf("Battery working as expected!\n");
}

void get_operations(void (*operations[])(void *))
{
	operations[0] = tire_pressure_status;
	operations[1] = tire_temperature_status;
	operations[2] = tire_wear_level_status;
	operations[3] = tire_performance_score;
	operations[4] = pmu_compute_power;
	operations[5] = pmu_regenerate_energy;
	operations[6] = pmu_get_energy_usage;
	operations[7] = pmu_is_battery_healthy;
}

// ------- DO NOT MODIFY ------- //


//Functia verifica ce comanda trebuie executata.
int which_command(char *command)
{
	if (strcmp(command, "print") == 0) {
		return PRINT;
	} else if (strcmp(command, "analyze") == 0) {
		return ANALYZE;
	} else if (strcmp(command, "clear") == 0) {
		return CLEAR;
	} else if (strcmp(command, "exit") == 0) {
		return EXIT;
	}
	return 0;
}

//Verifica daca index-ul senzorului este valabil,
//Apoi afiseaza informatiile despre senzor.
void print_sensor(sensor *sensor_array, int index, int nr_sensors)
{
	//Verific daca index-ul senzorului este valabil
	if (index < nr_sensors && index >= 0) {
		//Verific daca este senzor 'TIRE'
		if (sensor_array[index].sensor_type == TIRE) {
			//Afisez informatiile despre senzorul 'TIRE'
			printf("Tire Sensor\n");
			printf("Pressure: %.2f\n", ((tire_sensor *)
				   (sensor_array[index].sensor_data))->pressure);
			printf("Temperature: %.2f\n", ((tire_sensor *)
				   (sensor_array[index].sensor_data))->temperature);
			printf("Wear Level: %d%%\n", ((tire_sensor *)
				   (sensor_array[index].sensor_data))->wear_level);
			if (((tire_sensor *)(sensor_array[index].sensor_data))
				->performace_score == 0)
				printf("Performance Score: Not Calculated\n");
			else
				printf("Performance Score: %d\n", ((tire_sensor *)
					   (sensor_array[index].sensor_data))->performace_score);
		//Verific daca este senzor 'PMU'
		} else if (sensor_array[index].sensor_type == PMU) {
			//Afisez informatiile despre senzorul 'PMU'
			printf("Power Management Unit\n");
			printf("Voltage: %.2f\n", ((power_management_unit *)
				   (sensor_array[index].sensor_data))->voltage);
			printf("Current: %.2f\n", ((power_management_unit *)
				   (sensor_array[index].sensor_data))->current);
			printf("Power Consumption: %.2f\n", ((power_management_unit *)
				   (sensor_array[index].sensor_data))->power_consumption);
			printf("Energy Regen: %d%%\n", ((power_management_unit *)
				   (sensor_array[index].sensor_data))->energy_regen);
			printf("Energy Storage: %d%%\n", ((power_management_unit *)
				   (sensor_array[index].sensor_data))->energy_storage);
		}
	} else {
		printf("Index not in range!\n");
	}
}

//Functie basic de comparatie.
int cmp_types(const void *a, const void *b)
{
	sensor x = *(sensor *)a;
	sensor y = *(sensor *)b;

	if (x.sensor_type < y.sensor_type)
		return 1;
	if (x.sensor_type > y.sensor_type)
		return -1;
	return 0;
}

//Parcurg toti senzorii, verific de ce tip sunt, apoi verific daca
//contin informatii eronate.
void clear_sensors(sensor *sensor_array, int *nr_sensors)
{
	//Parcurg vectorul de senzori
	for (int i = 0; i < *nr_sensors; i++) {
		//Verific daca este senzor 'TIRE'
		if (sensor_array[i].sensor_type == TIRE) {
			//Folosesc o functie care imi spune daca senzorul contine
			//informatii eronate.
			if (check_tire(sensor_array[i]) == 0) {
				//Daca senzorul contine informatii eronate, atunci il elimin
				//din vectorul de senzori si modific dimensiunea vectorului.
				sensor aux = sensor_array[i];
				for (int j = i; j < *nr_sensors - 1; j++) {
					sensor_array[j] = sensor_array[j + 1];
				}
				//eliberez memoria senzorului care contine informatii eronate.
				free(aux.operations_idxs);
				free(aux.sensor_data);
				(*nr_sensors)--;
				i--;
			}
		//Verific daca este senzor 'PMU'
		} else if (sensor_array[i].sensor_type == PMU) {
			if (check_pmu(sensor_array[i]) == 0) {
			//Folosesc o functie care imi spune daca senzorul contine
			//informatii eronate.
				sensor aux = sensor_array[i];
				//Daca senzorul contine informatii eronate, atunci il elimin
				//din vectorul de senzori si modific dimensiunea vectorului.
				for (int j = i; j < *nr_sensors - 1; j++) {
					sensor_array[j] = sensor_array[j + 1];
				}
				//eliberez memoria senzorului care contine informatii eronate.
				free(aux.operations_idxs);
				free(aux.sensor_data);
				(*nr_sensors)--;
				i--;
			}
		}
	}
}


//Functia aceasta verifica daca senzorul de tip 'TIRE' contine
//informatii eronate.
int check_tire(sensor sensor)
{
	int ok = 1;
	if (((tire_sensor *)(sensor.sensor_data))->pressure < 19 ||
		28 < ((tire_sensor *)(sensor.sensor_data))->pressure) {
		ok = 0;
		return ok;
	} else if (((tire_sensor *)(sensor.sensor_data))->temperature < 0 ||
			   120 < ((tire_sensor *)(sensor.sensor_data))->temperature) {
		ok = 0;
		return ok;
	} else if (((tire_sensor *)(sensor.sensor_data))->wear_level < 0 ||
			   100 < ((tire_sensor *)(sensor.sensor_data))->wear_level) {
		ok = 0;
		return ok;
	}
	return ok;
}

//Functia aceasta verifica daca senzorul de tip 'PMU' contine
//informatii eronate.
int check_pmu(sensor sensor)
{
	int ok = 1;
	if (((power_management_unit *)(sensor.sensor_data))
		->voltage < 10 || 20 < ((power_management_unit *)
		(sensor.sensor_data))->voltage) {
		ok = 0;
		return ok;
	} else if (((power_management_unit *)(sensor.sensor_data))
			   ->current < -100 || 100 < ((power_management_unit *)
			   (sensor.sensor_data))->current) {
		ok = 0;
		return ok;
	} else if (((power_management_unit *)(sensor.sensor_data))
			   ->power_consumption < 0 || 1000 < ((power_management_unit *)
			   (sensor.sensor_data))->power_consumption) {
		ok = 0;
		return ok;
	} else if (((power_management_unit *)(sensor.sensor_data))
			   ->energy_regen < 0 || 100 < ((power_management_unit *)
			   (sensor.sensor_data))->energy_regen) {
		ok = 0;
		return ok;
	} else if (((power_management_unit *)(sensor.sensor_data))
			   ->energy_storage < 0 || 100 < ((power_management_unit *)
			   (sensor.sensor_data))->energy_storage) {
		ok = 0;
		return ok;
	}
	return ok;
}

//Inainte de a incheia executia programului, eliberez memoria alocata dinamic.
void exit_sensor(sensor *sensor_array, int nr_sensors)
{
	for (int i = 0; i < nr_sensors; i++) {
		free(sensor_array[i].operations_idxs);
		free(sensor_array[i].sensor_data);
	}
}

//Creez un vector de functii, pentru a apela functia care este necesara
//in functie de indexul operatiei.
void analyze_sensor(sensor sensor)
{
	void (**operations)(void *);
	operations = malloc(8 * sizeof(void *));
	DIE(!operations, "operation malloc() failed!\n");
	get_operations(operations);
	for (int i = 0; i < sensor.nr_operations; i++) {
		void (*aux)(void *) = operations[sensor.operations_idxs[i]];
		aux(sensor.sensor_data);
	}
	//Eliberez memoria ocupata de vectorul de functii.
	free(operations);
}