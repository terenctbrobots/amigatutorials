#include <libraries/mui.h>

#include <proto/exec.h>           // Include this so we can access the exec.library to open up the MUI shared library
#include <proto/intuition.h>       // Include this so we can access IDoMethod
#include <proto/muimaster.h>    // Include this for MUI
#include <stdio.h>


#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))

int init();
void cleanup(APTR app);

struct Library* MUIMasterBase;
struct MUIMasterIFace *IMUIMaster;

int init()
{
    // Activate shared library for use
    if ((MUIMasterBase = (struct Library*)  OpenLibrary(MUIMASTER_NAME,MUIMASTER_VMIN)) == NULL)
    {
    	printf("Failed to open %s",MUIMASTER_NAME);
		return -1;
    }
    
    // Access shared library interface
    if ((IMUIMaster = (struct MUIMasterIFace*) GetInterface(MUIMasterBase,"main",1,NULL)) == NULL)
    {
    	printf("Cannot open interface to %s",MUIMASTER_NAME);
    	return -1;
    }
}

void cleanup(APTR app)
{
    if (app)
    {
    	MUI_DisposeObject(app);
    }
    
    // Clean up access to interface and window allowing system to reclaim memory allocated if any
    if (IMUIMaster)
    {
    	DropInterface((struct Interface*) IMUIMaster);
    }
    
    if (MUIMasterBase)
    {
    	CloseLibrary(MUIMasterBase);
    }
}


int main(int argc, char *argv[])
{
	APTR app,window;
    
    if (init() < 0)
    {
		cleanup(NULL);
		return -1;
	}
    
	app = ApplicationObject,
			MUIA_Application_Title, "MUI Hello World OS4 Version",
 			MUIA_Version, "$VER, Window",
            	SubWindow, window = WindowObject,
								MUIA_Window_Title,"Hello Windows OS4 Version",
                                MUIA_Window_ID,MAKE_ID('H','E','L','L'),
                                MUIA_Window_Activate, TRUE,
                                WindowContents, VGroup,
                                	Child, TextObject,
										MUIA_Text_Contents, "Hello World from a MUI Window.\n"
										"Close window or hit CTRL-C to exit.",
										MUIA_Text_Copy, FALSE,
										MUIA_Text_SetMax, TRUE,
								End,
							End,
				End,
			End;
 
	if (app == NULL)
	{
		cleanup(NULL);
		return -1;
	}
    
	IDoMethod(window,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,
		app,2,MUIM_Application_ReturnID,MUIV_Application_ReturnID_Quit);

	set(window,MUIA_Window_Open,TRUE);

	{
		ULONG sigs = 0;

		while ((LONG)IDoMethod(app,MUIM_Application_NewInput,&sigs) != MUIV_Application_ReturnID_Quit)
		{
			if (sigs)
			{
				sigs = Wait(sigs | SIGBREAKF_CTRL_C);
				if (sigs & SIGBREAKF_CTRL_C) break;
			}
		}
	}

	set(window,MUIA_Window_Open,FALSE);
	cleanup(app);

	return 0;
}
