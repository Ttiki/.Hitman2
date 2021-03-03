#pragma once
#ifndef SHUTDOWNCONTROLLER_H
#define SHUTDOWNCONTROLLER_H

using namespace UltraEngine;

class ShutdownController
{
public:
	int ShutdownController::ShutDownSequence(int years, int months, int weeks, int days, int hours, int minutes, int seconds, int shutdownMethod);
	void ShutdownController::TestForSequence();
protected:
	int ShutdownController::SetShutdownTime(int years, int months, int weeks, int days, int hours, int minutes, int seconds);
};


#endif