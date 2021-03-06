/*
 * LK8000 Tactical Flight Computer -  WWW.LK8000.IT
 * Released under GNU/GPL License v.2
 * See CREDITS.TXT file for authors and copyrights
 *
 * File:   Sound.h
 * Author: Bruno de Lacheisserie
 *
 * Created on January 29, 2015, 10:11 PM
 */
#include "../Sound.h"
#include "externs.h"
#include <SDL_mixer.h>
#include <map>
#include "Util/ConstBuffer.hpp"
#include "resource_data.h"

// libsdl-mixer1.2-dev package required


static bool bSoundInit = false; // this is true only if #SDL_Mixer is initialized.
static bool bSoundFile = false; // this is true only if "_System/_Sounds" directory exists.
static TCHAR szSoundPath[MAX_PATH] = {}; // path of Sound file, initialized by  #SoundGlobalInit end never change;

/*
 * we use map for store sound when is used for first play
 * no need to load for next play.
 * all sound a free at shutdown.
 * for free before, we need to detect when sound ply is end.
 */
typedef std::map<std::tstring, Mix_Chunk*> audioChunkCache_t;
static audioChunkCache_t audioChunkCache; 

SoundGlobalInit::SoundGlobalInit() {
    if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096 ) != -1 ) {
        bSoundInit = true;
    } else {
        StartupStore(_T("ERROR Failed to initialize Audio Mixer <%s>%s"),Mix_GetError(),NEWLINE);
        StartupStore(_T("------ LK8000 SOUNDS NOT WORKING!%s"),NEWLINE);
    }

    if(bSoundInit) {
        TCHAR srcfile[MAX_PATH];
        SystemPath(szSoundPath,TEXT(LKD_SOUNDS));
        _stprintf(srcfile,TEXT("%s%s_SOUNDS"), szSoundPath, _T(DIRSEP));
        if ( lk::filesystem::exist(srcfile) ) {
            bSoundFile = true;
        } else {
            StartupStore(_T("ERROR NO SOUNDS DIRECTORY CHECKFILE <%s>%s"),srcfile,NEWLINE);
            StartupStore(_T("------ LK8000 SOUNDS NOT WORKING!%s"),NEWLINE);
        }    
    }      
}
  
SoundGlobalInit::~SoundGlobalInit() {
    
    for(auto chunk : audioChunkCache) {
        Mix_FreeChunk(chunk.second);
    }
    
    if(bSoundInit) {
        Mix_CloseAudio();
    }
}

bool SetSoundVolume() {
    // TODO : need to implement ?
    return false;
}

  
void LKSound(const TCHAR *lpName) {
    if(!bSoundInit && !EnableSoundModes) {
        return;
    }
    
    // Check if AudioChunk is already loaded.
    auto ib = audioChunkCache.insert(std::make_pair(lpName, nullptr));
    if(ib.second) {
        // Load AudioChunk
        TCHAR sndfile[MAX_PATH];
        _stprintf(sndfile,_T("%s%s%s"),szSoundPath, _T(DIRSEP), lpName);
        
        ib.first->second = Mix_LoadWAV(sndfile);
    }
    
    if(ib.first->second) {
        Mix_PlayChannel(-1, ib.first->second, 0);
    }

}

void PlayResource (const TCHAR* lpName) {
    assert(lpName);
    
    if(!lpName && !bSoundFile && !bSoundInit && !EnableSoundModes) {
        return;
    }
    
    // Check if AudioChunk is already loaded.
    auto ib = audioChunkCache.insert(std::make_pair(lpName, nullptr));
    if(ib.second) {
        // Load AudioChunk
        ConstBuffer<void> sndBuffer = GetNamedResource(lpName);
        if(!sndBuffer.IsEmpty()) {
            SDL_RWops* SndBuffer = SDL_RWFromConstMem(sndBuffer.data, sndBuffer.size);
            if(SndBuffer) {
                ib.first->second = Mix_LoadWAV_RW(SndBuffer, 1);
            }
        }
    }
    
    if(ib.first->second) {
        Mix_PlayChannel(-1, ib.first->second, 0);
    }
}

