#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H
#include <stdlib.h>
#include <tchar.h>
#include <iostream>
#include "uvcview.h"	// contains #include <strsafe.h>
#include "hostController.h"

ULONG tmpTotalDevicesConnected = -1; // ���������� ������������ ���������

// �������� ���������� � ���������� ������������ ��������� (hubs and devices)
void getCountInfo(PDEVICE_HUB_LIST ask);

// ������� � ���� "fileName" ���������� � HostController`��
void printHostControllersInfo(const char* fileName);

// ������� � ���� "fileName" ���������� � Hubs`��
void printHubsInfo(const char* fileName);

// ������� � ���� "fileName" ���������� � ������������ ������� �����������
void printDevicesInfo(const char* fileName);


void getCountInfo(PDEVICE_HUB_LIST ask) {
	diplayInfo = 4;				// 0-all, 1-hostController, 2-Hubs, 3-devices 4-nothing
	setlocale(LC_ALL, "rus");	// ����� ������� ��������� ������

	// ������������� ������� gHubList � gDeviceList
	gHubList.DeviceInfo = INVALID_HANDLE_VALUE;
	InitializeListHead(&gHubList.ListHead);
	gDeviceList.DeviceInfo = INVALID_HANDLE_VALUE;
	InitializeListHead(&gDeviceList.ListHead);

	// ���������� � ���������� ������������ ���������
	EnumerateAllDevices(ask);

	// ������������ ��������
	ClearDeviceList(&gHubList);
	ClearDeviceList(&gDeviceList);
	ClearHCList();
	fout.close();
}


void printHostControllersInfo(const char* fileName) {
	diplayInfo = 1;				// 0-all, 1-hostController, 2-Hubs, 3-devices 4-nothing
	setlocale(LC_ALL, "rus");	// ����� ������� ��������� ������
	fout.open(fileName, ios_base::out | ios_base::trunc); // ������ ����������

	// ������������� ������� gHubList � gDeviceList
	gHubList.DeviceInfo = INVALID_HANDLE_VALUE;
	InitializeListHead(&gHubList.ListHead);
	gDeviceList.DeviceInfo = INVALID_HANDLE_VALUE;
	InitializeListHead(&gDeviceList.ListHead);

	// ������� ����������
	EnumerateHostControllers(NULL, &tmpTotalDevicesConnected);

	// ������������ ��������
	ClearDeviceList(&gHubList);
	ClearDeviceList(&gDeviceList);
	ClearHCList();
	fout.close();
}

void printHubsInfo(const char* fileName) {
	diplayInfo = 2;				// 0-all, 1-hostController, 2-Hubs, 3-devices 4-nothing
	setlocale(LC_ALL, "rus");	// ����� ������� ��������� ������
	fout.open(fileName, ios_base::out | ios_base::trunc); // ������ ����������

	// ������������� ������� gHubList � gDeviceList
	gHubList.DeviceInfo = INVALID_HANDLE_VALUE;
	InitializeListHead(&gHubList.ListHead);
	gDeviceList.DeviceInfo = INVALID_HANDLE_VALUE;
	InitializeListHead(&gDeviceList.ListHead);

	// ������� ����������
	EnumerateHostControllers(NULL, &tmpTotalDevicesConnected);

	// ������������ ��������
	ClearDeviceList(&gHubList);
	ClearDeviceList(&gDeviceList);
	ClearHCList();
	fout.close(); 
}

void printDevicesInfo(const char* fileName) {
	diplayInfo = 3;				// 0-all, 1-hostController, 2-Hubs, 3-devices 4-nothing
	setlocale(LC_ALL, "rus");	// ����� ������� ��������� ������
	fout.open(fileName, ios_base::out | ios_base::trunc); // ������ ����������

	// ������������� ������� gHubList � gDeviceList
	gHubList.DeviceInfo = INVALID_HANDLE_VALUE;
	InitializeListHead(&gHubList.ListHead);
	gDeviceList.DeviceInfo = INVALID_HANDLE_VALUE;
	InitializeListHead(&gDeviceList.ListHead);

	// ������� ����������
	EnumerateHostControllers(NULL, &tmpTotalDevicesConnected);

	// ������������ ��������
	ClearDeviceList(&gHubList);
	ClearDeviceList(&gDeviceList);
	ClearHCList();
	fout.close();
}
#endif