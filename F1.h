#pragma once
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_STRING_LENGTH 256

#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		   \
			perror(call_description);			\
			exit(errno);				         \
		}							\
	} while (0)

#define PRINT 2
#define ANALYZE 3
#define CLEAR 4
#define EXIT 5

enum sensor_type {
	TIRE,
	PMU
};

typedef struct {
	enum sensor_type sensor_type;
	void *sensor_data;
	int nr_operations;
	int *operations_idxs;
} sensor;

typedef struct {
	float voltage;
	float current;
	float power_consumption;
	int energy_regen;
	int energy_storage;
} power_management_unit;

typedef struct {
	float pressure;
	float temperature;
	int wear_level;
	int performace_score;
} tire_sensor;

int which_command(char *command);
void print_sensor(sensor *sensor_array, int index, int nr_sensors);
int cmp_types(const void *a, const void *b);
void clear_sensors(sensor *sensor_array, int *nr_sensors);
int check_tire(sensor sensor);
int check_pmu(sensor sensor);
void exit_sensor(sensor *sensor_array, int nr_sensors);
static void tire_pressure_status(void *data);
static void tire_temperature_status(void *data);
static void tire_wear_level_status(void *data);
static void tire_performance_score(void *data);
static void pmu_compute_power(void *data);
static void pmu_regenerate_energy(void *data);
static void pmu_get_energy_usage(void *data);
static void pmu_is_battery_healthy(void *data);
void get_operations(void (*operations[])(void *));
void analyze_sensor(sensor sensor);