/**
 * ./gtfs-editor
 */
#include <cstdlib>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE /* for tm_gmtoff and tm_zone */
#endif
#include <time.h>

#include <argtable2.h>
#include "utilstring.h"
#include "gtfs-builder.h"
#include "gtfs-editor-config.h"
#include "imgui_impl_glfw.h"

const char* progname = "gtfs-editor";

#define DEF_GTFS_FOLDER			"gtfs"
#define UNICODE_RANGE_NAMES		"latin, korean, japanese, chinese, cyrillic, thai"
#define DEF_UNICODE_RANGE_NAME	"cyrillic"
#define DEF_FONT_NAME			"Roboto-Medium.ttf"
#define DEF_FONT_SIZE			13
#define DEF_FONT_SIZE_S			"13"

/**
 * Parse command line into struct ClientConfig
 * Return 0- success
 *        1- show help and exit, or command syntax error
 *        2- output file does not exists or can not open to write
 **/
int parseCmd
(
	int argc,
	char* argv[],
	struct Config *value
)
{
	// GTFS https://developers.google.com/transit/gtfs/reference/?csw=1
	struct arg_file *a_gtfs_folder = arg_file0("G", "gtfs-folder", "<GTFS folder>", "Default " DEF_GTFS_FOLDER);
	struct arg_int *a_time_zone_offset = arg_int0(NULL, "tzo", "<seconds>", "Time zone for GTFS. 32400 for Asia/Yakutsk. Default- your time zone");

	struct arg_str *a_unicode_range_name = arg_str0("u", "unicode", "<name>", "Valid names: " UNICODE_RANGE_NAMES ". Default " DEF_UNICODE_RANGE_NAME);
	struct arg_str *a_font_name = arg_str0("f", "font-name", "<name>", "Font name. Default " DEF_FONT_NAME);
	struct arg_int *a_font_size = arg_int0("s", "font-size", "<number>", "Font size. Default- " DEF_FONT_SIZE_S);
	
	struct arg_lit *a_help = arg_lit0("h", "help", "Show this help");
	struct arg_end *a_end = arg_end(20);

	void* argtable[] = { 
		a_gtfs_folder, a_time_zone_offset,
		a_unicode_range_name, a_font_name, a_font_size, 
		a_help, a_end 
	};

	int nerrors;

	// verify the argtable[] entries were allocated successfully
	if (arg_nullcheck(argtable) != 0)
	{
		arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
		return 1;
	}
	// Parse the command line as defined by argtable[]
	nerrors = arg_parse(argc, argv, argtable);

	// special case: '--help' takes precedence over error reporting
	if ((a_help->count) || nerrors)
	{
		if (nerrors)
			arg_print_errors(stderr, a_end, progname);
		printf("Usage: %s\n",  progname);
		arg_print_syntax(stdout, argtable, "\n");
		printf("GTFS editor\n");
		arg_print_glossary(stdout, argtable, "  %-25s %s\n");
		arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
		return 1;
	}

	if (a_time_zone_offset->count)
		value->timezone_offset = *a_time_zone_offset->ival;
	else
		value->timezone_offset = getCurrentTimeOffset();
	
	if (a_gtfs_folder->count)
		value->gtfs_path = *a_gtfs_folder->filename;
	else
		value->gtfs_path = DEF_GTFS_FOLDER;

	if (a_unicode_range_name->count)
		value->unicode_range_name = *a_unicode_range_name->sval;
	else
		value->unicode_range_name = DEF_UNICODE_RANGE_NAME;

	if (a_font_name->count)
		value->font_name = *a_font_name->sval;
	else
		value->font_name = DEF_FONT_NAME;

	if (a_font_size->count)
		value->font_size = *a_font_size->ival;
	else
		value->font_size = DEF_FONT_SIZE;

	arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
	return 0;
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

void addWeekDays
(
	bool weekdays[7],
	int x,
	int y,
	int stepx,
	int stepy
)
{
	const char* wdn[7] = {u8"Пн", u8"Вт", u8"Ср", u8"Чт", u8"Пт", u8"Сб", u8"Вс" };
	for (int w = 0; w < 7; w++)
	{
		ImGui::SetCursorPosX(x + w * stepx);
		ImGui::SetCursorPosY(y + w * stepy);
		if (ImGui::Checkbox(wdn[w], &weekdays[w]))
		{
		}
	}
}

int imgui1(Config *config)
{
	// Setup window
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		return 1;
	GLFWwindow* window = glfwCreateWindow(640, 480, "GTFS editor", NULL, NULL);
	glfwMakeContextCurrent(window);

	// Setup ImGui binding
	ImGui_ImplGlfw_Init(window, true);

	// Load Fonts
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	ImFontConfig fontConfig;
	fontConfig.MergeMode = true;
	const ImWchar*  range;

	if (config->unicode_range_name == "korean") 
		range = io.Fonts->GetGlyphRangesKorean();
	else
		if (config->unicode_range_name == "japanese") 
			range = io.Fonts->GetGlyphRangesJapanese();
		else
			if (config->unicode_range_name == "cyrillic") 
				range = io.Fonts->GetGlyphRangesCyrillic();
			else
				if (config->unicode_range_name == "thai") 
					range = io.Fonts->GetGlyphRangesThai();
				else
					range = io.Fonts->GetGlyphRangesDefault();

	io.Fonts->AddFontFromFileTTF(config->font_name.c_str(), config->font_size, &fontConfig, range);

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) 
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	});

	int padding_weekdays = 48;
	int padding_listbox = 4;
	int padding_bottom = 80;
	int items_count = 10;
	int current_item = -1;
	char* items[10] = {"8:00", "9:00", "3", "4", "5", "6", "7", "8", "9", "10"};
	bool weekdays[7] = {false, false, false, false, false, false, false };

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		// Close window on pressing ESC

		ImGui_ImplGlfw_NewFrame();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		
		int list_width = display_w / 3 - padding_listbox;

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(display_w, display_h), ImGuiSetCond_Always);
		
		ImGui::Begin(u8"", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders |ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar);

		// ImGui::ListBoxHeader(u8"Мар", items_count);
		if (ImGui::Combo(u8"Маршрут", &current_item, items, items_count))
		{
			// current_item++;
		}

		if (ImGui::Combo(u8"Период", &current_item, items, items_count))
		{
			// current_item++;
		}

	
		addWeekDays(weekdays, ImGui::GetCursorPosX(), ImGui::GetCursorPosY(), padding_weekdays, 0);

		int lines_per_list = (display_h - padding_bottom) / 20;
		double x = ImGui::GetCursorPosX();
		double y = ImGui::GetCursorPosY();

		ImGui::PushItemWidth(list_width);
		
		ImGui::Text(u8"Отправление");
		double y1 = ImGui::GetCursorPosY();
		if (ImGui::ListBox(u8" ", &current_item, items, items_count, lines_per_list ))
		{
			// current_item++;
		}

		ImGui::SetCursorPosX(x + list_width + padding_listbox);
		ImGui::SetCursorPosY(y);
		ImGui::Text(u8"Выходные");
		ImGui::SetCursorPosX(x + list_width + padding_listbox);
		ImGui::SetCursorPosY(y1);
		ImGui::PushItemWidth(list_width);
		if (ImGui::ListBox(u8"  ", &current_item, items, items_count, lines_per_list))
		{
			// current_item++;
		}

		ImGui::SetCursorPosX(x + 2 * (list_width + padding_listbox));
		ImGui::SetCursorPosY(y);
		ImGui::Text(u8"Рабочие");
		ImGui::SetCursorPosX(x + 2 * (list_width + padding_listbox));
		ImGui::SetCursorPosY(y1);
		ImGui::PushItemWidth(list_width);
		if (ImGui::ListBox(u8"   ", &current_item, items, items_count, lines_per_list ))
		{
			// current_item++;
		}
	
		if (ImGui::Button("Закрыть")) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::End();
		// Rendering
		glViewport(0, 0, display_w, display_h);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplGlfw_Shutdown();
	glfwTerminate();
}

int main(int argc, char** argv)
{
	struct Config config;
	int r;
	if (r = parseCmd(argc, argv, &config))
		exit(r);
	imgui1(&config);
	return 0;
}
