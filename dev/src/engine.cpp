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

// Engine integration with Lobster VM.

#include "lobster/stdafx.h"

#include "lobster/compiler.h"  // For RegisterBuiltin().

#include "lobster/sdlinterface.h"

#include "lobster/compiler.h"

using namespace lobster;

extern void AddGraphics(NativeRegistry &nfr);
extern void AddFont(NativeRegistry &nfr);
extern void AddSound(NativeRegistry &nfr);
extern void AddPhysics(NativeRegistry &nfr);
extern void AddNoise(NativeRegistry &nfr);
extern void AddMeshGen(NativeRegistry &nfr);
extern void AddCubeGen(NativeRegistry &nfr);
extern void AddOcTree(NativeRegistry &nfr);
extern void AddOcTreePhysics(NativeRegistry &nfr);
extern void AddVR(NativeRegistry &nfr);
extern void AddSteam(NativeRegistry &nfr);
extern void AddIMGUI(NativeRegistry &nfr);

namespace lobster {

FileLoader EnginePreInit(NativeRegistry &nfr) {
    RegisterBuiltin(nfr, "graphics",  AddGraphics);
    RegisterBuiltin(nfr, "font",      AddFont);
    RegisterBuiltin(nfr, "sound",     AddSound);
    RegisterBuiltin(nfr, "physics",   AddPhysics);
    RegisterBuiltin(nfr, "noise",     AddNoise);
    RegisterBuiltin(nfr, "meshgen",   AddMeshGen);
    RegisterBuiltin(nfr, "cubegen",   AddCubeGen);
    RegisterBuiltin(nfr, "octree",    AddOcTree);
    RegisterBuiltin(nfr, "octreeph",  AddOcTreePhysics);
    RegisterBuiltin(nfr, "vr",        AddVR);
    RegisterBuiltin(nfr, "steam",     AddSteam);
    RegisterBuiltin(nfr, "imgui",     AddIMGUI);
    return SDLLoadFile;
}

}  // namespace lobster
