/*
  sdobjects

  Main program module

  The MIT License (MIT)

  Copyright (c) 2022 James Haley

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "elib/elib.h"
#include "elib/m_argv.h"
#include "elib/misc.h"
#include "elib/qstring.h"
#include "hal/hal_init.h"
#include "hal/hal_opendir.h"
#include "sdobjects.h"

static bool printresults;

//
// Process a single file
//
static void ProcessFile(const char *inputpath)
{
    ebyte *pdata;
    const size_t len = M_ReadFile(inputpath, &pdata);
    if(pdata != nullptr && len > 0)
    {
        const EUniquePtr<ebyte> updata { pdata };
        const RLObjects objects = ReadRLObjects(pdata, len);
        if(printresults)
            PrintRLObjects(objects);
    }
    else
        std::printf("Could not open file '%s' for input\n", inputpath);
}

//
// Process a whole directory
//
static void ProcessDirectory(const char *inputpath)
{
    hal_dir_t *const dir = hal_directory.openDir(inputpath);
    if(dir == nullptr)
    {
        std::printf("Could not open directory '%s' for input\n", inputpath);
        return;
    }

    hal_direntry_t *ent;
    while((ent = hal_directory.readDir(dir)) != nullptr)
    {
        const qstring fullname = qstring(inputpath) / hal_directory.getEntryName(ent);
        if(hal_platform.fileExists(fullname.c_str()) == HAL_TRUE) // if not a directory
            ProcessFile(fullname.c_str());
    }

    hal_directory.closeDir(dir);
}

//
// Main program
//
void sdobjects_main()
{
    // init elib HAL
    HAL_Init();

    const EArgManager &args = EArgManager::GetGlobalArgs();
    const char *const *argv = args.getArgv();
    const int          argc = args.getArgc();

    if(args.findArgument("-print"))
        printresults = true;
    else
        std::printf("sbobjects - SNES Doom objects converter program\n");

    // process individual files
    int p = 1;
    while((p = args.indexOf("-file", p)) != 0)
    {
        while(++p < argc)
        {
            if(argv[p][0] == '-')
                break;
            ProcessFile(argv[p]);
        }
    }

    // process directories
    p = 1;
    while((p = args.indexOf("-dir", p)) != 0)
    {
        while(++p < argc)
        {
            if(argv[p][0] == '-')
                break;
            ProcessDirectory(argv[p]);
        }
    }
}

// EOF
