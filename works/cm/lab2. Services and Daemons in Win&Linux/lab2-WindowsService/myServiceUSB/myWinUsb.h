#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H
#include <stdlib.h>
#include <tchar.h>
#include <iostream>
#include "uvcview.h"	// contains #include <strsafe.h>
#include "hostController.h"

ULONG tmpTotalDevicesConnected = -1; // количество подключенных устройств

// получить информацию о количестве подключенных устройств (hubs and devices)
void getCountInfo(PDEVICE_HUB_LIST ask);

// вывести в файл "fileName" информацию о HostController`ах
void printHostControllersInfo(const char* fileName);

// вывести в файл "fileName" информацию о Hubs`ах
void printHubsInfo(const char* fileName);

// вывести в файл "fileName" информацию о подключенных внешних устройствах
void printDevicesInfo(const char* fileName);


void getCountInfo(PDEVICE_HUB_LIST ask) {
	diplayInfo = 4;				// 0-all, 1-hostController, 2-Hubs, 3-devices 4-nothing
	setlocale(LC_ALL, "rus");	// вызов функции настройки локали

	// инициализация списков gHubList и gDeviceList
	gHubList.DeviceInfo = INVALID_HANDLE_VALUE;
	InitializeListHead(&gHubList.ListHead);
	gDeviceList.DeviceInfo = INVALID_HANDLE_VALUE;
	InitializeListHead(&gDeviceList.ListHead);

	// информация о количестве подключенных устройств
	EnumerateAllDevices(ask);

	// освобождение ресурсов
	ClearDeviceList(&gHubList);
	ClearDeviceList(&gDeviceList);
	ClearHCList();
	fout.close();
}


void printHostControllersInfo(const char* fileName) {
	diplayInfo = 1;				// 0-all, 1-hostController, 2-Hubs, 3-devices 4-nothing
	setlocale(LC_ALL, "rus");	// вызов функции настройки локали
	fout.open(fileName, ios_base::out | ios_base::trunc); // печать результата

	// инициализация списков gHubList и gDeviceList
	gHubList.DeviceInfo = INVALID_HANDLE_VALUE;
	InitializeListHead(&gHubList.ListHead);
	gDeviceList.DeviceInfo = INVALID_HANDLE_VALUE;
	InitializeListHead(&gDeviceList.ListHead);

	// вывести информацию
	EnumerateHostControllers(NULL, &tmpTotalDevicesConnected);

	// освобождение ресурсов
	ClearDeviceList(&gHubList);
	ClearDeviceList(&gDeviceList);
	ClearHCList();
	fout.close();
}

void printHubsInfo(const char* fileName) {
	diplayInfo = 2;				// 0-all, 1-hostController, 2-Hubs, 3-devices 4-nothing
	setlocale(LC_ALL, "rus");	// вызов функции настройки локали
	fout.open(fileName, ios_base::out | ios_base::trunc); // печать результата

	// инициализация списков gHubList и gDeviceList
	gHubList.DeviceInfo = INVALID_HANDLE_VALUE;
	InitializeListHead(&gHubList.ListHead);
	gDeviceList.DeviceInfo = INVALID_HANDLE_VALUE;
	InitializeListHead(&gDeviceList.ListHead);

	// вывести информацию
	EnumerateHostControllers(NULL, &tmpTotalDevicesConnected);

	// освобождение ресурсов
	ClearDeviceList(&gHubList);
	ClearDeviceList(&gDeviceList);
	ClearHCList();
	fout.close(); 
}

void printDevicesInfo(const char* fileName) {
	diplayInfo = 3;				// 0-all, 1-hostController, 2-Hubs, 3-devices 4-nothing
	setlocale(LC_ALL, "rus");	// вызов функции настройки локали
	fout.open(fileName, ios_base::out | ios_base::trunc); // печать результата

	// инициализация списков gHubList и gDeviceList
	gHubList.DeviceInfo = INVALID_HANDLE_VALUE;
	InitializeListHead(&gHubList.ListHead);
	gDeviceList.DeviceInfo = INVALID_HANDLE_VALUE;
	InitializeListHead(&gDeviceList.ListHead);

	// вывести информацию
	EnumerateHostControllers(NULL, &tmpTotalDevicesConnected);

	// освобождение ресурсов
	ClearDeviceList(&gHubList);
	ClearDeviceList(&gDeviceList);
	ClearHCList();
	fout.close();
}
#endif