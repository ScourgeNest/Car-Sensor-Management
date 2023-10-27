#include "F1.h"

int main(int argc, char const *argv[])
{
	char *command;
	int index;
	command = malloc(MAX_STRING_LENGTH);
	DIE(!command, "command malloc() failed!\n");
	//Verific daca este trimis un fisier ca parametru.
	if (argc < 2) {
		printf("Trimite un fisier ca parametru!\n");
		return -1;
	}
	//Deschud fisierul pentru a lua informatiile despre senzori.
	FILE *in = fopen(argv[1], "r");

	int nr_sensors;

	//citesc cati senzori contine fisierul
	fread(&nr_sensors, sizeof(int), 1, in);
	//Creez un vector de senzori.
	sensor *sensor_array;
	sensor_array = malloc(nr_sensors * sizeof(sensor));
	DIE(!sensor_array, "sensor_array malloc() failed!\n");
	//Pentru fiecare senzor in parte, ii umplu campurile cu informatiile
	//din fisier.
	for (int i = 0; i < nr_sensors; i++) {
		fread(&sensor_array[i].sensor_type, sizeof(enum sensor_type), 1, in);
		if (sensor_array[i].sensor_type == TIRE) {
			sensor_array[i].sensor_data = malloc(sizeof(tire_sensor));
			DIE(!sensor_array[i].sensor_data,
				"sensor_data malloc() failed!\n");

			fread(&((tire_sensor *)(sensor_array[i].sensor_data))
			->pressure, sizeof(float), 1, in);

			fread(&((tire_sensor *)(sensor_array[i].sensor_data))
			->temperature, sizeof(float), 1, in);

			fread(&((tire_sensor *)(sensor_array[i].sensor_data))
			->wear_level, sizeof(int), 1, in);

			fread(&((tire_sensor *)(sensor_array[i].sensor_data))
			->performace_score, sizeof(int), 1, in);
		} else if (sensor_array[i].sensor_type == PMU) {
			sensor_array[i].sensor_data =
			malloc(sizeof(power_management_unit));
			DIE(!sensor_array[i].sensor_data,
				"sensor_data malloc() failed!\n");

			fread(&((power_management_unit *)(sensor_array[i].sensor_data))
				  ->voltage, sizeof(float), 1, in);

			fread(&((power_management_unit *)(sensor_array[i].sensor_data))
				  ->current, sizeof(float), 1, in);

			fread(&((power_management_unit *)(sensor_array[i].sensor_data))
				  ->power_consumption, sizeof(float), 1, in);

			fread(&((power_management_unit *)(sensor_array[i].sensor_data))
				  ->energy_regen, sizeof(int), 1, in);

			fread(&((power_management_unit *)(sensor_array[i].sensor_data))
			      ->energy_storage, sizeof(int), 1, in);
		}
		fread(&sensor_array[i].nr_operations, sizeof(int), 1, in);
		sensor_array[i].operations_idxs = malloc(sensor_array[i].nr_operations
		* sizeof(int));
		for (int j = 0; j < sensor_array[i].nr_operations; j++) {
			fread(&sensor_array[i].operations_idxs[j], sizeof(int), 1, in);
		}

	}
	//Inchid fisierul.
	fclose(in);
	//sortez vectorul.
	qsort(sensor_array, nr_sensors, sizeof(sensor), cmp_types);
	//Citesc comenzi pana cand este intalnita comanda 'exit'.
	while (1) {
		scanf("%s", command);
		//Verific sa vad ce comanda este introdusa pentru a o executa.
		switch (which_command(command)) {

		case PRINT:
			//Execut comanda de print.
			scanf("%d", &index);
			print_sensor(sensor_array, index, nr_sensors);
			break;

		case ANALYZE:
			//Execut comanda de analyze.
			scanf("%d", &index);
			analyze_sensor(sensor_array[index]);
			break;

		case CLEAR:
			//Execut comanda de clear.
			clear_sensors(sensor_array, &nr_sensors);
			//Dupa ce am sters senzorii cu informatii eronate, realloc vectorul
			sensor_array = realloc(sensor_array, nr_sensors * sizeof(sensor));
			qsort(sensor_array, nr_sensors, sizeof(sensor), cmp_types);
			break;
		case EXIT:
			//Execut comanda de exit.
			exit_sensor(sensor_array, nr_sensors);
			free(sensor_array);
			free(command);
			exit(0);
			break;
		}
	}
	return 0;
}
