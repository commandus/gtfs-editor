/**
 * ./gtfs-editor
 */
#include <cstdlib>
#include <GLFW/glfw3.h>
#include "imgui.h"
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

const char* progname = "gtfs-editor";

#define DEF_GTFS_FOLDER			"gtfs"

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
	struct ClientConfig *value
)
{
	// GTFS https://developers.google.com/transit/gtfs/reference/?csw=1
	struct arg_file *a_gtfs_folder = arg_file0("G", "gtfs-folder", "<GTFS folder>", "Default " DEF_GTFS_FOLDER);
	struct arg_int *a_time_zone_offset = arg_int0(NULL, "tzo", "<seconds>", "Time zone for GTFS. 32400 for Asia/Yakutsk. Default- your time zone");

	struct arg_lit *a_help = arg_lit0("h", "help", "Show this help");
	struct arg_end *a_end = arg_end(20);

	void* argtable[] = { 
		a_gtfs_folder, a_time_zone_offset,
		a_help, a_end };

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

	arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
	return 0;
}

/**
 * Open file to read, skip UTF-8 BOM if exists.
 */
std::ifstream *openUtf8BOM(const std::string &fn)
{
	std::ifstream *ret = new std::ifstream(fn, std::ifstream::in);
	// remove byte order mark (BOM) 0xef 0xbb 0xbf
	unsigned char bom[3];
	ret->read((char*) bom, 3);
	if (!((bom[0] == 0xef) && (bom[1] == 0xbb) && (bom[2] == 0xbf)))
		ret->seekg(0);
	return ret;
}

int imgui0(void);
void setup(GLFWwindow* window);
void renderer(ImDrawData* draw_data);

int main(int argc, char** argv)
{
	struct ClientConfig config;
	int r;
	if (r = parseCmd(argc, argv, &config))
		exit(r);
	imgui0();
	return 0;
}


//-------------------------

int imgui0(void)
{

    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    static GLFWwindow* window {
        glfwCreateWindow(320, 240, "Simple example", NULL, NULL) 
    };

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    // Close window on pressing ESC
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    });

    setup(window);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();

        // Use ImGui functions between here and Render()
        ImGui::NewFrame();

        // This creates a window
        ImGui::Begin("Window Title Here");
        ImGui::Text("Hello, world!");
        ImGui::End();

        // ImGui functions end here
        ImGui::Render();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

/*! /brief Initialise the ImGuiIO struct.
 *
 *   The ImGuiIO struct is the main configuration and
 *   I/O between your application and ImGui.
 */
void setup(GLFWwindow* window)
{
    unsigned char* pixels;
    int width,
        height,
        display_width,
        display_height;
    GLuint g_FontTexture;

    ImGuiIO& io { ImGui::GetIO() };

    io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);

    // Upload texture to graphics system
    GLint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGenTextures(1, &g_FontTexture);
    glBindTexture(GL_TEXTURE_2D, g_FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);
    
    // Get display size
    glfwGetWindowSize(window, &width, &height);
    glfwGetFramebufferSize(window, &display_width, &display_height);

    io.DisplaySize = ImVec2((float)width, (float)height);
    io.RenderDrawListsFn = renderer;
    io.Fonts->TexID = (void *)(intptr_t)g_FontTexture;

    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);
}

/*! /brief Boilerplate function for OpenGL 2.0 rendering.
 *
 *  This function isn't written by us, but is mandatory
 *  boilerplate from the library. It can be copy/pasted
 *  into your projects, but should really be part of the
 *  library itself?
 */
void renderer(ImDrawData* draw_data)
{
    ImGuiIO& io { ImGui::GetIO() };
    int fb_width { (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x) };
    int fb_height { (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y) };

    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);

    // Setup viewport, orthographic projection matrix
    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, +1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Render command lists
    #define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const unsigned char* vtx_buffer = (const unsigned char*)&cmd_list->VtxBuffer.front();
        const ImDrawIdx* idx_buffer = &cmd_list->IdxBuffer.front();
        glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, pos)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, uv)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, col)));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer);
            }
            idx_buffer += pcmd->ElemCount;
        }
    }
    #undef OFFSETOF

    // Restore modified state
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
}
