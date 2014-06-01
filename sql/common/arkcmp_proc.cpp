/**********************************************************************
// @@@ START COPYRIGHT @@@
//
// (C) Copyright 2013-2014 Hewlett-Packard Development Company, L.P.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
// @@@ END COPYRIGHT @@@
**********************************************************************/
/* -*-C++-*-
 *****************************************************************************
 *
 * File:         arkcmp_proc.cpp
 * Description:  This is the main entry point for arkcmp procedural interface. 
 *               
 *               
 *
 *
 * Created:      7/9/2013
 * Language:     C++
 *
 *
 *
 *****************************************************************************
 */


#undef  NA_COMPILE_INSTANTIATE
#define NA_COMPILE_INSTANTIATE

#include "Platform.h"
#include "NewDel.h"
#include <fstream>
#include <string.h>


#include "seabed/ms.h"
#include "seabed/fs.h"
extern void my_mpi_fclose();
#include "SCMVersHelp.h"
DEFINE_DOVERS(tdm_arkcmp)


#include "SqlExportDllDefines.h"

#include "CmpMessage.h"
#include "CmpConnection.h"
#include "CmpContext.h"
#include "CmpCommon.h"
#include "CmpStoredProc.h"
#include "CmpErrLog.h"
#include "CompException.h"
// #include "StaticCompiler.h"
#include "logmxevent.h"
#include "QCache.h"
#include "QueryCacheSt.h"
#include "NATableSt.h"
#include "ComCextdecs.h"

#define CLI_DLL
#include "SQLCLIdev.h"
#undef CLI_DLL

#include "EHException.h"
#include "CmpEHCallBack.h"

#include "ObjectNames.h"
#include "Globals.h"
#include "CliSemaphore.h"





THREAD_P NABoolean DisplayGraph = FALSE;

ostream &operator<<(ostream &dest, const ComDiagsArea& da);


// mainNewHandler_CharSave and mainNewHandler are used in the error
// handling when running out of virtual memory for the main program.
//
// Save 4K bytes of memory for the error handling when running out of VM.

static char* mainNewHandler_CharSave = new char[4096];


static Int32  mainNewHandler(size_t s)

{
  if (mainNewHandler_CharSave)
  {
    delete[] mainNewHandler_CharSave;
    mainNewHandler_CharSave = NULL;
    CmpErrLog((char *)"Memory allocation failure");
    ArkcmpFatalError(ARKCMP_ERROR_PREFIX "Out of virtual memory.", NOMEM_SEV);
  }
  return 0;

}

static void longJumpHandler()
{
  // check if we have emergency memory for reporting error.
  if (mainNewHandler_CharSave == 0)
    exit(1);

  delete [] mainNewHandler_CharSave;
  mainNewHandler_CharSave = 0;
  ArkcmpFatalError(ARKCMP_ERROR_PREFIX "Assertion failed (thrown by longjmp).");
}

extern THREAD_P jmp_buf CmpInternalErrorJmpBuf;

static void initializeArkcmp()
{
  CmpInternalErrorJmpBufPtr = &CmpInternalErrorJmpBuf;
  // noop for now
}

void deinitializeArkcmp()
{
  // noop for now
}



//extern void bloom_filter_test();
//extern void  hdfs_list_test(int argc, char **argv);

Int32 arkcmp_main_entry()
{

  if (getenv("SQL_CMP_MSGBOX_PROCESS") != NULL )
     MessageBox(NULL, "Server: Process Launched", "tdm_arkcmp",
		MB_OK|MB_ICONINFORMATION);
  // The following call causes output messages to be displayed in the
  // same order on NSK and Windows.
  cout.sync_with_stdio();


  initializeArkcmp();
  

  try
  {
    

    { // a local ctor scope, within a try block


      // Set up the context info for the connection, it contains the variables
      // persistent through each statement loops.

      CmpContext *context=NULL;
      NAHeap *parentHeap = GetCliGlobals()->getCurrContextHeap();
      NAHeap *cmpContextHeap = new (parentHeap)
                      NAHeap((char *)"Cmp Context Heap",
                           parentHeap,
                           (Lng32)524288);
      try
      {
        CLISemaphore *cliSemaphore;
        cliSemaphore = GetCliGlobals()->getSemaphore();
        cliSemaphore->get();
        context= new (cmpContextHeap)CmpContext(CmpContext::IS_DYNAMIC_SQL,
			 cmpContextHeap);
        cliSemaphore->release();

      }
      catch (...)
      {
        ArkcmpErrorMessageBox
          (ARKCMP_ERROR_PREFIX "- Cannot initialize Compiler global data.", 
           ERROR_SEV, FALSE, FALSE, TRUE);
        return(1);
      }

      //  moved down the IdentifyMyself so that it can be determined that the
      //  context has not yet been set up
      IdentifyMyself::SetMyName(I_AM_EMBEDDED_SQL_COMPILER);
      context->setIsEmbeddedArkcmp(TRUE);
      context->initContextGlobals();
      context->envs()->cleanup();

      // Clear the CLI globals context area if there are any leftover CLI diags 
      //This is a fresh context and shouldn't have CLI diags.
      SQL_EXEC_ClearDiagnostics(NULL);

      assert(cmpCurrentContext == context);
    
      // Clear the SQL text buffer in the event logging area.
      cmpCurrentContext->resetLogmxEventSqlText();

    }
  
  }

  catch(BaseException& bE)
    {
      char msg[500];
      sprintf(msg, "%s BaseException: %s %d", ARKCMP_ERROR_PREFIX,
              bE.getFileName(), bE.getLineNum());
      ArkcmpFatalError(msg);
  }
  catch(...)
    {
      ArkcmpFatalError(ARKCMP_ERROR_PREFIX "Fatal exception.");
    }

   ENDTRANSACTION();
#ifdef CLOSED_SOURCE_1
#ifdef NA_CMPDLL
  // This flag causes sqlark_requester flag to be set and eventually telling
  // TSE to set the FL_SQL_ANSI_TABLE in the file label, otherwise, we could
  // get error 1163 (FERESERVEDNAME) while doing DDL operations via the
  // embedded arkcmp
  if (CatFsSetSqlLicense(1))
    {
      SQLMXLoggingArea::logExecRtInfo(NULL, 0,
                                "Cannot get FS2 license.", 0);
    }
#endif  // NA_CMPDLL
#endif


  return 0;  
}


void arkcmp_main_exit()
{

}