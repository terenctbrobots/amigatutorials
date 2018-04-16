#include <proto/exec.h>           // Include this so we can access the exec.library to open up the MUI shared library
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
    if ((MUIMasterBase = IExec->OpenLibrary(MUIMASTER_NAME,MUIMASTER_VMIN)) == NULL)
    {
    	printf("Failed to open %s",MUIMASTER_NAME);
   	return -1;
    }
    
    // Access shared library interface
    if ((IMUIMaster = (struct MUIMasterIFace*) IExec->GetInterface(MUIMasterBase,"main",1,NULL)) == NULL)
    {
    	printf("Cannot open interface to %s",MUIMASTER_NAME);
    	return -1;
    }
}

void cleanup(APTR app)
{
    if (app)
    {
    	IMUIMaster->MUI_DisposeObject(app);
    }
    
    // Clean up access to interface and window allowing system to reclaim memory allocated if any
    if (IMUIMaster)
    {
    	IExec->DropInterface((struct Interface*) IMUIMaster);
    }
    
    if (MUIMasterBase)
    {
    	IExec->CloseLibrary(MUIMasterBase);
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
    
    app = IMUIMaster->MUI_NewObject(MUIC_Application,
                                                         MUIA_Application_Title, "MUI Hello World OS4 Version",
                                                         MUIA_Version, "$VER, Window",
                                                         SubWindow, window = IMUIMaster->MUI_NewObject(MUIC_Window,
                                                                                                                                     MUIA_Window_Title,"Hello Windows OS4 Version",
                                                                                                                                     MUIA_Window_ID,MAKE_ID('H','E','L','L'),
                                                                                                                                     MUIA_Window_Activate, TRUE,
                                                                                                                                     MUIA_Window_RootObject,
                                                                                                                                     IMUIMaster->MUI_NewObject(MUIC_Group,
                                                                                                                                         MUIA_Group_Child,
                                                                                                                                            IMUIMaster->MUI_NewObject(MUIC_Text,
                                                                                                                                                                                        MUIA_Text_Contents,"Hello World")
                                                                                                                                     )));
                                                                                           
    
    
    if (app == NULL)
    {
       cleanup(NULL);
    }   

    return 0;
}
