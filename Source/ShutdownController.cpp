#include "pch.h"
#include "Global.h"
#include "ShutdownController.h"
#include <math.h>       /* pow */


using namespace UltraEngine;

int ShutdownController::SetShutdownTime(int years, int months, int weeks, int days, int hours, int minutes, int seconds) {
	int shutdownTime = 0;

	shutdownTime = (years * pow(3.154,7.0)) + (months * pow(2.628, 6.0)) + (weeks * 604800) + (days * 86400) + (hours * 3600) + (minutes * 60) + seconds; //We calculate the total time in seconds
	return shutdownTime; //We return the result to comunicate with the shutdown command
}

int ShutdownController::ShutDownSequence(int years, int months, int weeks, int days,  int hours, int minutes, int seconds, int shutdownMethod)
{
	int shutdownTime = ShutdownController::SetShutdownTime(years, months, weeks, days, hours, minutes, seconds);
	if (shutdownTime == 0) {
		Notify(L"ERROR! Your computer would shutdown immediately. To make sure this is not a mistake, we suggest you enter 1 second if you want your computer to shutdown immediately!", "ERROR - 0 SECOND SHUTDOWN", true);
		return -1;
	}

	switch (shutdownMethod)
	{
	case 0: //Shuting down
		if (Confirm(L"You are going to shutdown your computer in " + WString(shutdownTime) + L" second(s). Are you sure you want to proceed?", L"Shutdown confirmation", true)) {
			Print(L"Shuting down computer");
			String command = "shutdown /s /t " + String(shutdownTime);
			system(command.c_str());
			Notify(L"Make sure to save your work before leaving!", L"SAVE YOUR WORK", true);
		}
		else {
			Notify(L"Shutdown successfuly aborted!", L"Shutdown aborted!");
		}
		break;
	case 1: //Restarting
		if (Confirm(L"You are going to restart your computer in " + WString(shutdownTime) + L" second(s). Are you sure you want to proceed?", L"Restart confirmation", true)) {
			Print(L"Restarting computer");
			String command = "shutdown /r /t " + String(shutdownTime);
			system(command.c_str());
			Notify(L"Make sure to save your work before leaving!", L"SAVE YOUR WORK", true);
		}
		else {
			Notify(L"Restart successfuly aborted!", L"Restart aborted!");
		}
		break;
	//case 2: //Hibernation
	//	if (Confirm(L"You are going to put your computer under hibernation in " + WString(shutdownTime) + L" second(s). Are you sure you want to proceed?", L"Restart confirmation", true)) {
	//		Print(L"Computer hibernating");
	//		String command = "shutdown /h /t " + String(shutdownTime);
	//		system(command.c_str());
	//		Notify(L"Make sure to save your work before leaving!", L"SAVE YOUR WORK", true);
	//	}
	//	else {
	//		Notify(L"Hibernation successfuly aborted!", L"Hibernation aborted!");
	//	}
	//	break;
	case 2: //Forced shutdown
		if (Confirm(L"You are going to forced shutdown your computer in " + WString(shutdownTime) + L" second(s). Are you sure you want to proceed?", L"Shutdown confirmation", true)) {
			Print(L"Forced shutdown");
			String command = "shutdown /s /f /t " + String(shutdownTime);
			system(command.c_str());
			Notify(L"Make sure to save your work before leaving!", L"SAVE YOUR WORK", true);
		}
		else {
			Notify(L"Forced shutdown successfuly aborted!", L"Forced shutdown aborted!");
		}
		break;
	case 3: //Forced restart
		if (Confirm(L"You are going to forced restart your computer in " + WString(shutdownTime) + L" second(s). Are you sure you want to proceed?", L"Restart confirmation", true)) {
			Print(L"Forced restarting computer");
			String command = "shutdown /r /f /t " + String(shutdownTime);
			system(command.c_str());
			Notify(L"Make sure to save your work before leaving!", L"SAVE YOUR WORK", true);
		}
		else {
			Notify(L"Forced restart successfuly aborted!", L"Forced restart aborted!");
		}
		break;
	//case 5: //Forced hibernation
	//	if (Confirm(L"You are going to force your computer under hibernation in " + WString(shutdownTime) + L" second(s). Are you sure you want to proceed?", L"Restart confirmation", true)) {
	//		Print(L"Forced computer hibernating");
	//		String command = "shutdown /h /f /t " + String(shutdownTime);
	//		system(command.c_str());
	//		Notify(L"Make sure to save your work before leaving!", L"SAVE YOUR WORK", true);
	//	}
	//	else {
	//		Notify(L"Forced hibernation successfuly aborted!", L"Forced hibernation aborted!");
	//	}
	//	break;
	default: //Shuting down
		if (Confirm(L"You are going to shutdown your computer in " + WString(shutdownTime) + L" second(s). Are you sure you want to proceed?", L"Shutdown confirmation", true)) {
			Print(L"Shuting down computer");
			String command = "shutdown /s /t " + String(shutdownTime);
			system(command.c_str());
			Notify(L"Make sure to save your work before leaving!", L"SAVE YOUR WORK", true);
		}
		else {
			Notify(L"Shutdown successfuly aborted!", L"Shutdown aborted!");
		}
		break;
	}
	return shutdownTime;
}

void ShutdownController::TestForSequence() {
	String command = "shutdown /a";
	system(command.c_str());
	return;
}