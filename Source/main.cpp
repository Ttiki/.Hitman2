#include "pch.h"
#include "ShutdownController.h"
#include "Global.h"
#include <windows.h>
#include <math.h>
//#include <time.h>

using namespace UltraEngine;
 
bool RescaleUI(const Event& event, shared_ptr<Object> extra)
{
	auto ui = dynamic_pointer_cast<Interface>(extra);
	ui->SetScale(float(event.data) / 100.0f);
	auto window = dynamic_pointer_cast<Window>(event.source);
	window->SetShape(event.position.x, event.position.y, event.size.x, event.size.y);
	return true;
}

void LoadDefaultTheme(std::shared_ptr <Interface> ui, std::shared_ptr <Widget> menu_colorDark, std::shared_ptr <Widget> menu_colorLight, std::shared_ptr <Widget> menu_colorMilitary, std::shared_ptr <Widget> menu_colorMatrix) {
	//Charge default theme
	char* defaultfile;
	auto savefile = ReadFile("saves.txt");
	if (savefile == NULL)
	{
		Print("Failed to read file.");
		
		auto savefile = WriteFile("saves.txt");
		if (savefile == NULL)
		{
			Print("Failed to write file.");
			Notify(L"We could not found saves.txt nor create it thus disabling us from saving any information. Please, make sure you have the correct right to use this software!", L"ERROR 0x11", true);
			return ;
		}

		savefile->WriteString("0");
		savefile->Close();
		
		return;
	}

	switch (Int(savefile->ReadString()))
	{
	case 0: //Dark theme is default
		menu_colorDark->SetState(true);
		menu_colorLight->SetState(false);
		menu_colorMilitary->SetState(false);
		menu_colorMatrix->SetState(false);

		defaultfile = "Themes/dark.json";
		ui->LoadColorScheme(defaultfile);
		break;
	case 1: //Light theme is default
		menu_colorDark->SetState(false);
		menu_colorLight->SetState(true);
		menu_colorMilitary->SetState(false);
		menu_colorMatrix->SetState(false);

		defaultfile = "Themes/light.json";
		ui->LoadColorScheme(defaultfile);
		break;
	case 2: //Military theme is default
		menu_colorDark->SetState(false);
		menu_colorLight->SetState(false);
		menu_colorMilitary->SetState(true);
		menu_colorMatrix->SetState(false);

		defaultfile = "Themes/military.json";
		ui->LoadColorScheme(defaultfile);
		break;
	case 3: //Terminal theme is default
		menu_colorDark->SetState(false);
		menu_colorLight->SetState(false);
		menu_colorMilitary->SetState(false);
		menu_colorMatrix->SetState(true);

		defaultfile = "Themes/johnwick.json";
		ui->LoadColorScheme(defaultfile);
		break;
	default: //Dark theme is default
		menu_colorDark->SetState(true);
		menu_colorLight->SetState(false);
		menu_colorMilitary->SetState(false);
		menu_colorMatrix->SetState(false);

		defaultfile = "Themes/dark.json";
		ui->LoadColorScheme(defaultfile);
		break;
	}

	savefile->Close();
}

int main(int argc, const char* argv[])
{
	auto displays = ListDisplays();
	displays = ListDisplays();
	displays = ListDisplays();

	auto win = CreateWindow(".HitMan2", 0, 0, 900 /** displays[0]->scale*/, 1500 /** displays[0]->scale*/, displays[0], WINDOW_HIDDEN | WINDOW_CENTER | WINDOW_TITLEBAR | WINDOW_DOUBLEBUFFERED);
	win->SetMinSize(900, 1500);

	auto ui = CreateInterface(win);

	auto mainpanel = ui->root;

	auto mainmenu = CreateMenu("", mainpanel);
	int menuheight = mainmenu->GetSize().y;

	/*======================================================================*/
	//!WRITE MENU CODE UNDER THIS LINE
	/*======================================================================*/

	auto menu_file = CreateMenu("File", mainmenu);
	auto menu_reset = CreateMenu("Reset", menu_file);
	CreateMenu("", menu_file);
	auto menu_quit = CreateMenu("Quit", menu_file);


	auto menu_edit = CreateMenu("Edit", mainmenu);
	auto menu_color_scheme = CreateMenu("Change theme", menu_edit);
	auto menu_colorDark = CreateMenu("Dark", menu_color_scheme);
	auto menu_colorLight = CreateMenu("Light", menu_color_scheme);
	auto menu_colorMilitary = CreateMenu("Military", menu_color_scheme);
	auto menu_colorMatrix = CreateMenu("Matrix", menu_color_scheme);
	//Load default theme
	LoadDefaultTheme(ui, menu_colorDark, menu_colorLight, menu_colorMilitary, menu_colorMatrix);


	auto menu_help = CreateMenu("Help", mainmenu);
	CreateMenu("Help Contents", menu_help);
	auto menu_about = CreateMenu("About", menu_help);

	/*======================================================================*/
	//!WRITE INTERFACE CODE UNDER THIS LINE!
	/*======================================================================*/

	int border = 8;

	auto subpanel1 = CreatePanel(0, menuheight, mainpanel->ClientSize().x, mainpanel->ClientSize().y, mainpanel);
	subpanel1->SetLayout(1, 1, 1, 1);
	//subpanel1->Hide();

	//Title panel
	int y = 0;
	auto titlepanel = CreatePanel(border*2,border, (subpanel1->ClientSize().x - border*4), 96, subpanel1, PANEL_BORDER);
	titlepanel->SetLayout(1, 1, 1, 0);
	auto title = CreateLabel(L".HITMAN²", 0, 0, titlepanel->ClientSize().x, 96, titlepanel, LABEL_CENTER | LABEL_MIDDLE);
	title->SetFontScale(4);
	title->SetLayout(1, 1, 1, 1);

	//Options panel
	y += 128;
	int sliderypos = 24;
	auto optionpanel = CreatePanel(border, y, subpanel1->ClientSize().x - border*2, 375, subpanel1, PANEL_BORDER);
	//optionpanel->SetText("Options:");
	optionpanel->SetLayout(1, 1, 0, 0);
	y = 16;
	//Seconds panel
	auto seconds_panel = CreatePanel(0, y, optionpanel->ClientSize().x-16, 64, optionpanel, PANEL_GROUP);
	seconds_panel->SetText("Seconds");
	seconds_panel->SetLayout(1,1,1,1);
	CreateLabel("0-60", 0, 0, 64, 64, seconds_panel);
	auto label_seconds = CreateLabel("0", seconds_panel->ClientSize().x-16, 0, 64, 64, seconds_panel);
	label_seconds->SetLayout(0, 1, 0, 0);
	y += 4;
	auto slider_seconds = CreateSlider(0, sliderypos, seconds_panel->ClientSize().x-12, 32, seconds_panel, SLIDER_HORIZONTAL);
	slider_seconds->SetRange(0, MAXSEC);
	slider_seconds->SetLayout(1,1,1,1);
	label_seconds->SetText(slider_seconds->GetValue());

	y += 72;
	//Minutes panel
	auto minutes_panel = CreatePanel(0, y, optionpanel->ClientSize().x - 16, 64, optionpanel, PANEL_GROUP);
	minutes_panel->SetText("Minutes");
	minutes_panel->SetLayout(1, 1, 1, 1);
	CreateLabel("0-60", 0, 0, 64, 64, minutes_panel);
	auto label_minutes = CreateLabel("0", minutes_panel->ClientSize().x - 16, 0, 64, 64, minutes_panel);
	label_minutes->SetLayout(0, 1, 0, 0);
	y += 4;
	auto slider_minutes = CreateSlider(0, sliderypos, minutes_panel->ClientSize().x-10, 32, minutes_panel, SLIDER_HORIZONTAL);
	slider_minutes->SetRange(0, MAXMIN);
	slider_minutes->SetLayout(1, 1, 1, 1);
	label_minutes->SetText(slider_minutes->GetValue());

	y += 72;
	//Hours panel
	auto hours_panel = CreatePanel(0, y, optionpanel->ClientSize().x - 16, 64, optionpanel, PANEL_GROUP);
	hours_panel->SetText("Hours");
	hours_panel->SetLayout(1, 1, 1, 1);
	CreateLabel("0-24", 0, 0, 64, 64, hours_panel);
	auto label_hours = CreateLabel("0", hours_panel->ClientSize().x - 16, 0, 64, 64, hours_panel);
	label_hours->SetLayout(0, 1, 0, 0);
	y += 4;
	auto slider_hour = CreateSlider(0, sliderypos, hours_panel->ClientSize().x-10, 32, hours_panel, SLIDER_HORIZONTAL);
	slider_hour->SetRange(0, MAXHOUR);
	slider_hour->SetLayout(1, 1, 1, 1);
	label_hours->SetText(slider_hour->GetValue());

	y += 72;
	//Days panel
	auto days_panel = CreatePanel(0, y, optionpanel->ClientSize().x - 16, 64, optionpanel, PANEL_GROUP);
	days_panel->SetText("Days");
	days_panel->SetLayout(1, 1, 1, 1);
	CreateLabel("0-30", 0, 0, 64, 64, days_panel);
	auto label_days = CreateLabel("0", days_panel->ClientSize().x - 16, 0, 64, 64, days_panel);
	label_days->SetLayout(0, 1, 0, 0);
	y += 4;
	auto slider_days = CreateSlider(0, sliderypos, days_panel->ClientSize().x, 32, days_panel, SLIDER_HORIZONTAL);
	slider_days->SetRange(0, MAXDAYS);
	slider_days->SetLayout(1, 1, 1, 1);
	label_days->SetText(slider_days->GetValue());

	y += 72;
	//Weeks panel
	auto weeks_panel = CreatePanel(0, y, optionpanel->ClientSize().x - 16, 64, optionpanel, PANEL_GROUP);
	weeks_panel->SetText("Weeks");
	weeks_panel->SetLayout(1, 1, 1, 1);
	CreateLabel("0-4", 0, 0, 64, 64, weeks_panel);
	auto label_weeks = CreateLabel("0", weeks_panel->ClientSize().x - 16, 0, 64, 64, weeks_panel);
	label_weeks->SetLayout(0, 1, 0, 0);
	y += 4;
	auto slider_weeks = CreateSlider(0, sliderypos, weeks_panel->ClientSize().x, 32, weeks_panel, SLIDER_HORIZONTAL);
	slider_weeks->SetRange(0, MAXWEEKS);
	slider_weeks->SetLayout(1, 1, 1, 1);
	label_weeks->SetText(slider_weeks->GetValue());

	y += 72;
	//Months panel
	auto months_panel = CreatePanel(0, y, optionpanel->ClientSize().x - 16, 64, optionpanel, PANEL_GROUP);
	months_panel->SetText("Months");
	months_panel->SetLayout(1, 1, 1, 1);
	CreateLabel("0-30", 0, 0, 64, 64, months_panel);
	auto label_months = CreateLabel("0", months_panel->ClientSize().x - 16, 0, 64, 64, months_panel);
	label_months->SetLayout(0, 1, 0, 0);
	y += 4;
	auto slider_months = CreateSlider(0, sliderypos, months_panel->ClientSize().x, 32, months_panel, SLIDER_HORIZONTAL | SLIDER_TRACKBAR);
	slider_months->SetRange(0, MAXMONTHS);
	slider_months->SetLayout(1, 1, 1, 1);
	label_months->SetText(slider_months->GetValue());

	y += 72;
	//Years panel
	auto years_panel = CreatePanel(0, y, optionpanel->ClientSize().x - 16, 64, optionpanel, PANEL_GROUP);
	years_panel->SetText("Years");
	years_panel->SetLayout(1, 1, 1, 1);
	CreateLabel("0-28", 0, 0, 64, 64, years_panel);
	auto label_years = CreateLabel("0", years_panel->ClientSize().x - 16, 0, 64, 64, years_panel);
	label_years->SetLayout(0, 1, 0, 0);
	y += 4;
	auto slider_years = CreateSlider(0, sliderypos, years_panel->ClientSize().x, 32, years_panel, SLIDER_HORIZONTAL);
	slider_years->SetRange(0, MAXYEARS);
	slider_years->SetLayout(1, 1, 1, 1);
	label_years->SetText(slider_years->GetValue());
	
	
	auto methodspanel = CreatePanel(0, optionpanel->ClientSize().y-32, optionpanel->ClientSize().x, 32+border/4, optionpanel, PANEL_BORDER);
	methodspanel->SetLayout(1, 1, 0, 1);
	CreateLabel("Shutdown methods", border, border/2, 112-border, 24, methodspanel, LABEL_MIDDLE);
	WString availableMethods[4] = {
		L"Shutdown computer",
		L"Restart computer",
		//L"Hibernation",
		L"Forced shutdown computer",
		L"Forced restart computer",
		//L"Forced hibernation",
	};
	auto shutdownmethods = CreateComboBox(112+border, border/2, methodspanel->ClientSize().x-224, 24, methodspanel);
	for each (WString methods in availableMethods)
	{
		shutdownmethods->AddItem(methods);
	}
	shutdownmethods->SelectItem(0);
	shutdownmethods->SetLayout(1,1,0,0);

	auto trackbarSlider = CreateSlider(optionpanel->ClientSize().x-16, 0, 16, optionpanel->ClientSize().y-32, optionpanel, SLIDER_SCROLLBAR | SLIDER_VERTICAL);
	trackbarSlider->SetRange(0, 6);
	trackbarSlider->SetLayout(0,1,1,1);

	//Bottom panel
	auto bottompanel = CreatePanel(0, subpanel1->ClientSize().y - 128, mainpanel->ClientSize().x, 128, mainpanel, PANEL_GROUP);
	bottompanel->SetText("Launch!");
	bottompanel->SetLayout(1,1,0,1);
	auto launchbutton = CreateButton("LAUNCH!", border * 2, border, 96, 56, bottompanel, BUTTON_OK);
	auto abortbutton = CreateButton("ABORT!", border * 3 + 96 , border, 96, 56, bottompanel, BUTTON_CANCEL);
	auto label_panel = CreatePanel(bottompanel->ClientSize().x-200, 0, bottompanel->ClientSize().x/3-90, bottompanel->ClientSize().y-7, bottompanel, PANEL_GROUP);
	label_panel->SetText("You computer will shutdown in:");
	label_panel->SetLayout(0,1,0,0);
	int bl_y = 0;
	auto bottom_secondslabel = CreateLabel("0 second(s)", 0, bl_y, label_panel->ClientSize().x/2, 24, label_panel);
	bl_y += 15;
	auto bottom_minuteslabel = CreateLabel("0 minute(s)", 0, bl_y, label_panel->ClientSize().x/2, 24, label_panel);
	bl_y += 15;
	auto bottom_hourlabel = CreateLabel("0 hour(s)", 0, bl_y, label_panel->ClientSize().x/2, 24, label_panel);
	bl_y += 15;
	auto bottom_dayslabel = CreateLabel("0 day(s)", 0, bl_y, label_panel->ClientSize().x/2, 24, label_panel);
	bl_y = 0;
	auto bottom_weekslabel = CreateLabel("0 week(s)", label_panel->ClientSize().x / 2, bl_y, label_panel->ClientSize().x/2, 24, label_panel);
	bl_y += 15;
	auto bottom_monthslabel = CreateLabel("0 month(s)", label_panel->ClientSize().x / 2, bl_y, label_panel->ClientSize().x/2, 24, label_panel);
	bl_y += 15;
	auto bottom_yearslabel = CreateLabel("0 year(s)", label_panel->ClientSize().x / 2, bl_y, label_panel->ClientSize().x/2, 24, label_panel);

	auto progbar = CreateProgressBar(border, bottompanel->ClientSize().y-4, bottompanel->ClientSize().x - border*2, 24, bottompanel);
	progbar->SetLayout(1, 1, 1, 0);


	ui->SetScale(displays[0]->scale);
	ListenEvent(EVENT_WINDOWDPICHANGED, win, RescaleUI, ui);

	win->Show();
	win->Activate();

	ShutdownController shutdown;

	shared_ptr<TreeViewNode> dragnode, destnode;
	

	//Process Variable
	int shutdownTime = 0, hz = 0;
	float progress = 0;
	auto progressTimer = CreateTimer(1000);
	while (true)
	{
		const Event event = WaitEvent();

		switch (event.id)
		{
		case EVENT_WIDGETDROP:

			break;

		case EVENT_WIDGETSELECT:
	
			break;
		case EVENT_WIDGETACTION:
			if (event.source == trackbarSlider) { //Slider for time panels
				y = 24 - 64 * trackbarSlider->GetValue();
				seconds_panel->position.y = y;
				y = (24 + 150) - 64 * trackbarSlider->GetValue();
				minutes_panel->position.y = y;
				y = (24 + 2 * 150) - 64 * trackbarSlider->GetValue();
				hours_panel->position.y = y;
				y = (24 + 3 * 150) - 64 * trackbarSlider->GetValue();
				days_panel->position.y = y;
				y = (24 + 4 * 150) - 64 * trackbarSlider->GetValue();
				weeks_panel->position.y = y;
				y = (24 + 5 * 150) - 64 * trackbarSlider->GetValue();
				months_panel->position.y = y;
				y = (24 + 6 * 150) - 64 * trackbarSlider->GetValue();
				years_panel->position.y = y;
				optionpanel->Redraw();
			}

			//Lainch & abort events
			if (event.source == launchbutton) { //Launch events
				shutdownTime = shutdown.ShutDownSequence(slider_years->GetValue(), slider_months->GetValue(), slider_weeks->GetValue(), slider_days->GetValue(), slider_hour->GetValue(), slider_minutes->GetValue(), slider_seconds->GetValue(), shutdownmethods->GetSelectedItem());
			}
			if (event.source == abortbutton) { //Abort button
				shutdown.TestForSequence();
			}

			//Change the text of hours, minutes & seconds with sliders values
			if (event.source == slider_years)
			{
				label_years->SetText(slider_years->GetValue());
				bottom_yearslabel->SetText(WString(slider_years->GetValue())+L" year(s)");
			}
			if (event.source == slider_months)
			{
				label_months->SetText(slider_months->GetValue());
				bottom_monthslabel->SetText(WString(slider_months->GetValue())+L" month(s)");
			}
			if (event.source == slider_weeks)
			{
				label_weeks->SetText(slider_weeks->GetValue());
				bottom_weekslabel->SetText(WString(slider_weeks->GetValue())+L" week(s)");
			}
			if (event.source == slider_days)
			{
				label_days->SetText(slider_days->GetValue());
				bottom_dayslabel->SetText(WString(slider_days->GetValue())+L" day(s)");
			}
			if (event.source == slider_hour)
			{
				label_hours->SetText(slider_hour->GetValue());
				bottom_hourlabel->SetText(WString(slider_hour->GetValue())+L" hour(s)");
			}
			if (event.source == slider_minutes)
			{
				label_minutes->SetText(slider_minutes->GetValue());
				bottom_minuteslabel->SetText(WString(slider_minutes->GetValue())+L" minute(s)");
			}
			if (event.source == slider_seconds)
			{
				label_seconds->SetText(slider_seconds->GetValue());
				bottom_secondslabel->SetText(WString(slider_seconds->GetValue())+L" second(s)");
			}


			//Color schemes changes
			if (event.source == menu_colorDark)
			{
				menu_colorDark->SetState(true);
				menu_colorLight->SetState(false);
				menu_colorMilitary->SetState(false);
				menu_colorMatrix->SetState(false);

				auto file = "Themes/dark.json";
				ui->LoadColorScheme(file);
				auto savefile = WriteFile("saves.txt");
				if (savefile == NULL)
				{
					Print("Failed to write file.");
					return 0;
				}

				savefile->WriteString("0");
				savefile->Close();
			}
			if (event.source == menu_colorLight)
			{
				menu_colorDark->SetState(false);
				menu_colorLight->SetState(true);
				menu_colorMilitary->SetState(false);
				menu_colorMatrix->SetState(false);

				auto file = "Themes/light.json";
				ui->LoadColorScheme(file);
				auto savefile = WriteFile("saves.txt");
				if (savefile == NULL)
				{
					Print("Failed to write file.");
					return 0;
				}

				savefile->WriteString("1");
				savefile->Close();
			}
			if (event.source == menu_colorMilitary)
			{
				menu_colorDark->SetState(false);
				menu_colorLight->SetState(false);
				menu_colorMilitary->SetState(true);
				menu_colorMatrix->SetState(false);

				auto file = "Themes/military.json";
				ui->LoadColorScheme(file);
				auto savefile = WriteFile("saves.txt");
				if (savefile == NULL)
				{
					Print("Failed to write file.");
					return 0;
				}

				savefile->WriteString("2");
				savefile->Close();
			}
			if (event.source == menu_colorMatrix)
			{
				menu_colorDark->SetState(false);
				menu_colorLight->SetState(false);
				menu_colorMilitary->SetState(false);
				menu_colorMatrix->SetState(true);

				auto file = "Themes/johnwick.json";
				ui->LoadColorScheme(file);

				auto savefile = WriteFile("saves.txt");
				if (savefile == NULL)
				{
					Print("Failed to write file.");
					return 0;
				}

				savefile->WriteString("3");
				savefile->Close();
			}

			//Help section
			char* linkChar;
			if (event.source == menu_about)
			{
				linkChar = "https://github.com/Ttiki/.Hitman2/wiki/.Hitman(%C2%B2)";
				ShellExecute(NULL, "open", linkChar, NULL, NULL, SW_SHOWNORMAL);
			}
			if (event.source == menu_help) {
				linkChar = "https://github.com/Ttiki/.Hitman2/wiki";
				ShellExecute(NULL, "open", linkChar, NULL, NULL, SW_SHOWNORMAL);
			}


			//File section
			if (event.source == menu_reset)
			{
				if (Confirm(L"By reseting the application you cancel any shut down in ! Are you sure you want to reload the application", "Confirm reset", true)) {
					slider_seconds->SetValue(0);
					slider_minutes->SetValue(0);
					slider_hour->SetValue(0);
					slider_days->SetValue(0);
					slider_weeks->SetValue(0);
					slider_months->SetValue(0);
					slider_years->SetValue(0);
					shutdown.TestForSequence();
				}
			}
			if (event.source == menu_quit)
			{
				ui->Save("ui.json");
				return 0;
			}
			break;
		case EVENT_WINDOWCLOSE:
			ui->Save("ui.json");
			return 0;
			break;
		}
		if (shutdownTime != NULL) {
			hz += progressTimer->frequency;

			progress = (100 / shutdownTime) * hz;
			progbar->SetProgress(progress);
			Print("Progress = " + String(progress));
			Print("Shutdowntime = " + String(shutdownTime/100));
			Print("Frequency = " + String(progressTimer->frequency));

		}
	}
	return 0;
}