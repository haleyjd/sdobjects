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

static bool    printresults;
static bool    doOutput;
static qstring outputpath;
static int     filenum;

//
// Construct output filename for THINGS lumps
//
static qstring GetOutputFilename(const char *inputpath)
{
    qstring outfn;

    // output THINGS.xy for input file with extension .xy
    const qstring inpstr { inputpath };
    if(const size_t inplen = inpstr.length(); inplen >= 3)
    {
        if(inpstr[inplen - 3] == '.')
        {
            if(ectype::isDigit(inpstr[inplen - 2]) && ectype::isDigit(inpstr[inplen - 1]))
                outfn = "THINGS" + qstring(&inpstr[inplen - 3]);
        }
    }

    // otherwise, use a monotonically increasing output number
    if(outfn.empty())
    {
        outfn = "THINGS." + qstring::ToString(filenum);
        ++filenum;
    }

    return outputpath / outfn;
}

//
// Process a single file
//
static void ProcessFile(const char *inputpath)
{
    EUniquePtr<ebyte> updata;
    const size_t len = M_ReadFileUnique(inputpath, updata);
    if(ebyte *const pdata = updata.get(); pdata != nullptr && len > 0)
    {
        const RLObjects objects = ReadRLObjects(pdata, len);
        if(printresults)
            PrintRLObjects(objects);
        if(doOutput)
        {
            const qstring outname { GetOutputFilename(inputpath) };
            if(WriteTHINGS(objects, outname.c_str()) == false)
                std::fprintf(stderr, "Failed to write output file '%s'\n", outname.c_str());
        }
    }
    else
        std::fprintf(stderr, "Could not open file '%s' for input\n", inputpath);
}

//
// Process a whole directory
//
static void ProcessDirectory(const char *inputpath)
{
    hal_dir_t *const dir = hal_directory.openDir(inputpath);
    if(dir == nullptr)
    {
        std::fprintf(stderr, "Could not open directory '%s' for input\n", inputpath);
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
        std::printf("sdobjects - SNES Doom objects converter program\n\n");

    if(const int outidx = args.getArgParameters("-out", 1); outidx != 0)
    {
        doOutput   = true;
        outputpath = argv[outidx];
        if(hal_platform.makeDirectory(outputpath.c_str()) == HAL_FALSE)
        {
            std::fprintf(stderr, "Could not create output directory '%s'\n", outputpath.c_str());
            doOutput = false;
        }
    }

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
