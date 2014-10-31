// Copyright 2014 Wouter van Oortmerssen. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

static const LineInfo &LookupLine(int pos, const vector<LineInfo> &_lineinfo)
{
    const LineInfo *lineinfo = _lineinfo.data();
    int size = _lineinfo.size();
    assert(size);

    for (;;)    // quick hardcoded binary search
    {
        if (size == 1) return *lineinfo;

        int nsize = size / 2;
        if (lineinfo[nsize].bytecodestart <= pos) { lineinfo += nsize; size -= nsize; }
        else size = nsize;
    }
}

static void LvalDisAsm(FILE *f, int *&ip)
{
    #define F(N) #N,
    static const char *lvonames[] = { LVALOPNAMES };
    #undef F

    fprintf(f, "%s ", lvonames[*ip++]);
}

static int *DisAsmIns(FILE *f, SymbolTable &st, int *ip, int *code, const LineInfo &li)
{
    #define F(N) #N,
    static const char *ilnames[] = { ILNAMES };
    #undef F

    // FIXME: some indication of the filename, maybe with a table index?
    fprintf(f, "I %d \tL %d \t%s ", int(ip - code), li.line, ilnames[*ip]);

    switch(*ip++)
    {
        case IL_PUSHINT:
        case IL_PUSHFUN:
        case IL_CALLV:
        case IL_CALLVCOND:
        case IL_DUP:
        case IL_CONT1:
        case IL_JUMP:
        case IL_JUMPFAIL:
        case IL_JUMPFAILR:
        case IL_JUMPNOFAIL:
        case IL_JUMPNOFAILR:
        case IL_TT:
        case IL_TTSTRUCT:
        case IL_LOGREAD:
            fprintf(f, "%d", *ip++);
            break;

        case IL_RETURN:
        {
            auto id = *ip++;
            fprintf(f, "%s", id >= 0 ? st.functiontable[id]->name.c_str() : inttoa(id));
            break;
        }

        case IL_CALL:
        case IL_CALLMULTI:
        {
            auto nargs = *ip++;
            auto id = *ip++;
            auto bc = *ip++;
            fprintf(f, "%d %s %d", nargs, st.functiontable[id]->name.c_str(), bc);
            break;
        }

        case IL_NEWVEC:
        {
            auto t = *ip++;
            auto nargs = *ip++;
            fprintf(f, "%s %d", t >= 0 ? st.ReverseLookupType(t).c_str() : "vector", nargs);
            break;
        }

        case IL_BCALL:
        {
            int a = *ip++;
            fprintf(f, "%s %d", natreg.nfuns[a]->name.c_str(), *ip++);
            break;
        }

        case IL_LVALVAR:
            LvalDisAsm(f, ip);
        case IL_PUSHVAR:
            fprintf(f, "%s", st.ReverseLookupIdent(*ip++).c_str());
            break;

        case IL_LVALFLDO:
        case IL_LVALFLDT:
        case IL_LVALLOC:
           LvalDisAsm(f, ip);
        case IL_PUSHFLDT:
        case IL_PUSHFLDO:
        case IL_PUSHLOC:
            fprintf(f, "%d", *ip++);
            break;

        case IL_LVALFLDC:
           LvalDisAsm(f, ip);
        case IL_PUSHFLDC:
        {
            int si = *ip++;
            int o1 = *ip++;
            fprintf(f, "%d %d %d", si, o1, *ip++);
            break;
        }

        case IL_LVALIDX:
            LvalDisAsm(f, ip);
            break;

        case IL_PUSHFLT:
            fprintf(f, "%f", *(float *)ip);
            ip++;
            break;

        case IL_PUSHSTR:
            fprintf(f, "\"");
            while(*ip) fprintf(f, "%c", *ip++);
            ip++;
            fprintf(f, "\"");
            break;

        case IL_FUNSTART:
        {
            int n = *ip++;
            while (n--) fprintf(f, "%s ", st.ReverseLookupIdent(*ip++).c_str());
            n = *ip++; 
            fprintf(f, "=> ");
            while (n--) fprintf(f, "%s ", st.ReverseLookupIdent(*ip++).c_str());
            n = *ip++;
            if (n) fprintf(f, "(log = %d)", n);
            break;
        }

        case IL_ISTYPE:
        {
            int idx = *ip++;
            fprintf(f, "%d %d", idx, *ip++);
            break;
        }

        case IL_CORO:
        {
            fprintf(f, "%d", *ip++);
            int n = *ip++;
            for (int i = 0; i < n; i++) fprintf(f, " v%d", *ip++);
            break;
        }

        case IL_FIELDTABLES:
            fprintf(f, "%d", *ip);
            ip = code + *ip;
            break;

        case IL_FUNMULTI:
        {
            auto n = *ip++;
            auto nargs = *ip++;
            fprintf(f, "%d %d", n, nargs);
            ip += (nargs * 2 + 1) * n;
        }

    }

    return ip;
}

static void DisAsm(FILE *f, SymbolTable &st, int *code, const vector<LineInfo> &lineinfo, int len)
{
    int *ip = code;
    while (ip < code + len)
    {
        ip = DisAsmIns(f, st, ip, code, LookupLine(ip - code, lineinfo));
        fprintf(f, "\n");
    }
}



